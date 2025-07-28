#include <Arduino.h>
#include "Display.hpp"
#include "WifiManager.hpp"
#include "SimpleWebServer.hpp"
#include "DadosSolchef.hpp"
#include "SensorTemperatura.hpp"
#include "NvsManager.hpp"
#include "Gps.hpp"
#include "LoraService.hpp"
#include "RabbitMqService.hpp"

// Objetos globais
Display oled(21, 22, 128, 64); // pino SDA, pino SCL, largura, altura
WifiManager wifiManager;
SimpleWebServer webServer;
SensorTemperatura sensorTemperatura(4);
NvsManager nvsManager;   // ao criar já inicializa a NVS e corrige se necessário
Gps gps(34, 12, 9600);
LoraService loraService(
    915E6,   // frequencia LoRa
    18,      // ss / CS
    14,      // rst
    26       // dio0
);
RabbitMqService rabbit("http://192.168.100.107:15672", "solchef", "solchef");
bool apModeAtivo = false;

void setup() {
  Serial.begin(115200);
  oled.Start(); // Inicia o display OLED
  gps.IniciaGps();

  // Inicia LoRa
  if (!loraService.Start()) {
    Serial.println("Erro ao iniciar LoRa.");
    delay(3000);
    while (true); // trava para debug
  }

  // verifica se ja tem wifi cadastrado
  String savedSSID, savedPass;
  bool conectado = false;

  if (nvsManager.LerNvsWifi(savedSSID, savedPass)) {
    Serial.println("Credenciais encontradas. Tentando conectar...");
    conectado = wifiManager.connect(savedSSID.c_str(), savedPass.c_str(), 10000);

    if (!conectado) {
      Serial.println("Falha ao conectar → iniciando AP");
      wifiManager.startAccessPoint("SolChef_Config", "12345678");
      apModeAtivo = true;
    } else {
      Serial.println("Conectado em Station Mode!");
    }
  } else {
    Serial.println("Nenhuma credencial salva. Iniciando AP...");
    wifiManager.startAccessPoint("SolChef_Config", "12345678");
    apModeAtivo = true;
  }

  // Sempre inicia o servidor web, independente do modo
  webServer.SetupRoutes();
  delay(1000); // Aguarda o servidor web iniciar
  Serial.println("Setup concluido!");
}

void loop() {
/* 
  webServer.HandleClient();  // Mantem o servidor HTTP responsivo
  Serial.println("Aguardando dados LoRa...");
  String payload;

  if (loraService.ReceberDados(payload)) {

    // Tenta desserializar
    if (dados.FromJson(payload)) {
      
      // Exibe JSON no Serial
      Serial.println("Dados desserializados:");
      Serial.println(dados.ToJSON());
      
      dados.macAddressReceiver = wifiManager.getMeuMacAddress().c_str();

      // Exibe dados no display OLED
      oled.Clear();
      oled.PrintLine(0, ("IP: " + wifiManager.getIPAddress()).c_str());
      oled.PrintLine(1, ("Ag: " + String(dados.tempAgua) + " Int: " + String(dados.tempInterna)).c_str());
      oled.PrintLine(2, ("Date: " + String(dados.horaRegistro)).c_str());
      oled.PrintLine(3, ("GPS: " + String(dados.latitude) + " | " + String(dados.longitude)).c_str());

      webServer.SendData(dados);
      rabbit.PublishToQueue(dados);
    } 
    else 
    {
      // Payload invÃ¡lido (formato incorreto)
      Serial.println("Payload invÃ¡lido:");
      Serial.println(payload);
    }
    delay(2000);  // Aguarda 02 segundos antes de receber novamente
  }*/
  
  gps.SincronizaGps();
  webServer.HandleClient();  // Mantem o servidor HTTP responsivo

  // Criar objeto de dados simulados
  DadosSolchef dados;
  dados.tempAgua = sensorTemperatura.GetTemperaturaAgua();    
  dados.tempInterna = sensorTemperatura.GetTemperaturaInterna(); 

  if (gps.TemFix())
  {
    dados.latitude = gps.GetLatitude(); // usa TinyGPSPlus
    dados.longitude = gps.GetLongitude();
  }
  else
  {
    dados.latitude = 0.0;
    dados.longitude = 0.0;
  }

  char bufferHora[6];
  snprintf(bufferHora, sizeof(bufferHora), "%02d:%02d", gps.GetHora().toInt());
  dados.horaRegistro = String(bufferHora);
  dados.macAddressSender = wifiManager.getMeuMacAddress().c_str();

  // Serializa e envia via LoRa
  //loraService.EnviaDados(dados);
  //Serial.println("Payload enviado:");
  //gps.ImprimirDadosGps();
  webServer.SendData(dados);
  rabbit.PublishToQueue(dados);
  delay(5000);
}

