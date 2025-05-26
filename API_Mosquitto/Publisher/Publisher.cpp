// -----------------------------------------------------------------------------------------------------------------------
// Módulo do Programa: Publicador MQTT
// -----------------------------------------------------------------------------------------------------------------------
//  	 
// Descrição: Programa para publicar dados de um sensor HX711 utilizando MQTT
// -----------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------

#include <Arduino.h>
#include <WiFi.h>                       	// Biblioteca para conexão Wi-Fi
#include <PubSubClient.h>               	// Biblioteca para comunicação MQTT
#include <ArduinoJson.h>                	// Biblioteca para formato JSON
#include "HX711.h"                      	// Biblioteca para o sensor HX711

// -----------------------------------------------------------------------------------------------------------------------

// Variáveis para controle de tempo
unsigned long lastSendTime = 0;            	// Armazena o timestamp da última mensagem enviada
const unsigned long sendInterval = 5000;   	// Intervalo entre mensagens (em milissegundos)

// -----------------------------------------------------------------------------------------------------------------------

#define DT 14                              	// Pino de dados do sensor HX711
#define SCK 12                             	// Pino de clock do sensor HX711

// -----------------------------------------------------------------------------------------------------------------------

// Configuração Wi-Fi
const char* ssid = "NOME_DA_REDE";          	// Nome da rede Wi-Fi (SSID)
const char* password = "senha123";          	// Senha do Wi-Fi

// Configuração MQTT
const char* mqttServer = "test.mosquitto.org";    	// Endereço do broker MQTT
const int mqttPort = 1883;                        	// Porta do broker MQTT
const char* mqttUser = "mqqtUser";                	// (Opcional) Usuário MQTT
const char* mqttPassword = "mqttPassword";        	// (Opcional) Senha MQTT

// -----------------------------------------------------------------------------------------------------------------------

String macAddress;                                	// Variável para armazenar o endereço MAC do ESP32
HX711 scale;                                      	// Objeto para gerenciar operações da balança
WiFiClient espClient;                             	// Objeto para gerenciar a conexão Wi-Fi
PubSubClient client(espClient);                   	// Objeto para gerenciar a conexão MQTT

// -----------------------------------------------------------------------------------------------------------------------

void setupWiFi();                                 	// Inicializa a conexão Wi-Fi
void connectMQTT();                               	// Conecta ao broker MQTT
void setupScale();                                	// Inicializa a balança
void sendData();                                  	// Envia dados via MQTT

// -----------------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);                           	// Inicializa a comunicação serial
  setupWiFi();                                    	// Configura a conexão Wi-Fi
  client.setServer(mqttServer, mqttPort);         	// Configura o broker MQTT
  setupScale();                                   	// Configura a balança
}

// -----------------------------------------------------------------------------------------------------------------------

void loop() {
  // Verifica e mantém a conexão com o broker MQTT
  if(!client.connected()) {
	connectMQTT();
  }
  // Mantém a conexão MQTT ativa
  client.loop();

  // Verifica se é hora de enviar dados e envia, se necessário
  unsigned long currentMillis = millis();
  if(currentMillis - lastSendTime >= sendInterval) {
	sendData();                                   	 
	lastSendTime = currentMillis;                         	// Atualiza o tempo da última mensagem enviada
  }
}

// -----------------------------------------------------------------------------------------------------------------------

// Conecta o ESP32 a uma rede Wi-Fi
void setupWiFi() {
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);

  const unsigned long timeout = 10000;                                        	// Timeout de 10 segundos
  unsigned long startAttemptTime = millis();

  while(WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime) < timeout) {
	delay(500);
	Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED) {
	Serial.println("\nWi-Fi conectado!");
	Serial.printf("Endereço IP: %s\n", WiFi.localIP().toString().c_str());
	macAddress = WiFi.macAddress();                                         	// Obtém e armazena o endereço MAC
	Serial.printf("Endereço MAC: %s\n", macAddress.c_str());                	 
  } else {
	Serial.println("\nFalha ao conectar ao Wi-Fi. Tentando novamente...");
  }
}

// -----------------------------------------------------------------------------------------------------------------------

// Conecta ao broker MQTT
void connectMQTT() {
  unsigned long lastAttemptTime = 0;          	// Tempo da última tentativa de conexão
  const unsigned long reconnectInterval = 2000;   // Intervalo entre tentativas de reconexão

  if(!client.connected() && (millis() - lastAttemptTime >= reconnectInterval)) {  	 
	Serial.println("Tentando conectar ao broker MQTT...");
	if(client.connect("ESP32Publisher")) {    	// Identificador do cliente MQTT
  	Serial.println("Conectado ao broker MQTT!");
	} else {
  	Serial.print("Falha na conexão, código de erro: ");
  	Serial.print(client.state());
	}
	lastAttemptTime = millis();               	// Atualiza o timestamp da última tentativa
  }
}

// -----------------------------------------------------------------------------------------------------------------------

void setupScale() {
  // Inicializa o HX711 com os pinos de dados e clock
  scale.begin(DT, SCK);                         	 

  // Verifica se a balança está pronta após inicialização
  if(scale.is_ready()) {
	Serial.println("Balança pronta.");
	scale.set_scale(473759.058823);                 	// Define o fator de calibração
	scale.tare(0);                                  	// Tare é o ajuste para zero
	Serial.println("Balança pronta. Coloque o item para pesagem.");
  } else {
	Serial.println("Balança não pronta. Verifique as conexões.");
  }
}


// -----------------------------------------------------------------------------------------------------------------------

void sendData() {
  float weight = scale.get_units(20);

  if(weight <= 0.001) {
	weight = 0.0;                  	 
	Serial.print("Peso inválido detectado. Enviando 0 kg.");
  }
    
  Serial.print("Peso: ");
  Serial.print(weight, 3);      	// Exibe o peso com 3 casas decimais
  Serial.println(" kg");

  // Cria o documento JSON
  StaticJsonDocument<200> jsonDoc;

  // Documento JSON com os dados
  jsonDoc["mac"] = macAddress;                    	// Adiciona o endereço MAC
  jsonDoc["weight"] = weight;                     	// Adiciona o peso

  // Serializa o documento JSON para uma string
  char payload[256];
  serializeJson(jsonDoc, payload, sizeof(payload));

  // Publica os dados no tópico MQTT
  client.publish("data/weight", payload);
 
  // Exibe os dados enviados no monitor serial
  Serial.print("\nDados enviados: ");
  Serial.println(payload);
}
