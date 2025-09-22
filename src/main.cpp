#include <Arduino.h>
#include "Display.hpp"
#include "WifiManager.hpp"

// Objetos globais
Display oled(17, 18, 21, 128, 64); // pino SDA, pino SCL, OLED_RESET, largura, altura
WifiManager wifiManager;
bool apMode = false;

int i = 0;

void setup() {
  Serial.begin(115200);

  String savedSSID, savedPass;
  bool connected = false;

  connected = wifiManager.connect(savedSSID.c_str(), savedPass.c_str(), 10000);

  oled.Start(); // Inicia o display OLED

  oled.PrintLine(1, ("Connecting..."));
  delay(2500);

  if(!connected) {
    Serial.println("Falha ao conectar -> iniciando AP");
    wifiManager.startAccessPoint("SolChef_Config", "solchef12345");
    apMode = true;
    oled.PrintLine(1, ("Initializing AP Mode..."));
    delay(2000);
  } else {
    Serial.println("Conectado em Station Mode!");
    oled.PrintLine(1, ("Connecting in Station Mode..."));
    delay(1000);
  }

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
