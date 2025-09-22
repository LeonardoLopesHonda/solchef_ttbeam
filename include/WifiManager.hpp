#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <string>

class WifiManager {
public:
  WifiManager();

  /**
   * Conecta ao WiFi no modo Station.
   * @param ssid Nome da rede WiFi.
   * @param key  Senha da rede WiFi.
   * @param timeoutMs Tempo limite para tentar conectar (default 10s).
   * @return true se conectado com sucesso, false se falhou.
   */
  bool connect(const char* ssid, const char* key, unsigned long = 10000);

  /**
   * Verifica se ainda está conectando ao WiFi
   * @return true se conectado, false caso contrário.
   */
  bool isConnected();

  /**
   * Retorna o endereço IP local em formato string
   * @return IpAddress:String
   */
  std::string getIPAddress();
  
  /**
   * Retorna o endereço MAC do dispositivo em formato string
   * @return IpAddress:String
   */
  std::string getMacAddress();

  /**
   * Ativa o modo AP (Access Point) para configuração.
   * 
   * @param apName Nome do Access Point.
   * @param apPassword Senha do Access Point.
   */
  void startAccessPoint(const char* apName = "SolChef_Config", const char* apPassword = "solchef12345");
  
private:
  std::string _ssid;
  std::string _key;
};