#include "SimpleWebServer.hpp"

SimpleWebServer::SimpleWebServer() : server(80) {
  Serial.println("Construtor do servidor web chamado...");

  myIp = WiFi.localIP().toString();
  Serial.println("IP do dispositivo: " + myIp);
  
  macAddressReceiver = WiFi.macAddress();
  Serial.println("MAC Address do dispositivo: " + macAddressReceiver);
}

void SimpleWebServer::SetupRoutes() {
  server.on("/", HTTP_GET, [this]() {
    String html = R"rawliteral(
      <!DOCTYPE html>
      <html lang="pt-BR" data-theme="light">
        <head>
          <meta charset="UTF-8">
          <title>SolChef Dashboard</title>
          <link rel="stylesheet" href="https://unpkg.com/@picocss/pico@latest/css/pico.min.css">
          <style>
            /* === SPACING === */
            .m-0 { margin: 0; }
            .m-1 { margin: 0.25rem; }
            .m-2 { margin: 0.5rem; }
            .m-3 { margin: 1rem; }

            .mt-1 { margin-top: 0.25rem; }
            .mt-2 { margin-top: 0.5rem; }
            .mt-3 { margin-top: 1rem; }

            .mb-1 { margin-bottom: 0.25rem; }
            .mb-2 { margin-bottom: 0.5rem; }
            .mb-3 { margin-bottom: 1rem; }

            .p-1 { padding: 0.25rem; }
            .p-2 { padding: 0.5rem; }
            .p-3 { padding: 1rem; }

            /* === FLEXBOX === */
            .flex { display: flex; }
            .flex-col { flex-direction: column; }
            .flex-row { flex-direction: row; }
            .justify-start { justify-content: start; }
            .justify-center { justify-content: center; }
            .justify-around { justify-content: space-around; }
            .justify-between { justify-content: space-between; }
            .items-center { align-items: center; }
            .items-start { align-items: flex-start; }
            .items-end { align-items: flex-end; }
            .gap-1 { gap: 0.5rem; }
            .gap-2 { gap: 1rem; }
            .gap-3 { gap: 1.5rem; }

            /* === GRID === */
            .grid { display: grid; }
            .grid-2 { grid-template-columns: repeat(2, 1fr); }
            .grid-3 { grid-template-columns: repeat(3, 1fr); }
            .grid-4 { grid-template-columns: repeat(4, 1fr); }
            .grid-auto { grid-auto-flow: dense; }
            .grid-center { place-items: center; }
            .grid-gap-1 { gap: 0.5rem; }
            .grid-gap-2 { gap: 1rem; }

            /* === WIDTH HELPERS === */
            .w-25 { width: 25%; }
            .w-50 { width: 50%; }
            .w-75 { width: 75%; }
            .w-100 { width: 100%; }

            /* === TEXT === */
            .text-center { text-align: center; }
            .text-right { text-align: right; }
            .bold { font-weight: bold; }
            .lg { font-size: 1.35rem; }
            .sm { font-size: 0.875rem; }
            .decoration-none { text-decoration: none; }

            /* === SHADOW === */
            .shadow-sm {
              box-shadow: rgba(0, 0, 0, 0.15) 1.95px 1.95px 2.6px;
            }

            /* === CUSTOM === */
            .layout {
              grid-template-columns: 250px 1fr;
              min-height: 100dvh;
            }

            .sidebar {
              position: sticky;
              top: 0;
              height: 100dvh;
              padding: 1rem;
              border-right: 1px solid #ddd;
            }

            .sidebar a svg {
              margin-right: 0.5rem;
              vertical-align: middle;
            }

            /* === LINKS === */
            .nav-item {
              min-width: 180px;
              max-width: 200px;
              
              transition: 300ms all ease
            }

            .nav-item:hover {
              scale: 1.075;
            }

            .unselected {
              color: #010101;
              border: 1px solid rgba(0, 0, 0, .15);
              background-color: #fff; 
            }

            .unselected:hover {
              background-color: #FFF5EE;
            }

            .selected {
              background-color: rgba(0, 71, 171, 1);
              color: white;
            }
          </style>
        </head>
        <body>
          <!-- / -->
          <div class="grid layout">
            <!-- Sidebar -->
            <nav class="sidebar flex-col items-center justify-start">
              <ul class="flex-col">
                <li><strong class="lg bold">SolChef</strong></li>
                <li>
                  <a class="flex flex-row items-center justify-center nav-item text-center decoration-none selected" href="/">
                    <svg width="18" height="18" fill="none" stroke="currentColor" stroke-width="2" 
                      stroke-linecap="round" stroke-linejoin="round" viewBox="0 0 24 24">
                      <path d="M3 13h8V3H3zm10 8h8V11h-8zM3 21h8v-6H3zM13 3v6h8V3z"/>
                    </svg>
                    Dashboard
                  </a>
                </li>
                <li>
                  <a class="flex flex-row items-center justify-center nav-item text-center decoration-none shadow-sm unselected" href="/network">
                  <svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" class="feather feather-wifi"><path d="M5 12.55a11 11 0 0 1 14.08 0"></path><path d="M1.42 9a16 16 0 0 1 21.16 0"></path><path d="M8.53 16.11a6 6 0 0 1 6.95 0"></path><line x1="12" y1="20" x2="12.01" y2="20"></line></svg>
                    Network
                  </a>
                </li>
                <li>
                  <a class="flex flex-row items-center justify-center nav-item text-center decoration-none shadow-sm unselected" href="/device">
                    <svg width="20" height="20" fill="none" stroke="currentColor" stroke-width="2" 
                        stroke-linecap="round" stroke-linejoin="round" viewBox="0 0 24 24">
                      <rect x="7" y="4" width="10" height="16" rx="2" />
                      <path d="M11 19h2"/>
                    </svg>
                    Device
                  </a>
                </li>
                <li>
                  <a class="flex flex-row items-center justify-center nav-item text-center decoration-none shadow-sm unselected" href="/settings">
                    <svg width="18" height="18" fill="none" stroke="currentColor" stroke-width="2" 
                        stroke-linecap="round" stroke-linejoin="round" viewBox="0 0 24 24">
                      <circle cx="12" cy="12" r="3" />
                      <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-4 0v-.09a1.65 1.65 0 0 0-1-1.51 1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1 0-4h.09a1.65 1.65 0 0 0 1.51-1 1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 2.83-2.83l.06.06a1.65 1.65 0 0 0 1.82.33h.09a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 4 0v.09a1.65 1.65 0 0 0 1 1.51h.09a1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82v.09a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/>
                    </svg>
                    Settings
                  </a>
                </li>
              </ul>
            </nav>
            <!-- Sidebar -->
            <main class="content container mt-3">
              <h1>Dashboard</h1>
              <hr>
              <section>
                <h2>Sensor Data</h2>
                <progress />
                <pre id="data">
                </pre>
              </section>
              <section>
                <h2>Local Data</h2>
                <progress />
                <pre id="data">
                </pre>
              </section>
            </main>
          </div>
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

  server.on("/scan", HTTP_GET, [this]() {
    int n = WiFi.scanNetworks();
    StaticJsonDocument<1024> doc;
    JsonArray arr = doc.createNestedArray("networks");

    std::vector<String> seen;

    for(int i = 0; i < n; i++) {
      String ssid = WiFi.SSID(i);
      if(ssid.isEmpty()) continue;

      bool duplicate = false;
      for(auto &s : seen) {
        if(s == ssid) {
          duplicate = true;
          break;
        }
      }

      if(duplicate) continue;
      
      seen.push_back(ssid);

      JsonObject net = arr.createNestedObject();
      net["ssid"] = ssid;
      net["rssi"] = WiFi.RSSI(i);
      net["secure"] = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);
    }

    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);

    WiFi.scanDelete();
  });

  server.on("/solchef", HTTP_GET, [this]() {
    String mode = (WiFi.getMode() ==  WIFI_AP) ? "AP" : "Station";
    String json = "{";
    json += "\"status\":\"ok\",";
    json += "\"mode\":\"" + mode + "\",";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"mac\":\"" + WiFi.macAddress() + "\",";
    json += "}";

    server.send(200, "application/json", json);
  });

  server.on("/network", HTTP_GET, [this]() {
    String html = R"rawliteral(
      <!DOCTYPE html>
      <html lang="pt-BR" data-theme="light">
        <head>
          <meta charset="UTF-8">
          <title>SolChef Dashboard</title>
          <link rel="stylesheet" href="https://unpkg.com/@picocss/pico@latest/css/pico.min.css">
          <style>
            /* === SPACING === */
            .m-0 { margin: 0; }
            .m-1 { margin: 0.25rem; }
            .m-2 { margin: 0.5rem; }
            .m-3 { margin: 1rem; }

            .mt-1 { margin-top: 0.25rem; }
            .mt-2 { margin-top: 0.5rem; }
            .mt-3 { margin-top: 1rem; }

            .mb-1 { margin-bottom: 0.25rem; }
            .mb-2 { margin-bottom: 0.5rem; }
            .mb-3 { margin-bottom: 1rem; }

            .p-1 { padding: 0.25rem; }
            .p-2 { padding: 0.5rem; }
            .p-3 { padding: 1rem; }

            /* === FLEXBOX === */
            .flex { display: flex; }
            .flex-col { flex-direction: column; }
            .flex-row { flex-direction: row; }
            .justify-start { justify-content: start; }
            .justify-center { justify-content: center; }
            .justify-around { justify-content: space-around; }
            .justify-between { justify-content: space-between; }
            .items-center { align-items: center; }
            .items-start { align-items: flex-start; }
            .items-end { align-items: flex-end; }
            .gap-1 { gap: 0.5rem; }
            .gap-2 { gap: 1rem; }
            .gap-3 { gap: 1.5rem; }

            /* === GRID === */
            .grid { display: grid; }
            .grid-2 { grid-template-columns: repeat(2, 1fr); }
            .grid-3 { grid-template-columns: repeat(3, 1fr); }
            .grid-4 { grid-template-columns: repeat(4, 1fr); }
            .grid-auto { grid-auto-flow: dense; }
            .grid-center { place-items: center; }
            .grid-gap-1 { gap: 0.5rem; }
            .grid-gap-2 { gap: 1rem; }

            /* === WIDTH HELPERS === */
            .w-25 { width: 25%; }
            .w-50 { width: 50%; }
            .w-75 { width: 75%; }
            .w-100 { width: 100%; }

            /* === TEXT === */
            .text-center { text-align: center; }
            .text-right { text-align: right; }
            .bold { font-weight: bold; }
            .lg { font-size: 1.35rem; }
            .sm { font-size: 0.875rem; }
            .decoration-none { text-decoration: none; }

            /* === SHADOW === */
            .shadow-sm {
              box-shadow: rgba(0, 0, 0, 0.15) 1.95px 1.95px 2.6px;
            }

            /* === CUSTOM === */
            .layout {
              grid-template-columns: 250px 1fr;
              min-height: 100dvh;
            }

            .sidebar {
              position: sticky;
              top: 0;
              height: 100dvh;
              padding: 1rem;
              border-right: 1px solid #ddd;
            }

            .sidebar a svg {
              margin-right: 0.5rem;
              vertical-align: middle;
            }

            /* === LINKS === */
            .nav-item {
              min-width: 180px;
              max-width: 200px;
              
              transition: 300ms all ease
            }

            .nav-item:hover {
              scale: 1.075;
            }

            .unselected {
              color: #010101;
              border: 1px solid rgba(0, 0, 0, .15);
              background-color: #fff; 
            }

            .unselected:hover {
              background-color: #FFF5EE;
            }

            .selected {
              background-color: rgba(0, 71, 171, 1);
              color: white;
            }

            .spinner {
              display: inline-block;
              width: 14px;
              height: 14px;
              border: 2px solid #ccc;
              border-top: 2px solid #333;
              border-radius: 50%;
              animation: spin 0.8s linear infinite;
              vertical-align: middle;
              margin-left: 6px;
            }

            @keyframes spin {
              to { transform: rotate(360deg); }
            }            
          </style>
        </head>
        <body>
          <!-- / -->
          <div class="grid layout">
            <!-- Sidebar -->
            <nav class="sidebar flex-col items-center justify-start">
              <ul class="flex-col">
                <li><strong class="lg bold">SolChef</strong></li>
                <li>
                  <a class="flex flex-row items-center justify-center nav-item text-center decoration-none shadow-sm unselected" href="/">
                    <svg width="18" height="18" fill="none" stroke="currentColor" stroke-width="2" 
                      stroke-linecap="round" stroke-linejoin="round" viewBox="0 0 24 24">
                      <path d="M3 13h8V3H3zm10 8h8V11h-8zM3 21h8v-6H3zM13 3v6h8V3z"/>
                    </svg>
                    Dashboard
                  </a>
                </li>
                <li>
                  <a class="flex flex-row items-center justify-center nav-item text-center decoration-none selected" href="/network">
                  <svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" class="feather feather-wifi"><path d="M5 12.55a11 11 0 0 1 14.08 0"></path><path d="M1.42 9a16 16 0 0 1 21.16 0"></path><path d="M8.53 16.11a6 6 0 0 1 6.95 0"></path><line x1="12" y1="20" x2="12.01" y2="20"></line></svg>
                    Network
                  </a>
                </li>
                <li>
                  <a class="flex flex-row items-center justify-center nav-item text-center decoration-none shadow-sm unselected" href="/device">
                    <svg width="20" height="20" fill="none" stroke="currentColor" stroke-width="2" 
                        stroke-linecap="round" stroke-linejoin="round" viewBox="0 0 24 24">
                      <rect x="7" y="4" width="10" height="16" rx="2" />
                      <path d="M11 19h2"/>
                    </svg>
                    Device
                  </a>
                </li>
                <li>
                  <a class="flex flex-row items-center justify-center nav-item text-center decoration-none shadow-sm unselected" href="/settings">
                    <svg width="18" height="18" fill="none" stroke="currentColor" stroke-width="2" 
                        stroke-linecap="round" stroke-linejoin="round" viewBox="0 0 24 24">
                      <circle cx="12" cy="12" r="3" />
                      <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-4 0v-.09a1.65 1.65 0 0 0-1-1.51 1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1 0-4h.09a1.65 1.65 0 0 0 1.51-1 1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 2.83-2.83l.06.06a1.65 1.65 0 0 0 1.82.33h.09a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 4 0v.09a1.65 1.65 0 0 0 1 1.51h.09a1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82v.09a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/>
                    </svg>
                    Settings
                  </a>
                </li>
              </ul>
            </nav>
            <main class="content container mt-3">
              <h1>Configure Network</h1>
              <hr>
              <section>
                <h2>Wi-Fi Configuration</h2>

                <div class="flex-row items-center gap-sm">
                  <button id="scanBtn" type="button">🔍 Rescan Networks</button>
                  <span id="scanStatus" style="margin-left:10px; color:gray;">Idle</span>
                </div>

                <form id="wifiForm">
                  <label for="ssid">SSID:</label>
                  <input list="networks" id="ssid" name="ssid" required />
                  <datalist id="networks"></datalist>

                  <label for="password">Password:</label>
                  <input type="password" id="password" name="password" required />

                  <button type="submit">Save & Restart</button>
                </form>
                <p id="msg"></p>
              </section>
            </main>
          </div>
          <script>
            async function loadNetworks() {
              const status = document.getElementById("scanStatus");
              const list = document.getElementById("networks");
              const btn = document.getElementById("scanBtn");

              try {
                status.innerHTML = 'Scanning... <span class="spinner"></span>';
                btn.disabled = true;
                list.innerHTML = "";

                const res = await fetch("/scan");
                if (!res.ok) throw new Error("HTTP " + res.status);

                const data = await res.json();

                // Deduplicate and clean SSIDs
                const uniqueSSIDs = [...new Set(
                  data.networks
                    .map(net => net.ssid.trim())
                    .filter(ssid => ssid.length > 0)
                )];

                if (uniqueSSIDs.length === 0) {
                  status.textContent = "No networks found";
                  return;
                }

                uniqueSSIDs.forEach(ssid => {
                  const opt = document.createElement("option");
                  opt.value = ssid;
                  list.appendChild(opt);
                });

                status.textContent = `Found ${uniqueSSIDs.length} networks`;
              } catch (err) {
                console.error("Failed to load networks:", err);
                status.textContent = "Scan failed";
              } finally {
                btn.disabled = false;
              }
            }

            async function loadInfo() {
              try {
                const res = await fetch('/solchef');
                const json = await res.json();
                document.getElementById('mode').textContent = json.mode || '-';
                document.getElementById('ip').textContent = json.ip || '-';
                document.getElementById('mac').textContent = json.mac || '-';
              } catch (err) {
                console.error("Failed to load info:", err);
              }
            }

            async function loadData() {
              try {
                const res = await fetch('/dadossolchef');
                const json = await res.json();
                document.getElementById('data').textContent = JSON.stringify(json, null, 2);
              } catch (err) {
                console.error("Failed to load data:", err);
              }
            }

            document.getElementById("wifiForm").addEventListener("submit", async (e) => {
              e.preventDefault();
              const ssid = document.getElementById("ssid").value;
              const password = document.getElementById("password").value;

              const msg = document.getElementById("msg");
              msg.textContent = "Sending credentials...";

              try {
                const res = await fetch("/config", {
                  method: "POST",
                  headers: { "Content-Type": "application/x-www-form-urlencoded" },
                  body: `ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`
                });

                const result = await res.json();
                msg.textContent = result.msg;

                if (result.status === "ok") {
                  // Give the ESP32 a moment, then go to /networks
                  setTimeout(() => {
                    window.location.href = "/networks";
                  }, 2000);
                }
              } catch (err) {
                console.error("Failed to send credentials:", err);
                msg.textContent = "Error sending credentials.";
              }
            });

            document.getElementById("scanBtn").addEventListener("click", loadNetworks);

            // Initial loads
            loadNetworks();
            loadInfo();
            loadData();

            // Auto refresh data every 2 seconds
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