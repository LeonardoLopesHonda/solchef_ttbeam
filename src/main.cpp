#include <Arduino.h>
#include <Preferences.h>
#include "WifiManager.hpp"
#include "SimpleWebServer.hpp"
#include "DadosSolchef.hpp"
#include "SensorTemperatura.hpp"

WifiManager wifi;
SimpleWebServer webServer;
Preferences prefs;
DadosSolchef dados;
SensorTemperatura sensorTemperatura(4);

bool apModeAtivo = false;

void setup() {
  Serial.begin(115200);

  //Le credenciais do NVS
  prefs.begin("wifi", true); // modo somente leitura
  String savedSSID = prefs.getString("ssid", "");
  String savedPass = prefs.getString("password", "");
  prefs.end();

  bool conectado = false;

  if (savedSSID != "") {
    Serial.println("Credenciais encontradas na flash:");
    Serial.println("SSID: " + savedSSID);
    conectado = wifi.connect(savedSSID.c_str(), savedPass.c_str(), 10000);
  } else {
    Serial.println("Nenhuma credencial salva. Pulando tentativa de conexao.");
  }

  if (!conectado) {
    wifi.startAccessPoint("SolChef_Config", "12345678");
    Serial.println("Modo AP para configuração");
} else {
    Serial.println("Conectado em Station Mode!");
}

// Sempre inicia o servidor web, independente do modo
  webServer.SetupRoutes();
}

void loop() {
    webServer.HandleClient(); // processa requisições em qualquer modo
    dados.horaRegistro = String(millis() / 1000);
    dados.macAddressSender = wifi.getMeuMacAddress().c_str();
    dados.latitude = 0.0; // Exemplo, deve ser atualizado com dados reais
    dados.longitude = 0.0; // Exemplo, deve ser atualizado com dados reais
    dados.tempAgua = 25.0 + 1; // Exemplo, deve ser atualizado com dados reais
    dados.tempInterna = 22.0 + 1; // Exemplo, deve ser atualizado com dados reais

    webServer.SendData(dados); // Envia dados para o servidor web

    delay(1000); // Aguarda 1 segundo antes de enviar novamente
}