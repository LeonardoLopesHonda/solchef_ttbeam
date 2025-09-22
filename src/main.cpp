#include <Arduino.h>
#include "Display.hpp"
#include "WifiManager.hpp"
#include "NvsManager.hpp"
#include "DadosSolchef.hpp"

// Objetos globais
Display oled(17, 18, 21, 128, 64); // pino SDA, pino SCL, OLED_RESET, largura, altura
WifiManager wifiManager;
bool apMode = false;
NvsManager nvsManager;

int i = 0;

void setup() {
  Serial.begin(115200);

  String savedSSID, savedPass;
  bool connected = false;

  if(nvsManager.LerNvsWifi(savedSSID, savedPass)) {
    Serial.println("Credenciais encontradas. Tentando conectar...");
    connected = wifiManager.connect(savedSSID.c_str(), savedPass.c_str(), 10000);
    
    if(!connected) {
      Serial.println("Falha ao conectar -> iniciando AP");
      wifiManager.startAccessPoint("SolChef_Config", "solchef12345");
      apMode = true;
    } else {
      Serial.println("Conectado em Station Mode!");
    }
  } else {
    Serial.println("Nenhuma credencial salva. Iniciando AP...");
    wifiManager.startAccessPoint("SolChef_Config", "solchef12345");
    apMode = true;
  }
  
  oled.Start(); // Inicia o display OLED
  oled.Clear();
  oled.PrintLine(1, ("SolChef | LoRa"));
  oled.PrintLine(2, ("Receptor - Monitor"));

  delay(3000);
}

void loop() {
  DadosSolchef dados;

  dados.macAddressSender = wifiManager.getMacAddress().c_str();

  oled.Clear();
  oled.PrintLine(0, ("IP: " + std::string(wifiManager.getIPAddress())).c_str());
  oled.PrintLine(1, ("Ag: " + String(i) + " Cº").c_str());
  oled.PrintLine(2, ("Date: dd/MM/yyyy"));
  oled.PrintLine(3, ("GPS: xx.xx | xx.xx"));

  i++;
  delay(1000);
}
