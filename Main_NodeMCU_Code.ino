#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>      // Include the Wi-Fi library
#include <ESP8266HTTPClient.h> // Include the HTTP client library
// #include <WiFiClientSecure.h> // REMOVED - Using standard HTTP now

// === WIFI Configuration (REQUIRED) ===
const char* ssid = "SSID";     // Replace with your WiFi network name
const char* password = "PASS"; // Replace with your WiFi password
// UPDATED to use HTTP instead of HTTPS
const char* serverUrl = "http://10.0.3.1:8000/entry"; 

// === Pin Definitions for NodeMCU (ESP8266) ===
#define SS_PIN D2 // NodeMCU D8 (GPIO15) connects to SDA (SS) on MFRC522
#define RST_PIN D1 // NodeMCU D3 (GPIO0) connects to RST on MFRC522

// === Status LED Pins (Using D4 and D1 to avoid conflict with MFRC522 RST pin D3) ===
#define SUCCESS_PIN D4 // D4 (GPIO2): HIGH for HTTP 201 (Success)
#define FAILURE_PIN D3 // D1 (GPIO5): HIGH for Other HTTP Status or Request Failure

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522::MIFARE_Key key;

// ==========================================================
// TARGET ADDRESSES
// ==========================================================

// Sector 0
const byte sector = 0;
const byte blockAddr1 = 1; // Absolute block address for the first data block in Sector 0
const byte blockAddr2 = 2; // Absolute block address for the second data block in Sector 0

// Buffer to store the read data
byte readBuffer1[18]; // 16 bytes data + 2 bytes CRC
byte readBuffer2[18];

// Size of the data block (16 bytes)
byte bufferSize = 18; 
byte dataSize = 16; 

// === Helper Function Prototypes ===
String GetErrorMessage(MFRC522::StatusCode code);
void readAndPostSectorData(byte sectorNum, byte addr1, byte addr2);


// === Error Message Helper Function ===
// This function helps translate MFRC522 status codes into readable messages.
String GetErrorMessage(MFRC522::StatusCode code) {
  switch (code) {
    case MFRC522::STATUS_OK:             return F("Success.");
    case MFRC522::STATUS_ERROR:          return F("Error in communication.");
    case MFRC522::STATUS_COLLISION:      return F("Collision detected.");
    case MFRC522::STATUS_TIMEOUT:        return F("Timeout occurred.");
    case MFRC522::STATUS_NO_ROOM:        return F("A buffer is not big enough.");
    case MFRC522::STATUS_INTERNAL_ERROR: return F("Internal error in the code.");
    case MFRC522::STATUS_INVALID:        return F("Invalid argument.");
    case MFRC522::STATUS_CRC_WRONG:      return F("The CRC_A does not match.");
    case MFRC522::STATUS_MIFARE_NACK:    return F("A NAK has been received.");
    default:                             return F("Unknown error.");
  }
}

void setup() {
  Serial.begin(115200);
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522

  // Initialize LED pins
  pinMode(SUCCESS_PIN, OUTPUT);
  pinMode(FAILURE_PIN, OUTPUT);
  
  // Ensure LEDs start off
  digitalWrite(SUCCESS_PIN, LOW);
  digitalWrite(FAILURE_PIN, LOW);

  Serial.println("MFRC522 RFID Card Reader & HTTP Poster ready.");

  // Prepare Key A (default key for demonstration)
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF; // Default key is six bytes of 0xFF
  }

  // === Connect to Wi-Fi ===
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi. Check credentials.");
  }
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("\nCard detected. UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Perform the read and post operation
  readAndPostSectorData(sector, blockAddr1, blockAddr2);

  // Halt PICC and go into power down
  mfrc522.PICC_HaltA();
  // Stop encryption on the Pcd
  mfrc522.PCD_StopCrypto1(); 

  delay(3000); // Wait 3 seconds before checking for another card
}

/**
 * Reads data from two blocks in a sector, formats it, and sends it via HTTP POST.
 * Also controls D4 (Success) and D1 (Failure) pins based on HTTP response code.
 */
/**
 * Reads data from two blocks in a sector, formats it, and sends it via HTTP POST.
 * Also controls D4 (Success) and D1 (Failure) pins based on HTTP response code.
 */
/**
 * Reads data from two blocks in a sector, formats it, and sends it via HTTP POST.
 * Also controls D4 (Success) and D1 (Failure) pins based on HTTP response code.
 */
void readAndPostSectorData(byte sectorNum, byte addr1, byte addr2) {
  // Turn off previous indicators
  digitalWrite(SUCCESS_PIN, LOW);
  digitalWrite(FAILURE_PIN, LOW);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Skipping POST: WiFi is not connected. Setting D1 HIGH.");
    digitalWrite(FAILURE_PIN, HIGH);
    delay(1000);
    digitalWrite(FAILURE_PIN, LOW);
    return;
  }
  
  MFRC522::StatusCode status;
  
  // ===================================================
  // 1. Authenticate with the Sector Trailer (Key A)
  // ===================================================
  byte trailerAddr = (sectorNum * 4) + 3; 

  Serial.print("Authenticating Sector "); Serial.print(sectorNum); 
  Serial.print(" (Trailer Block "); Serial.print(trailerAddr); Serial.println(")...");

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerAddr, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK) {
    Serial.print("Authentication failed: ");
    digitalWrite(FAILURE_PIN, HIGH); delay(1000); digitalWrite(FAILURE_PIN, LOW);
    Serial.println(GetErrorMessage(status)); 
    return;
  }
  Serial.println("Authentication successful.");

  // ===================================================
  // 2. Read Data Block 1 (Name)
  // ===================================================
  byte count = bufferSize;
  status = mfrc522.MIFARE_Read(addr1, readBuffer1, &count);
  
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading Block "); Serial.print(addr1); 
    Serial.print(" failed: "); Serial.println(GetErrorMessage(status));
    digitalWrite(FAILURE_PIN, HIGH); delay(1000); digitalWrite(FAILURE_PIN, LOW);
    return;
  }
  
  // ===================================================
  // 3. Read Data Block 2 (Decimal ID)
  // ===================================================
  count = bufferSize;
  status = mfrc522.MIFARE_Read(addr2, readBuffer2, &count);
  
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading Block "); Serial.print(addr2); 
    Serial.print(" failed: "); Serial.println(GetErrorMessage(status));
    digitalWrite(FAILURE_PIN, HIGH); delay(1000); digitalWrite(FAILURE_PIN, LOW);
    return;
  }
  
  Serial.println("Data read successfully.");

  // ===================================================
  // 4. Format Data and Send HTTP POST Request
  // ===================================================
  
  // --- Convert Block 1 (Name) ---
  String rfidName = "";
  for (byte i = 0; i < dataSize; i++) {
      // Assuming Block 1 is ASCII text. Stop at null terminator.
      if (readBuffer1[i] == 0) break;
      if (readBuffer1[i] >= 32 && readBuffer1[i] <= 126) {
          rfidName += (char)readBuffer1[i];
      }
  }
  rfidName.trim(); 

  // --- FIX START: Convert Block 2 (ID) as ASCII Text ---
  String rfidDecimalId = "";
  for (byte i = 0; i < dataSize; i++) {
      // Stop reading if we hit a null byte (0x00)
      if (readBuffer2[i] == 0) break;

      // Only add valid printable characters (e.g., '0'-'9' are 48-57)
      if (readBuffer2[i] >= 32 && readBuffer2[i] <= 126) {
          // Cast the byte to (char) to treat 50 as '2', 51 as '3', etc.
          rfidDecimalId += (char)readBuffer2[i];
      }
  }
  rfidDecimalId.trim(); // Remove any extra spaces
  // --- FIX END ---
  
  // Construct JSON body
  String jsonPayload = "{\"name\": \"";
  jsonPayload += rfidName;
  jsonPayload += "\", \"id\": \"";
  jsonPayload += rfidDecimalId; 
  jsonPayload += "\"}";
  
  Serial.print("JSON Payload: ");
  Serial.println(jsonPayload);

  // Initialize HTTP client
  HTTPClient http;
  WiFiClient client;
  
  http.begin(client, serverUrl); 
  http.setTimeout(15000); 
  http.addHeader("Content-Type", "application/json");

  Serial.print("Sending POST request to: ");
  Serial.println(serverUrl);

  // Send the request
  int httpResponseCode = http.POST(jsonPayload);

  // === 5. Check Status Code and Control Pins ===
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.print("Server Response: ");
    Serial.println(response);

    // Control pins based on status code
    if (httpResponseCode == 201 || httpResponseCode == 200) {
        digitalWrite(FAILURE_PIN, LOW); 
        digitalWrite(SUCCESS_PIN, HIGH); // D4 HIGH for Success
        Serial.print("-> Success. Setting D4 HIGH.");
    } else {
        digitalWrite(SUCCESS_PIN, LOW); 
        digitalWrite(FAILURE_PIN, HIGH); // D3 HIGH for Other Status
        Serial.print("-> Non-success response. Setting D3 HIGH.");
    }
  } else {
    Serial.print("HTTP Request failed, error: ");
    Serial.println(httpResponseCode);
    
    // Indicate general failure
    digitalWrite(SUCCESS_PIN, LOW);
    digitalWrite(FAILURE_PIN, HIGH); 
    Serial.println("-> Request failed. Setting D3 HIGH.");
  }

  // Keep LED on briefly
  delay(1000); 
  
  // Turn off LEDs
  digitalWrite(SUCCESS_PIN, LOW);
  digitalWrite(FAILURE_PIN, LOW);

  // Free resources
  http.end();
  mfrc522.PCD_StopCrypto1();
}
