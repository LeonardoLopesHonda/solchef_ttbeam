#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "DadosSolchef.hpp"

class SimpleWebServer {
  public:
    SimpleWebServer();
    void SetupRoutes();
    void HandleClient();
    String getMyIp() const { return myIp; }
    String getMacAddressReceiver() const { return macAddressReceiver; }
    void SendData(const DadosSolchef &DadosSolchef);

  private:
    WebServer server;             // Servidor HTTP na porta 80
    String myIp;                  // IP atual do dispositivo
    String macAddressReceiver; // MAC Address do dispositivo
    Preferences prefs;            // Global para salvar configs
    DadosSolchef dadosRecebidos;
};