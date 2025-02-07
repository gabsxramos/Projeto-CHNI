# Projeto-CHNI
Projeto em parceria com a empresa CHNI, com objetivo de criar uma forma de sinalização sobre informações das peças dos carrinhos de montagem utilizados na empresa.

PASTAS :

esp32_WeightScale => Códigos usados no Wokwi para simular o circuito com o ESP32 e a célula de carga 5gk HX711, que envia/publica as mensagens em formato json e os envia para o HiveMQ.

HiveMQTT_service => Códigos em C# do cliente MQTT que recebe as mensagens vindas do HiveMQ.
