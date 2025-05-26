#include <WiFi.h>
#include <PubSubClient.h>
#include "HX711_ADC.h"
#include <LiquidCrystal.h>

/**
 * @brief Configuração do WiFi e MQTT
 */
const char* ssid = "Wokwi-GUEST"; ///< SSID do WiFi
const char* password = ""; ///< Senha do WiFi
const char* mqttServer = "broker.hivemq.com"; ///< Endereço do broker MQTT
const int mqttPort = 1883; ///< Porta do broker MQTT
const char* mqttTopic = "weightScale/data"; ///< Tópico MQTT para os dados de peso

WiFiClient espClient; ///< Cliente WiFi
PubSubClient client(espClient); ///< Cliente MQTT

/**
 * @brief Configuração da célula de carga e LCD
 */
LiquidCrystal lcd(23, 22, 5, 17, 16, 4); ///< LCD com pinos configurados
HX711_ADC LoadCell(19, 18); ///< Célula de carga HX711 com pinos de dados e clock
int tareButton = 25; ///< Pino do botão de tara
const float overload = 5000.0; ///< Limite de sobrecarga em gramas

/**
 * @brief Inicializa WiFi, MQTT, célula de carga e LCD
 */
void setup() {
  Serial.begin(115200);
  connectToWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  connectToMQTT();

  pinMode(tareButton, INPUT_PULLUP);
  LoadCell.begin();
  LoadCell.start(1000);
  LoadCell.tare();
  LoadCell.setCalFactor(0.42);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Weighing Scale");
}

/**
 * @brief Conecta-se ao WiFi
 */
void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("WiFi conectado!");
}

/**
 * @brief Conecta-se ao broker MQTT
 */
void connectToMQTT() {
  while (!client.connected()) {
    Serial.println("Conectando ao MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado ao MQTT!");
      client.subscribe(mqttTopic);
    } else {
      Serial.print("Falha na conexão. Código: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

/**
 * @brief Callback para mensagens MQTT
 * @param topic Tópico recebido
 * @param payload Mensagem recebida
 * @param length Tamanho da mensagem
 */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem no tópico: ");
  Serial.print(topic);
  Serial.print(". Conteúdo: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/**
 * @brief Loop principal: atualiza peso, publica no MQTT e exibe no LCD
 */
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
  if (!client.connected()) {
    connectToMQTT();
  }

  client.loop();
  LoadCell.update();
  float weight = LoadCell.getData();
  Serial.println(weight);

  if (weight < overload) {
    lcd.setCursor(1, 1);
    lcd.print(weight, 0);
    lcd.print(" g    ");

    String message = "{ \"weight\": \"" + String(weight, 0) + "\" }";
    client.publish(mqttTopic, message.c_str());
    Serial.println("Mensagem enviada: " + message);
  } else {
    lcd.setCursor(1, 1);
    lcd.print("Over Load");
    client.publish(mqttTopic, "{ \"weight\": \"Over Load\" }");
  }

  if (digitalRead(tareButton) == LOW) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Taring----");
    LoadCell.start(1000);
    delay(200);
    lcd.clear();
    lcd.print("Weighing Scale");
  }

  delay(100);
}
