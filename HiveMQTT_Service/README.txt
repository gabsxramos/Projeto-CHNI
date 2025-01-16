Este código implementa um serviço hospedado em .NET para integração com o protocolo MQTT, permitindo a comunicação com um broker MQTT, como o HiveMQ, para assinar tópicos e processar mensagens. 

Instruções para rodar o código:

 1. Crie uma conta e faça login no HiveMQ.
 2. Crie um Cluster, e utilize as informações do mesmo para configurar o código:
	- Broker URL: Endereço do broker MQTT (por exemplo, your-cluster-uuid.hivemq.cloud).
	- Porta: 8883 para comunicação segura via SSL/TLS.
	- Usuário e senha: Credenciais de autenticação geradas para o seu cluster.
 3. Atualize o código para usar o cluster:
	- No código fornecido, substitua as constantes com as informações do cluster:
		private const string mqttServer = "your-cluster-uuid.hivemq.cloud"; // URL do broker MQTT
		private const int mqttPort = 8883; // Porta para comunicação segura (SSL/TLS)
		private const string mqttUsername = "your-username"; // Usuário fornecido pelo HiveMQ Cloud
		private const string mqttPassword = "your-password"; // Senha fornecida pelo HiveMQ Cloud
4. Para rodar o código, digite no terminal:
	cd "diretório_do_seu_programa"
	dotnet run

Abaixo está um resumo de sua funcionalidade:

1. **Classe MqttService**:
   - Implementa a interface `IHostedService` para configurar o serviço MQTT como um serviço hospedado.
   - Conecta-se ao broker MQTT usando o cliente MQTTnet.
   - Assina o tópico especificado para receber mensagens, como o tópico `weightScale/data` usado neste exemplo prático.
   - Processa mensagens recebidas em formato JSON, desserializando-as para um objeto `SeuObjeto`.
   - Lida com conexões e desconexões do broker MQTT.

2. **Classe SeuObjeto**:
   - Representa a estrutura do JSON recebido via MQTT.
   - Inclui propriedades como `DeviceID`, `BatteryLevel` e `Charge` para armazenar os dados recebidos.

3. **Namespace Startup**:
   - Define a classe `Program` para iniciar o host da aplicação.
   - Configura o ambiente padrão e define a classe de inicialização da aplicação.

4. **Funcionamento Geral**:
   - O serviço conecta-se a um broker MQTT.
   - Após a conexão, ele assina um tópico especificado (`weightScale/data`) para receber mensagens publicadas.
   - Mensagens recebidas são exibidas no console e, se forem JSON válidos, são desserializadas para a classe `SeuObjeto` para posterior processamento.
   - O serviço gerencia o ciclo de vida da conexão MQTT, desconectando-se quando encerrado.

5. **Aplicação**:
   - Ideal para sistemas que precisam monitorar dispositivos IoT ou integrar dados recebidos via MQTT com uma aplicação .NET.
   - Oferece um modelo escalável e extensível para processar mensagens MQTT em tempo real.
