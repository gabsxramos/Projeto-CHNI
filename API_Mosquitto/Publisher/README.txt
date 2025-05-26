### Publicador MQTT com ESP32 e Sensor HX711

Este programa utiliza um **ESP32** para capturar leituras de um **sensor de peso HX711** e publicar os dados via **MQTT** em um broker.

#### Funcionamento
1. Conecta o ESP32 a uma **rede Wi-Fi**.
2. Inicializa a **balança HX711** e realiza a calibração.
3. Mantém a conexão com o **broker MQTT**.
4. A cada **5 segundos**, lê o peso e envia em formato **JSON** para o tópico `data/weight`.

#### Componentes e Bibliotecas
- **WiFi.h** → Conexão com a rede.
- **PubSubClient.h** → Comunicação MQTT.
- **ArduinoJson.h** → Formatação dos dados.
- **HX711.h** → Operação do sensor de peso.

#### Configuração
- **Wi-Fi:** Definido no código (`ssid` e `password`).
- **MQTT:** Conecta ao broker `test.mosquitto.org` (porta 1883).
- **Pinos HX711:** `DT = GPIO 14`, `SCK = GPIO 12`.

#### Loop Principal
- Mantém a conexão MQTT ativa.
- Obtém o peso do HX711 e formata os dados em JSON.
- Publica a mensagem no **broker MQTT**.



