#include <Arduino.h>
#include "Display.hpp"

// Objetos globais
Display oled(17, 18, 21, 128, 64); // pino SDA, pino SCL, OLED_RESET, largura, altura

int i = 0;

void setup() {
  Serial.begin(115200);

  oled.Start(); // Inicia o display OLED

  oled.Clear();
  oled.PrintLine(1, ("SolChef | LoRa"));
  oled.PrintLine(2, ("Receptor - Monitor"));

  delay(3000);
}

void loop() {
  oled.Clear();
  oled.PrintLine(0, ("IP: 192.168.---.---"));
  oled.PrintLine(1, ("Ag: " + String(i) + " Cº").c_str());
  oled.PrintLine(2, ("Date: dd/MM/yyyy"));
  oled.PrintLine(3, ("GPS: xx.xx | xx.xx"));

  i++;
  delay(1000);
}
