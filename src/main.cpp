/*
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
  Serial.println("Iniciando SolChef...");
  delay(500);

  Serial.println("Limpando apenas o namespace 'wifi'...");

  if (prefs.begin("wifi", false)) {
    prefs.clear();  // apaga apenas as chaves dentro de "wifi"
    prefs.end();
    Serial.println("Namespace 'wifi' limpo com sucesso!");
  } else {
    Serial.println("Falha ao abrir namespace 'wifi'. Pode estar corrompido.");
  }

  Serial.println("Antes do prefs.begin...");

  // Usar namespace exclusivo para não conflitar com driver Wi-Fi interno
  if (!prefs.begin("wifi", true)) {
    Serial.println("Falha ao abrir namespace solchef_wifi!");
  }

  Serial.println("Acessando prefs.begin!");

  // Lê credenciais com fallback
  String savedSSID = prefs.getString("ssid", "");
  String savedPass = prefs.getString("password", "");
  prefs.end();  // fecha imediatamente para liberar NVS

  Serial.println("Credenciais lidas do NVS!");
  Serial.println("SSID salvo: " + savedSSID);
  Serial.println("Senha salva: " + savedPass);

  bool conectado = false;

  // Verifica se há SSID válido antes de tentar conectar
  if (savedSSID.length() > 1) {
    Serial.println("Credenciais válidas encontradas. Tentando conectar...");
    conectado = wifi.connect(savedSSID.c_str(), savedPass.c_str(), 10000);

    if (!conectado) {
      Serial.println("Falha na conexão Wi-Fi → iniciando AP");
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
    webServer.HandleClient(); // processa requisições em qualquer modo
    webServer.SendData(dados); // Envia dados para o servidor web
    delay(10000); // Aguarda 1 segundo antes de enviar novamente
}
*/

/*
#include <Arduino.h>
#include <Preferences.h>

Preferences prefs;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Testando leitura NVS...");
  prefs.begin("wifi", true);

  String ssid = prefs.getString("ssid", "VAZIO");
  String pass = prefs.getString("password", "VAZIO");

  prefs.end();

  Serial.println("SSID lido: " + ssid);
  Serial.println("Senha lida: " + pass);
}

void loop() {}*/

#include <Arduino.h>
#include "WifiManager.hpp"
#include "SimpleWebServer.hpp"
#include "DadosSolchef.hpp"
#include "SensorTemperatura.hpp"
#include "NvsManager.hpp"

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
  dados.macAddressSender = wifi.getMeuMacAddress().c_str();;
  // Exemplo: envia dados periodicamente
  webServer.SendData(dados);

  delay(2000); // Aguarda 10 segundos antes de enviar novamente
}


