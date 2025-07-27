#ifndef DADOSSOLCHEF_HPP
#define DADOSSOLCHEF_HPP

#include <Arduino.h>
#include <ArduinoJson.h>

class DadosSolchef {
    public:
        float tempAgua = 0;
        float tempInterna = 0;
        float latitude = 0;
        float longitude = 0;
        String horaRegistro;
        String macAddressSender;
        String macAddressReceiver;
        String ToJSON() const;
};

#endif