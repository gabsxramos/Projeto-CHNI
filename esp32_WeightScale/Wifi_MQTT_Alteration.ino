#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "HX711_ADC.h"
#include <LiquidCrystal.h>

/**
 * @brief WiFi and MQTT configuration parameters
 */
// WiFi Credentials
const char* ssid = "Seu_SSID";      ///< WiFi SSID
const char* password = "Sua_Senha"; ///< WiFi password

// MQTT Broker Credentials
const char* mqttServer = "meubroker.com"; ///< MQTT broker address (IP or domain)
const int mqttPort = 8883;                ///< MQTT broker port (use 1883 for unsecured or 8883 for SSL/TLS)
const char* mqttUser = "usuario";         ///< MQTT username
const char* mqttPassword = "senha";       ///< MQTT password
const char* mqttTopic = "weightScale/data"; ///< MQTT topic for weight data

// SSL/TLS Certificate for secure connection
const char* caCert = R"EOF(
-----BEGIN CERTIFICATE-----
SEU_CERTIFICADO_CA_AQUI
-----END CERTIFICATE-----
)EOF";

// WiFi and MQTT Clients
WiFiClientSecure espClientSecure; ///< Secure WiFi client instance
PubSubClient client(espClientSecure); ///< MQTT client instance using secure WiFi client

/**
 * @brief Load cell and LCD configuration
 */
LiquidCrystal lcd(23, 22, 5, 17, 16, 4); ///< LCD configuration with pins
HX711_ADC LoadCell(19, 18); ///< HX711 load cell instance with data and clock pins
int tareButton = 25; ///< Pin for the tare button
const float overload = 5000.0; ///< Overload threshold in grams

/**
 * @brief Arduino setup function. Initializes WiFi, MQTT, load cell, and LCD.
 */
void setup() {
  Serial.begin(115200); // Initialize serial communication
  connectToWiFi(); // Connect to WiFi network

  // Set up secure connection and MQTT server
  espClientSecure.setCACert(caCert); // Configure SSL/TLS certificate
  client.setServer(mqttServer, mqttPort); // Set MQTT server
  client.setCallback(callback); // Set MQTT callback function
  connectToMQTT(); // Connect to MQTT broker

  pinMode(tareButton, INPUT_PULLUP); // Configure tare button as input with pull-up resistor
  LoadCell.begin(); // Initialize the load cell
  LoadCell.start(1000); // Stabilize the load cell
  LoadCell.tare(); // Tare the load cell (reset to zero)
  LoadCell.setCalFactor(0.42); // Set calibration factor for the load cell

  lcd.begin(16, 2); // Initialize the LCD with 16 columns and 2 rows
  lcd.clear(); // Clear LCD display
  lcd.print("Weighing Scale"); // Display initial message on LCD
}

/**
 * @brief Connects to the WiFi network.
 */
void connectToWiFi() {
  WiFi.begin(ssid, password); // Start WiFi connection
  while (WiFi.status() != WL_CONNECTED) { // Wait for connection
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!"); // WiFi connected successfully
}

/**
 * @brief Connects to the MQTT broker.
 */
void connectToMQTT() {
  while (!client.connected()) { // Attempt to connect until successful
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) { // Use credentials
      Serial.println("Connected to MQTT broker!");
      client.subscribe(mqttTopic); // Subscribe to the MQTT topic
    } else {
      Serial.print("Failed to connect. Error code: ");
      Serial.println(client.state()); // Print connection error code
      delay(5000); // Retry after 5 seconds
    }
  }
}

/**
 * @brief MQTT message callback function.
 * 
 * @param topic The topic on which the message was received.
 * @param payload The payload of the message.
 * @param length The length of the payload.
 */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]); // Print message payload character by character
  }
  Serial.println();
}

/**
 * @brief Arduino loop function. Runs continuously to update the load cell, MQTT, and LCD.
 */
void loop() {
  if (WiFi.status() != WL_CONNECTED) { // Reconnect if WiFi is disconnected
    connectToWiFi();
  }

  if (!client.connected()) { // Reconnect if MQTT is disconnected
    connectToMQTT();
  }

  client.loop(); // Process incoming MQTT messages
  LoadCell.update(); // Update load cell readings

  float weight = LoadCell.getData(); // Get the current weight value
  Serial.println(weight); // Print weight to the serial monitor

  if (weight < overload) { // Check if weight is below the overload threshold
    lcd.setCursor(1, 1); // Set cursor position on LCD
    lcd.print(weight, 0); // Print weight value (no decimal places)
    lcd.print(" g    "); // Append "g" to indicate grams

    // Publish weight to MQTT in JSON format
    String message = "{ \"weight\": \"" + String(weight, 0) + "\" }";
    client.publish(mqttTopic, message.c_str()); // Send weight data to MQTT topic
    Serial.println("Message sent: " + message);
  } else { // Handle overload condition
    lcd.setCursor(1, 1);
    lcd.print("Over Load");
    client.publish(mqttTopic, "{ \"weight\": \"Over Load\" }"); // Send overload status
  }

  // Check if tare button is pressed
  if (digitalRead(tareButton) == LOW) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Taring----"); // Display "Taring" message
    LoadCell.start(1000); // Reset load cell
    delay(200); // Small delay for stability
    lcd.clear();
    lcd.print("Weighing Scale"); // Restore initial message
  }

  delay(100); // Delay to prevent excessive updates
}
