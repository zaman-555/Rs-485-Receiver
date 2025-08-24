# ESP32 RS485 Two-Way Communication System

![RS485 Communication](https://img.shields.io/badge/Protocol-RS485-blue)
![ESP32](https://img.shields.io/badge/Platform-ESP32-green)
![OLED Display](https://img.shields.io/badge/Display-SSD1306-yellow)

A robust bidirectional communication system between two ESP32 devices using RS485 protocol with real-time OLED status displays.

## 📋 Project Overview

This project implements a reliable two-way communication system between two ESP32 devices using MAX485 RS485 transceivers. The system features packet-based communication with error checking, acknowledgment system, and real-time status display on OLED screens.

## 🚀 Features

- **Bidirectional RS485 Communication** - Full duplex communication over long distances
- **Packet-Based Protocol** - Structured data packets with headers and checksums
- **Error Detection** - XOR checksum validation for data integrity
- **Acknowledgment System** - Automatic ACK responses for reliable delivery
- **Real-time OLED Display** - Live status updates on both devices
- **Timeout Handling** - Automatic recovery from communication failures
- **Device Addressing** - Support for multiple devices on the same bus

## 🛠 Hardware Requirements

### Components
- 2 × ESP32 Dev Modules
- 2 × MAX485 RS485 Transceiver Modules
- 2 × SSD1306 OLED Displays (128×64)
- Jumper wires and breadboard
- 120Ω termination resistors (for long cables)
- Power supply (3.3V/5V)

### Wiring Diagram
ESP32 Dev Module → MAX485 Module → OLED Display
─────────────────────────────────────────────
- GPIO32 (RX2) → RO (Receiver Out)
- GPIO33 (TX2) → DI (Driver In)
- GPIO4 → DE (Driver Enable)
- GPIO2 → RE (Receiver Enable)
- GPIO21 (SDA) → OLED SDA
- GPIO22 (SCL) → OLED SCL
- GND → GND (both MAX485 & OLED)
- 3.3V → VCC (both MAX485 & OLED)

```js
esp32-rs485-communication/
├── device-a/ # Car Controller Device
│ ├── src/
│ │ ├── main.cpp # Device A main code
│ │ └── config.h # Common configuration
│ └── platformio.ini # PlatformIO configuration
├── device-b/ # Car Receiver Device
│ ├── src/
│ │ ├── main.cpp # Device B main code
│ │ └── config.h # Common configuration
│ └── platformio.ini # PlatformIO configuration
└── README.md
```

## ⚙️ Configuration

### config.h - Common Settings
```cpp
// RS485 Pins
#define DE 4
#define RE 2
#define RXD2 32  // GPIO32 for RX
#define TXD2 33  // GPIO33 for TX

// Device IDs
#define DEVICE_A 0x01
#define DEVICE_B 0x02

// Packet structure with header, addressing, and checksum
struct Packet {
  uint8_t header = 0xAA;
  uint8_t destination;
  uint8_t source;
  uint8_t command;
  uint8_t data;
  uint8_t checksum;
};
```

### 🚦Communication Protocol

```txt
Command Types
Command	     Value	Description
CMD_PING	  0x01	Keep-alive heartbeat
CMD_DATA	  0x02	Data transmission
CMD_ACK	      0x03	Acknowledgment
CMD_ERROR	  0x04	Error notification
```