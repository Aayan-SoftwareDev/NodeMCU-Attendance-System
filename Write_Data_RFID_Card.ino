#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN D1
#define SS_PIN D2

MFRC522 mfrc522(SS_PIN, RST_PIN);

byte testKeys[][6] = {
  {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
  {0xA0,0xA1,0xA2,0xA3,0xA4,0xA5},
  {0xD3,0xF7,0xD3,0xF7,0xD3,0xF7},
  {0x00,0x00,0x00,0x00,0x00,0x00}
};

int totalKeys = 4;

bool tryAuth(byte block, MFRC522::MIFARE_Key &key) {
  return (mfrc522.PCD_Authenticate(
            MFRC522::PICC_CMD_MF_AUTH_KEY_A,
            block, 
            &key, 
            &(mfrc522.uid)
          ) == MFRC522::STATUS_OK);
}

void writeBlock(byte block, const char *text) {
  byte buffer[16];
  memset(buffer, 0, 16);
  strncpy((char *)buffer, text, 16);
  mfrc522.MIFARE_Write(block, buffer, 16);
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Tap card...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.println("Card detected.");

  MFRC522::MIFARE_Key key;
  bool authenticated = false;

  for (int k = 0; k < totalKeys; k++) {
    memcpy(key.keyByte, testKeys[k], 6);

    if (tryAuth(1, key)) {
      Serial.print("Authenticated with key index ");
      Serial.println(k);

      writeBlock(1, "Name"); // replace with the name you want
      Serial.println("Block 1 written.");

      // re-authenticate for next block (recommended)
      tryAuth(2, key);
      writeBlock(2, "ID"); // replace it with student id
      Serial.println("Block 2 written.");

      authenticated = true;
      break;
    }
  }

  if (!authenticated) {
    Serial.println("Authentication failed for all test keys.");
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
