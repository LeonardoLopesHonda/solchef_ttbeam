#include "WifiManager.hpp"
#include <Preferences.h> // Para salvar credenciais se necessário

WifiManager::WifiManager() : _ssid(""), _key("") {}

// Tentar conectar no WiFi Station Mode com timeout
bool WifiManager::connect(const char* ssid, const char* key, unsigned long timeoutMs) {
  _ssid = ssid ? ssid : "";
  _key = key ? key : "";

  Serial.print("Conectando ao WiFi: ");
  Serial.print(_ssid.c_str());

  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid.c_str(), _key.c_str());

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeoutMs) {
    delay(500);
    Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectando ao WiFi!");
    Serial.print("\nIP Address: "); Serial.println(WiFi.localIP());
    Serial.print("\nSubnet Mask: "); Serial.println(WiFi.subnetMask());
    Serial.print("\nGateway IP: "); Serial.println(WiFi.gatewayIP());
    Serial.print("\nMAC Address: "); Serial.println(WiFi.macAddress());
    return true;
  } else {
    Serial.println("\nFalha na conexão WiFi (timeout).");
    return false;
  }
}

// Verifica se ainda está conectado
bool WifiManager::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

// Retorna o IP em formato std::string
std::string WifiManager::getIPAddress() {
  if(WiFi.status() == WL_CONNECTED) {
    return std::string(WiFi.localIP().toString().c_str());
  }
  return "0.0.0.0";
}

// Retorno MAC Address
std::string WifiManager::getMacAddress() {
  return std::string(WiFi.macAddress().c_str());
}

// Ativa o Access Point Mode para configuração
void WifiManager::startAccessPoint(const char* apName, const char* apPassword) {
  Serial.println("Iniciando Access Point Mode para configuração...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apName, apPassword);

  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP iniciado. Acesse o IP: ");
  Serial.println(apIP);
}