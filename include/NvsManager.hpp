#ifndef NVS_MANAGER_HPP
#define NVS_MANAGER_HPP

#include <Arduino.h>
#include <Preferences.h>
#include "nvs_flash.h"

class NvsManager {
public:
    NvsManager();  
    bool LerNvsWifi(String &ssid, String &key);    
    bool GravarNvsWifi(const String &ssid, const String &key); 
    void LimparNamespace();  

private:
    void inicializarNVS();     // Inicializa NVS e limpa se necessário
    bool testarNamespace();    // Verifica se o namespace está acessível
    const char* NAMESPACE_WIFI = "wifi";
};

#endif

