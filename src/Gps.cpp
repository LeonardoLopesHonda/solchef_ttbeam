#include "Gps.hpp"
#include <HardwareSerial.h>

HardwareSerial SerialGPS(1); // UART1 para o GPS

Gps::Gps(int rxPin, int txPin, long baudRate)
    : rxPin(rxPin), txPin(txPin), baudRate(baudRate) {}

void Gps::IniciaGps() {
    SerialGPS.begin(baudRate, SERIAL_8N1, rxPin, txPin);
}

void Gps::SincronizaGps() {
    while (SerialGPS.available() > 0) {
        gps.encode(SerialGPS.read());
    }
}

bool Gps::TemFix() {
    return gps.location.isValid();
}

float Gps::GetLatitude() {
    return TemFix() ? gps.location.lat() : 0.0;
}

float Gps::GetLongitude() {
    return TemFix() ? gps.location.lng() : 0.0;
}

int Gps::GetNumeroSatelites() {
    return gps.satellites.isValid() ? gps.satellites.value() : 0;
}

String Gps::GetHora() {
    if (gps.time.isValid()) {
        int hora = gps.time.hour() - 4;
        if (hora < 0) {
            hora += 24;  // ajuste para o dia anterior
        }

        char buffer[9];
        snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",
                 hora, gps.time.minute(), gps.time.second());
        return String(buffer);
    } else {
        return "--:--:--";
    }
}


void Gps::ImprimirDadosGps() {
    Serial.println("=== Dados GPS ===");
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.print(gps.altitude.meters());
    Serial.println(" m");
    Serial.print("Satélites: ");
    Serial.println(gps.satellites.value());
    Serial.print("HDOP (precisão): ");
    Serial.println(gps.hdop.hdop(), 1);
    Serial.printf("Hora (UTC): %02d:%02d:%02d\n", gps.time.hour(), gps.time.minute(), gps.time.second());
    Serial.printf("Data: %02d/%02d/%04d\n", gps.date.day(), gps.date.month(), gps.date.year());
    Serial.print("Velocidade: ");
    Serial.print(gps.speed.kmph(), 2);
    Serial.println(" km/h");
    Serial.println("==================");
}