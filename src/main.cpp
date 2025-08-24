//Receiver Code
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// RS485 Pins
#define DE 4
#define RE 2
#define RXD2 32  // GPIO32 for RX
#define TXD2 33  // GPIO33 for TX

// OLED Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int receivedCount = 0;
String lastData = "";

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Initialize RS485 pins
  pinMode(DE, OUTPUT);
  pinMode(RE, OUTPUT);
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // Initialize OLED
  Wire.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.display();

  Serial.println("RS485 Receiver with OLED Ready");
}

void updateDisplay(String status, String data) {
  display.clearDisplay();
  display.setCursor(0, 0);
  
  display.println("RS485 RECEIVER");
  display.println("-------------");
  display.print("Status: ");
  display.println(status);
  display.print("Data: ");
  display.println(data);
  display.print("Received: ");
  display.println(receivedCount);
  display.print("Last: ");
  display.println(lastData);
  
  display.display();
}

void loop() {
  if (Serial2.available()) {
    int receivedData = Serial2.read();
    char receivedChar = (char)receivedData;
    
    // Update OLED
    lastData = String(receivedChar) + " [0x" + String(receivedData, HEX) + "]";
    updateDisplay("RECEIVED", lastData);

    // Serial output
    Serial.print("Received: ");
    Serial.print(receivedChar);
    Serial.print(" (0x");
    Serial.print(receivedData, HEX);
    Serial.println(")");
    
    receivedCount++;
  } else {
    updateDisplay("WAITING", "No data");
  }
  
  delay(100);
}