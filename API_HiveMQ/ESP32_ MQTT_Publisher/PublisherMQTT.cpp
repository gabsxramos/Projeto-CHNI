// -----------------------------------------------------------------------------------------------------------------------
// Program Module: MQTT Publisher Test
// -----------------------------------------------------------------------------------------------------------------------
// Atualizado para conexão com o HiveMQ Public Broker (broker.hivemq.com, porta 1883)
// -----------------------------------------------------------------------------------------------------------------------
// Descrição:
// Este programa configura um ESP32 para conectar-se a uma rede Wi-Fi e a um broker MQTT,
// enviando dados simulados de bateria (tensão, corrente e temperatura) em intervalos regulares.
// -----------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------
// Bibliotecas

#include <Arduino.h>
#include <WiFi.h>                           // Biblioteca para conexão Wi-Fi
#include <PubSubClient.h>                   // Biblioteca para comunicação MQTT

// -----------------------------------------------------------------------------------------------------------------------

// Variáveis para controle de tempo
unsigned long lastSendTime = 0;                // Armazena o timestamp da última mensagem enviada
const unsigned long sendInterval = 5000;       // Intervalo entre mensagens (em milissegundos)

// -----------------------------------------------------------------------------------------------------------------------
// Configuração

// Configuração Wi-Fi
const char* ssid = "ssid";                     // Nome da rede Wi-Fi (SSID)
const char* password = "password";             // Senha da rede Wi-Fi

// Configuração MQTT
const char* mqttServer = "broker.hivemq.com";  // Endereço do broker MQTT
const int mqttPort = 1883;                     // Porta do broker MQTT (não segura)

// -----------------------------------------------------------------------------------------------------------------------
// Objetos e Variáveis Globais

String macAddress;                                    // Variável para armazenar o endereço MAC do ESP32
WiFiClient espClient;                                 // Objeto para gerenciar conexão Wi-Fi
PubSubClient client(espClient);                       // Objeto para gerenciar conexão MQTT

// -----------------------------------------------------------------------------------------------------------------------
// Protótipos de Funções

void setupWiFi();                                     // Inicializa a conexão Wi-Fi
void connectMQTT();                                   // Conecta-se ao broker MQTT
void sendBatteryData();                               // Simula e envia dados de bateria via MQTT

// -----------------------------------------------------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);                               // Inicializa a comunicação serial
    setupWiFi();                                        // Configura a conexão Wi-Fi
    client.setServer(mqttServer, mqttPort);             // Configura o broker MQTT
}

// -----------------------------------------------------------------------------------------------------------------------

void loop() {
  // Verifica e mantém a conexão com o broker MQTT
  if (!client.connected()) {
    connectMQTT();
  }
  // Mantém a conexão MQTT ativa
  client.loop();

  // Verifica se é hora de enviar dados e envia, se necessário
  unsigned long currentMillis = millis();
  if (currentMillis - lastSendTime >= sendInterval) {
    sendBatteryData();                                  
    lastSendTime = currentMillis;                      // Atualiza o tempo da última mensagem enviada
  }
}

// -----------------------------------------------------------------------------------------------------------------------
// Implementações das Funções

// Conecta o ESP32 a uma rede Wi-Fi
void setupWiFi() {
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);

  const unsigned long timeout = 10000;                 // Timeout de 10 segundos
  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime) < timeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi conectado!");
    Serial.printf("Endereço IP: %s\n", WiFi.localIP().toString().c_str());
    macAddress = WiFi.macAddress();                    // Recupera e armazena o endereço MAC
    Serial.printf("Endereço MAC: %s\n", macAddress.c_str());                     
  } else {
    Serial.println("\nFalha ao conectar ao Wi-Fi. Tentando novamente...");
  }
}

// -----------------------------------------------------------------------------------------------------------------------

// Conecta ao broker MQTT
void connectMQTT() {
  const unsigned long reconnectInterval = 2000;   // Intervalo entre tentativas de reconexão
  unsigned long lastAttemptTime = millis();

  while (!client.connected() && (millis() - lastAttemptTime >= reconnectInterval)) {       
    Serial.println("Tentando conectar ao broker MQTT...");
    if (client.connect("ESP32Publisher")) {       // Identificador do cliente MQTT
      Serial.println("Conectado ao broker MQTT!");
    } else {
      Serial.print("Falha na conexão, código de erro: ");
      Serial.println(client.state());
      delay(reconnectInterval);
    }
  }
}

// -----------------------------------------------------------------------------------------------------------------------

// Simula e envia dados de bateria via MQTT
void sendBatteryData() {
  float voltage = random(3000, 4200) / 1000.0;       // Simula tensão entre 3.0V e 4.2V
  float current = random(0, 5000) / 1000.0;          // Simula corrente entre 0.0A e 5.0A
  float temperature = random(200, 450) / 10.0;       // Simula temperatura entre 20.0°C e 45.0°C

  // Formata os dados em uma string
  char payload[100];
  snprintf(payload, sizeof(payload), "MAC: %s, Tensão: %.2fV, Corrente: %.3fA, Temperatura: %.1f°C",
           macAddress.c_str(), voltage, current, temperature);

  // Publica os dados no tópico MQTT
  client.publish("battery/data", payload);
  
  // Exibe os dados enviados no monitor serial
  Serial.print("\nDados enviados: ");
  Serial.println(payload);
}
