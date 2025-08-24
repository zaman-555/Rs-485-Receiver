#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MY_ID DEVICE_B
#define TARGET_ID DEVICE_A

uint32_t lastPingTime = 0;
uint8_t receivedCount = 0;

// Function declarations
void setTransmitMode();
void setReceiveMode();
uint8_t calculateChecksum(Packet packet);
bool sendPacket(uint8_t destination, uint8_t command, uint8_t data);
void receiveData();
void processPacket(Packet packet);
void updateDisplay(String status, String line1, String line2);

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  pinMode(DE, OUTPUT);
  pinMode(RE, OUTPUT);
  setReceiveMode();
  
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  updateDisplay("BOOT", "Device B Ready", "");
  Serial.println("Device B - Car Receiver Ready");
}

void setTransmitMode() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delayMicroseconds(50);
}

void setReceiveMode() {
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delayMicroseconds(50);
}

uint8_t calculateChecksum(Packet packet) {
  return packet.destination ^ packet.source ^ packet.command ^ packet.data;
}

bool sendPacket(uint8_t destination, uint8_t command, uint8_t data) {
  Packet packet;
  packet.destination = destination;
  packet.source = MY_ID;
  packet.command = command;
  packet.data = data;
  packet.checksum = calculateChecksum(packet);
  
  setTransmitMode();
  size_t bytesSent = Serial2.write((uint8_t*)&packet, sizeof(Packet));
  Serial2.flush();
  setReceiveMode();
  
  return bytesSent == sizeof(Packet);
}

void receiveData() {
  if (Serial2.available() >= sizeof(Packet)) {
    Packet packet;
    Serial2.readBytes((uint8_t*)&packet, sizeof(Packet));
    
    if (packet.header != 0xAA) return;
    if (packet.destination != MY_ID) return;
    if (packet.checksum != calculateChecksum(packet)) return;
    
    processPacket(packet);
  }
}

void processPacket(Packet packet) {
  switch(packet.command) {
    case CMD_PING:
      updateDisplay("PING", "From Device A", String(packet.data, HEX));
      sendPacket(packet.source, CMD_ACK, packet.data);
      break;
      
    case CMD_DATA:
      receivedCount++;
      updateDisplay("DATA", "Value: " + String(packet.data), "Count: " + String(receivedCount));
      sendPacket(packet.source, CMD_ACK, packet.data);
      break;
      
    case CMD_ACK:
      updateDisplay("ACK", "Received", String(packet.data));
      break;
      
    case CMD_ERROR:
      updateDisplay("ERROR", "From Device", String(packet.data, HEX));
      break;
  }
}

void updateDisplay(String status, String line1, String line2) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("DEVICE B - CAR");
  display.println("Status: " + status);
  display.println(line1);
  display.println(line2);
  display.display();
}

void loop() {
  receiveData();
  
  // Send ping every 5 seconds
  if (millis() - lastPingTime > 5000) {
    sendPacket(TARGET_ID, CMD_PING, 0x55);
    lastPingTime = millis();
  }
  
  delay(10);
}