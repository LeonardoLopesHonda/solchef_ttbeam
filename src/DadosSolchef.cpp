#include "DadosSolchef.hpp"

String DadosSolchef::ToJSON() const {
    StaticJsonDocument<256> doc;

    doc["tempAgua"]             = tempAgua;
    doc["tempInterna"]          = tempInterna;
    doc["latitude"]             = latitude;
    doc["longitude"]            = longitude;
    doc["horaRegistro"]         = horaRegistro;
    doc["macAddressSender"]     = macAddressSender;
    doc["macAddressReceiver"]   = macAddressReceiver;

    String output;
    serializeJson(doc, output);
    return output;
}
