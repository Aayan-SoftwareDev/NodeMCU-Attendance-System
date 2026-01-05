# Circuit Documentation

## Summary
This document provides a detailed overview of a circuit design that integrates a **NodeMCU microcontroller**, an **RFID-RC522 module**, a **Buzzer**, and an **RGB Module**.  
The circuit interfaces with an RFID reader and provides **visual feedback** via the RGB Module and **auditory feedback** via the Buzzer. The NodeMCU acts as the central controller, managing communication between all components.

---

## Components

### NodeMCU
- **Description:** A low-cost, open-source IoT platform based on the ESP8266 WiFi module.
- **Pins:**  
  `A0, RSV, SD3, SD2, SD1, CMD, SD0, CLK, GND, 3V3, EN, RST, Vin, D0, D1, D2, D3, D4, D5, D6, D7, D8, RX, TX`

---

### RFID-RC522
- **Description:** A low-cost RFID reader module operating at **13.56 MHz**.
- **Pins:**  
  `VCC (3.3V), RST, GND, IRQ, MISO, MOSI, SCK, SDA`

---

### Buzzer
- **Description:** An electronic component that emits sound when powered.
- **Pins:**  
  `VCC, GND`

---

### RGB Module
- **Description:** A module containing red, green, and blue LEDs, allowing multiple color combinations.
- **Pins:**  
  `R, G, B, GND`

---

## Wiring Details

### NodeMCU Connections
| NodeMCU Pin | Connected To |
|------------|--------------|
| 3V3 | VCC (3.3V) of RFID-RC522 |
| D1 | RST of RFID-RC522 |
| GND | GND of RFID-RC522, Buzzer, RGB Module |
| D6 | MISO of RFID-RC522 |
| D7 | MOSI of RFID-RC522 |
| D5 | SCK of RFID-RC522 |
| D2 | SDA of RFID-RC522 |
| D4 | G pin of RGB Module, VCC of Buzzer |
| D3 | R pin of RGB Module |

---

### RFID-RC522 Connections
| RFID-RC522 Pin | Connected To |
|---------------|--------------|
| VCC (3.3V) | 3V3 of NodeMCU |
| RST | D1 of NodeMCU |
| GND | GND of NodeMCU |
| MISO | D6 of NodeMCU |
| MOSI | D7 of NodeMCU |
| SCK | D5 of NodeMCU |
| SDA | D2 of NodeMCU |

---

### Buzzer Connections
| Buzzer Pin | Connected To |
|-----------|--------------|
| VCC | D4 of NodeMCU |
| GND | GND of NodeMCU |

---

### RGB Module Connections
| RGB Pin | Connected To |
|--------|--------------|
| G | D4 of NodeMCU |
| R | D3 of NodeMCU |
| GND | GND of NodeMCU |

---

## Notes
- Ensure the RFID-RC522 module is powered **only with 3.3V** to avoid damage.
- Shared pins (D4) are used for synchronized visual and auditory feedback.
- All grounds must be connected together for stable operation.
