// config.h - Common configuration for both devices
#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// RS485 Pins
#define DE 4
#define RE 2
#define RXD2 32  // GPIO32 for RX
#define TXD2 33  // GPIO33 for TX

// OLED Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_SDA 21
#define OLED_SCL 22

// Device IDs
#define DEVICE_A 0x01
#define DEVICE_B 0x02

// Packet structure
struct Packet {
  uint8_t header = 0xAA;    // Start byte
  uint8_t destination;      // Target device ID
  uint8_t source;           // Source device ID
  uint8_t command;          // Command type
  uint8_t data;             // Data payload
  uint8_t checksum;         // XOR checksum
};

// Commands
enum Commands {
  CMD_PING = 0x01,
  CMD_DATA = 0x02,
  CMD_ACK  = 0x03,
  CMD_ERROR = 0x04
};

#endif