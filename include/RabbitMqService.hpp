#ifndef RABBITMQSERVICE_HPP
#define RABBITMQSERVICE_HPP

#include <Arduino.h>
#include <HTTPClient.h>
#include "DadosSolchef.hpp"

class RabbitMqService {
public:
    RabbitMqService(const String& host, const String& user, const String& password);
    bool CriarFilaSeNecessario(const String& routingKey);
    bool PublishToQueue(const DadosSolchef& dados);

private:
    String host;       // Ex: "http://192.168.0.100:15672"
    String user;
    String password;
    String queueName;
    bool filaCriada = false;
};

#endif