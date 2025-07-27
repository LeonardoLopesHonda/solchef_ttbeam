#ifndef LORASERVICE_HPP
#define LORASERVICE_HPP

#include <Arduino.h>
#include <LoRa.h>
#include "DadosSolchef.hpp"

class LoraService {
    public:
        LoraService(long frequency, int ss, int rst, int dio0);
        bool Start();
        void EnviaDados(const DadosSolchef& mensagem);
        bool ReceberDados(String& payload);

    private:
        long frequency;
        int ss, rst, dio0;
};

#endif