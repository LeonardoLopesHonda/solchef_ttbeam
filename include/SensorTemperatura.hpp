#ifndef SENSORTEMPERATURA_HPP
#define SENSORTEMPERATURA_HPP

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class SensorTemperatura
{
    public:
        SensorTemperatura(int pin);
        float GetTemperaturaAgua();
        float GetTemperaturaInterna();

    private:
        float temperaturaAgua;
        float temperaturaInterna;
        int pin;
        OneWire oneWire;
        DallasTemperature sensor;
};

#endif