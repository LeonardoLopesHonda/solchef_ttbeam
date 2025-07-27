#include <Arduino.h>
#include "WifiManager.hpp"
#include "SimpleWebServer.hpp"
#include "DadosSolchef.hpp"
#include "SensorTemperatura.hpp"
#include "NvsManager.hpp"
#include "Gps.hpp"

// Objetos globais
WifiManager wifi;
SimpleWebServer webServer;
DadosSolchef dados;
SensorTemperatura sensorTemperatura(4);
NvsManager nvsManager;   // ao criar já inicializa a NVS e corrige se necessário

bool apModeAtivo = false;

void setup() {
  Serial.begin(115200);
  Serial.println("\nIniciando SolChef...");
  delay(500);

  String savedSSID, savedPass;
  bool conectado = false;

  if (nvsManager.LerNvsWifi(savedSSID, savedPass)) {
    Serial.println("Credenciais encontradas. Tentando conectar...");
    conectado = wifi.connect(savedSSID.c_str(), savedPass.c_str(), 10000);

    if (!conectado) {
      Serial.println("Falha ao conectar → iniciando AP");
      wifi.startAccessPoint("SolChef_Config", "12345678");
      apModeAtivo = true;
    } else {
      Serial.println("Conectado em Station Mode!");
    }
  } else {
    Serial.println("Nenhuma credencial salva. Iniciando AP...");
    wifi.startAccessPoint("SolChef_Config", "12345678");
    apModeAtivo = true;
  }

  // Sempre inicia o servidor web, independente do modo
  webServer.SetupRoutes();
}

void loop() {
  // Processa requisições em qualquer modo
  webServer.HandleClient();
  dados.tempAgua = random(20, 30) + random(0, 100) / 100.0; // Simula temperatura da água
  dados.tempInterna = random(20, 30) + random(0, 100) / 100.0; // Simula temperatura interna
  dados.latitude = 37.7749; // Simula latitude
  dados.longitude = -122.4194; // Simula longitude
  dados.macAddressSender = wifi.getMeuMacAddress().c_str();;
  webServer.SendData(dados);

  delay(2000);
}


