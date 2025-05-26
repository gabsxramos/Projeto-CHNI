# Projeto-CHNI
Projeto em parceria com a empresa CHNI, com objetivo de criar uma forma de sinalização sobre informações das peças dos carrinhos de montagem utilizados na empresa.

PASTAS :

Wokwi_ESP32_MQTT_Publisher => Códigos usados no Wokwi para simular o circuito com o ESP32 e a célula de carga 5gk HX711, que publica as mensagens/dados em formato json para o HiveMQ.

HiveMQTT_Client(API) => Códigos em C# do cliente MQTT, ou seja, a API, que recebe as mensagens vindas do HiveMQ atráves da assinatura de tópico.

ESP32_ MQTT_Publisher => Código extra em C++ para publicar dados teste no broker MQTT HiveMQ.
