#include <SensorTemperatura.hpp>

SensorTemperatura::SensorTemperatura(int pin)
    : pin(pin), oneWire(pin), sensor(&oneWire)
{
    sensor.begin();
    temperaturaAgua = 0.0;
    temperaturaInterna = 0.0;
}

float SensorTemperatura::GetTemperaturaAgua() {
    sensor.requestTemperatures();
    temperaturaAgua = sensor.getTempCByIndex(0);
    Serial.println("Temperature: " + String(temperaturaAgua) + " °C");
    return temperaturaAgua;
}

float SensorTemperatura::GetTemperaturaInterna() {
    sensor.requestTemperatures();
    temperaturaInterna = sensor.getTempCByIndex(1);
    Serial.println("Temperature: " + String(temperaturaInterna) + " °C");
    return temperaturaInterna;
}