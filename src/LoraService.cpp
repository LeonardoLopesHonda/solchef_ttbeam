#include "LoraService.hpp"

LoraService::LoraService(long frequency, int ss, int rst, int dio0)
    : frequency(frequency), ss(ss), rst(rst), dio0(dio0) {}

bool LoraService::Start() {
    LoRa.setPins(ss, rst, dio0);

    if (!LoRa.begin(frequency)) {
        Serial.println("Falha ao iniciar LoRa.");
        return false;
    }

    LoRa.setSPIFrequency(10E6);  // Aumenta desempenho da comunicaÃ§Ã£o SPI
    Serial.println("LoRa iniciado com sucesso.");
    return true;
}

void LoraService::EnviaDados(const DadosSolchef& mensagem) {
    String json = mensagem.ToJSON();

    if (json.length() == 0) {
        Serial.println("Erro: JSON vazio, pacote nÃ£o enviado.");
        return;
    }

    LoRa.beginPacket();
    LoRa.print(json);
    int result = LoRa.endPacket();

    if (result == 1) {
        Serial.print("Enviado (json): ");
        Serial.println(json);
        Serial.print("Tamanho JSON: ");
        Serial.println(json.length());
    } else {
        Serial.println("Erro ao finalizar e enviar o pacote LoRa.");
    }
}

bool LoraService::ReceberDados(String& payload) {
    int packetSize = LoRa.parsePacket();
    if (packetSize == 0) return false;

    Serial.print("Tamanho do pacote recebido: ");
    Serial.println(packetSize);

    payload = "";
    while (LoRa.available()) {
        payload += (char)LoRa.read();
    }

    Serial.print("Recebido: ");
    Serial.println(payload);
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());
    Serial.print("SNR: ");
    Serial.println(LoRa.packetSnr());

    return payload.length() > 0;
}