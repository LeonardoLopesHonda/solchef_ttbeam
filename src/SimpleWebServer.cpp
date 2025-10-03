#include "SimpleWebServer.hpp"

SimpleWebServer::SimpleWebServer() : server(80) {
  Serial.println("Construtor do servidor web chamado...");

  myIp = WiFi.localIP().toString();
  Serial.println("IP do dispositivo: " + myIp);
  
  macAddressReceiver = WiFi.macAddress();
  Serial.println("MAC Address do dispositivo: " + macAddressReceiver);
}

void SimpleWebServer::SetupRoutes() {
  // GET simples
  server.on("/solchef", HTTP_GET, [this]() {
    String mode = (WiFi.getMode() ==  WIFI_AP) ? "AP" : "Station";
    String json = "{";
    json += "\"status\":\"ok\",";
    json += "\"mode\":\"" + mode + "\",";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"mac\":\"" + WiFi.macAddress() + "\",";
    json += "}";

  String html = R"rawliteral(
    <!DOCTYPE html>
    <html lang="en">
    <head>
      <meta charset="UTF-8">
      <title>SolChef Dashboard</title>
      <link rel="stylesheet" href="https://unpkg.com/@picocss/pico@latest/css/pico.min.css">
    </head>
    <body>
      <main class="container">
        <h1>🌞 SolChef Dashboard</h1>

        <section>
          <h2>Device Info</h2>
          <ul>
            <li>Mode: <strong id="mode">...</strong></li>
            <li>IP: <strong id="ip">...</strong></li>
            <li>MAC: <strong id="mac">...</strong></li>
          </ul>
        </section>

        <section>
          <h2>Sensor Data</h2>
          <pre id="data">Loading...</pre>
        </section>
      </main>

      <script>
        async function loadInfo() {
          let res = await fetch('/solchef');
          let json = await res.json();
          document.getElementById('mode').textContent = json.mode;
          document.getElementById('ip').textContent = json.ip;
          document.getElementById('mac').textContent = json.mac;
        }
        async function loadData() {
          let res = await fetch('/dadossolchef');
          let json = await res.json();
          document.getElementById('data').textContent = JSON.stringify(json, null, 2);
        }
        loadInfo();
        loadData();
        setInterval(loadData, 2000);
      </script>
    </body>
    </html>
  )rawliteral";

    server.send(200, "text/html", html);
  });

  server.on("/config", HTTP_POST, [this](){
    Serial.println("POST /config recebido");
    String ssid;
    String password;

    //Primeiro tenta form-urlencoded
    if (server.hasArg("ssid") && server.hasArg("password")) {
      ssid = server.arg("ssid");
      password = server.arg("password");
    } 
    //Se não veio form, tenta JSON
    else if (server.hasArg("plain")) {
      String body = server.arg("plain");
      Serial.println("Body JSON recebido:");
      Serial.println(body);

      // Buffer para parse
      StaticJsonDocument<256> doc;
      DeserializationError err = deserializeJson(doc, body);

      if (err) {
        Serial.print("Erro ao parsear JSON: ");
        Serial.println(err.c_str());
      } else {
        ssid = doc["ssid"] | "";
        password = doc["password"] | "";
      }
    } else {
      Serial.println("Nenhum argumento recebido!");
    }
    
    if (ssid.length() > 0 && password.length() > 0) {
      Serial.println("Credenciais recebidas:");
      Serial.println("SSID: " + ssid);
      Serial.println("Password: " + password);

      prefs.begin("wifi", false);
      prefs.putString("ssid", ssid);
      prefs.putString("password", password);
      prefs.end();

      server.send(200, "application/json", "{\"status\":\"ok\",\"msg\":\"Credenciais salvas, reiniciando...\"}");
      delay(1500);
      ESP.restart();
    } else {
      server.send(400, "application/json",
      "{\"status\":\"error\",\"msg\":\"Parâmetros ssid e password são obrigatórios\"}");
    }});

  server.on("/dadossolchef", HTTP_GET, [this]() {
    String json = dadosRecebidos.ToJSON();
    server.send(200, "application/json", json);
    Serial.println("GET /dadosSolChef -> enviado pacote JSON.");
  });

  server.on("/getmacaddresssender", HTTP_GET, [this]() {
    server.send(200, "text/plain", dadosRecebidos.macAddressSender);
    Serial.println("MAC Address SolChef: " + dadosRecebidos.macAddressSender);
  });

  server.on("/getmacaddressreceiver", HTTP_GET, [this]() {
    server.send(200, "text/plain", dadosRecebidos.macAddressReceiver);
    Serial.println("MAC Address SolChef: " + macAddressReceiver);
  });

  server.begin();
  Serial.println("Servidor HTTP inciado na porta 80.");
}

void SimpleWebServer::SendData(const DadosSolchef& DadosSolchef) {
  dadosRecebidos = DadosSolchef;
  Serial.println("Dados recebidos e enviados para webserver: ");
  Serial.println(dadosRecebidos.ToJSON());
}

void SimpleWebServer::HandleClient() {
  server.handleClient();
}