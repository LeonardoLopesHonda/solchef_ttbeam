#include "RabbitMqService.hpp"

RabbitMqService::RabbitMqService(const String& host, const String& user, const String& password)
    : host(host), user(user), password(password), filaCriada(false) {}

bool RabbitMqService::CriarFilaSeNecessario(const String& routingKey) {
    if (filaCriada) return true;

    Serial.printf("[RabbitMQ] Criando fila para routing key: %s\n", routingKey.c_str());

    const String vhost = "%2F"; // "/" codificado em URL

    // Sintetiza o nome da fila (substitui ":" por "_")
    queueName = routingKey;
    queueName.replace(":", ".");

    HTTPClient http;

    // 1. Criar fila (PUT)
    String queueUrl = host + "/api/queues/" + vhost + "/" + queueName;
    String queueBody = "{\"durable\":true}";

    http.begin(queueUrl);
    http.setAuthorization(user.c_str(), password.c_str());
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.PUT(queueBody);
    Serial.printf("[RabbitMQ] Código PUT: %d\n", httpCode);
    Serial.println("---------------------------------------");

    if (httpCode != 201 && httpCode != 204) {
        Serial.printf("[RabbitMQ] Erro ao criar fila '%s': %d\n", queueName.c_str(), httpCode);
        Serial.println(http.getString());
        http.end();
        return false;
    }
    http.end();

    // 2. Criar binding (POST)
    String bindingUrl = host + "/api/bindings/" + vhost + "/e/amq.direct/q/" + queueName;
    String bindBody = "{\"routing_key\":\"" + queueName + "\"}";

    http.begin(bindingUrl);
    http.setAuthorization(user.c_str(), password.c_str());
    http.addHeader("Content-Type", "application/json");

    httpCode = http.POST(bindBody);
    if (httpCode != 201 && httpCode != 204) {
        Serial.printf("[RabbitMQ] Erro ao criar binding: %d\n", httpCode);
        Serial.println(http.getString());
        http.end();
        return false;
    }

    http.end();
    filaCriada = true;
    Serial.println("[RabbitMQ] Fila e binding criados com sucesso.");
    return true;
}

bool RabbitMqService::PublishToQueue(const DadosSolchef& dados) {
    if (!filaCriada) {
        if (!CriarFilaSeNecessario(dados.macAddressSender)) {
            Serial.println("[RabbitMQ] Falha ao criar fila antes de publicar.");
            return false;
        }
    }

    HTTPClient http;
    String url = host + "/api/exchanges/%2F/amq.direct/publish";

    // Usa ArduinoJson para gerar o JSON corretamente com escape
    StaticJsonDocument<512> doc;
    doc.createNestedObject("properties");
    doc["routing_key"] = queueName;
    doc["payload"] = dados.ToJSON();  // string JSON como payload
    doc["payload_encoding"] = "string";

    String jsonBody;
    serializeJson(doc, jsonBody);

    Serial.println("[RabbitMQ] Enviando corpo:");
    Serial.println(jsonBody);

    http.begin(url);
    http.setAuthorization(user.c_str(), password.c_str());
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.POST(jsonBody);
    bool success = (httpCode == 200);

    Serial.print("[RabbitMQ] Código POST: ");
    Serial.println(httpCode);
    if (!success) {
        Serial.println("[RabbitMQ] Erro ao publicar na fila.");
        Serial.println(http.getString());
    }

    http.end();
    return success;
}

