# RFID Attendance System

## Overview

This project is an **RFID-based Attendance System** built using a **NodeMCU (ESP8266)**, **RFID-RC522**, **RGB Module**, and **Buzzer**, with a **Node.js backend**.
The NodeMCU reads RFID cards and communicates with the backend server using HTTP requests. Visual and audio feedback is provided based on backend responses.

---

## System Architecture

* **Hardware:** NodeMCU, RFID-RC522, RGB Module, Buzzer
* **Firmware:** NodeMCU (ESP8266) code for RFID handling and HTTP communication
* **Backend:** Node.js REST API for attendance and admin authentication

---

## Circuit Documentation

### Summary

The circuit integrates a NodeMCU microcontroller with an RFID-RC522 module for card scanning, an RGB Module for visual feedback, and a Buzzer for audio alerts. The NodeMCU acts as the central controller.

---

### Components

#### NodeMCU

* **Description:** Open-source IoT platform based on the ESP8266 WiFi module
* **Pins:**
  `A0, RSV, SD3, SD2, SD1, CMD, SD0, CLK, GND, 3V3, EN, RST, Vin, D0, D1, D2, D3, D4, D5, D6, D7, D8, RX, TX`

#### RFID-RC522

* **Description:** RFID reader module operating at **13.56 MHz**
* **Pins:**
  `VCC (3.3V), RST, GND, IRQ, MISO, MOSI, SCK, SDA`

#### Buzzer

* **Description:** Emits sound when powered
* **Pins:**
  `VCC, GND`

#### RGB Module

* **Description:** RGB LED module for multi-color indication
* **Pins:**
  `R, G, B, GND`

---

### Wiring Details

#### NodeMCU Connections

| NodeMCU Pin | Connected To                          |
| ----------- | ------------------------------------- |
| 3V3         | VCC (3.3V) of RFID-RC522              |
| D1          | RST of RFID-RC522                     |
| GND         | GND of RFID-RC522, Buzzer, RGB Module |
| D6          | MISO of RFID-RC522                    |
| D7          | MOSI of RFID-RC522                    |
| D5          | SCK of RFID-RC522                     |
| D2          | SDA of RFID-RC522                     |
| D4          | G pin of RGB Module, VCC of Buzzer    |
| D3          | R pin of RGB Module                   |

#### RFID-RC522 Connections

| RFID-RC522 Pin | Connected To   |
| -------------- | -------------- |
| VCC (3.3V)     | 3V3 of NodeMCU |
| RST            | D1 of NodeMCU  |
| GND            | GND of NodeMCU |
| MISO           | D6 of NodeMCU  |
| MOSI           | D7 of NodeMCU  |
| SCK            | D5 of NodeMCU  |
| SDA            | D2 of NodeMCU  |

#### Buzzer Connections

| Buzzer Pin | Connected To   |
| ---------- | -------------- |
| VCC        | D4 of NodeMCU  |
| GND        | GND of NodeMCU |

#### RGB Module Connections

| RGB Pin | Connected To   |
| ------- | -------------- |
| G       | D4 of NodeMCU  |
| R       | D3 of NodeMCU  |
| GND     | GND of NodeMCU |

---

### Hardware Notes

* RFID-RC522 must be powered with **3.3V only**
* All GND pins must be common
* RGB Module and Buzzer share control logic for feedback

---

## Backend API Documentation

### Base URL

`http://<server-ip>:<port>`

---

### Make Attendance Entry

**Endpoint**
`POST /entry`

**Description**
Used by the NodeMCU to mark attendance after an RFID scan.

**Request Body**

```json
{
  "name": "string",
  "id": "string"
}
```

---

### Admin Sign In

**Endpoint**
`POST /user/signin`

**Description**
Authenticates an admin user for the admin panel.

**Request Body**

```json
{
  "username": "string",
  "password": "string"
}
```
