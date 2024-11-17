Este código implementa um sistema de balança eletrônica utilizando um ESP32, uma célula de carga com o amplificador HX711, e um display LCD para exibir o peso medido. Ele também conecta-se a uma rede WiFi para publicar os dados de peso via protocolo MQTT em um broker público. 

Instruções para rodar o código:

 	1. Após criar uma conta no Wokwi, faça login e crie um projeto ESP32.
 	2. Nesse projeto, cole o código dado e implemente o circuito com base nas instruções do "InstrucoesCircuito_ESP32_HX711_LCD.ino".
	3. Observe que no código a configuração para o Wifi é um ponto de acesso aberto e nenhuma senha é necessária:
	    	const char* ssid = "Wokwi-GUEST"; 
		const char* password = ""; 
	4. Caso o queira conectar-se a uma rede Wifi pessoal siga esses passos, altere o SSID e a senha:
		const char* ssid = "Seu_SSID";      // Substitua por sua rede WiFi
		const char* password = "Sua_Senha"; // Substitua pela senha da sua rede
	5. Observe que no código a configuração para a conexão com o MQTT é publica, alocada na porta 1883 (padrão para comunicação mqtt não criptografada),
	   e publica mensagens no tópico "weightScale/data":
		const char* mqttServer = "broker.hivemq.com"; 
		const int mqttPort = 1883; 
		const char* mqttTopic = "weightScale/data"; 
	6. Caso o queira conectar-se a um MQTT privado, teste substituir o código pelo implementado no "Wifi_MQTT_Alteration.txt".

Aqui está um resumo do funcionamento:

	1. Configuração inicial:
  	 - Conecta-se à rede WiFi.
  	 - Configura o cliente MQTT e se conecta ao broker.
   	 - Inicializa a célula de carga, realiza o processo de tara (calibração), e configura o display LCD.

	2. Loop principal:
   	 - Atualiza as leituras da célula de carga.
   	 - Exibe o peso no LCD.
  	 - Publica o peso (em JSON) no tópico MQTT, se estiver abaixo do limite de sobrecarga.
  	 - Detecta condição de sobrecarga e publica uma mensagem correspondente.

	3. Função de tara:
   	 - Detecta a pressão do botão de tara e reinicia as configurações da célula de carga para recalibrar o peso base.

	4. MQTT:
   	 - Recebe e processa mensagens de um tópico MQTT (embora o código apenas exiba essas mensagens no monitor serial, sem ações adicionais).

O código é ideal para aplicações de monitoramento remoto de peso, como em balanças inteligentes ou sistemas industriais.
