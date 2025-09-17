// #include <Wire.h>
// #include "SSD1306Wire.h"
// // #include <Adafruit_GFX.h>
// // #include <Adafruit_SSD1306.h>

// // put function declarations here:
// #define SDA 17
// #define SCL 18
// #define OLED_RST 21
// #define logo_width 64
// #define logo_height 64

// SSD1306Wire display(0x3c, SDA, SCL);

// int i = 0;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(115200);
//   pinMode(OLED_RST, OUTPUT);
//   digitalWrite(OLED_RST, LOW);
//   delay(20);
//   digitalWrite(OLED_RST, HIGH);

//   Wire.begin(SDA, SCL);
//   display.init();
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   display.clear();  
//   display.setFont(ArialMT_Plain_10); 

//   display.drawString(0, 0, "SolChef - Pasteurizar água");
//   display.drawString(0, 12, "192.168.-.-");
//   display.drawString(0, 24, "Temp: " + String(i) + " Cº");
//   display.drawString(0, 36, "LoRa Connection: OK");
//   display.display();

//   i++;
//   delay(1000);
// }

