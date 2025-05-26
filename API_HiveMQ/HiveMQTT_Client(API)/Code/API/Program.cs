using System.Security.Authentication;
using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Formatter;
using System.Text;
using System.Text.Json;

public class MqttService : IHostedService
{
    // Definições de configuração do broker MQTT e credenciais de autenticação
    private readonly IMqttClient _mqttClient;
    private const string mqttServer = "2bbeb90f7c024f7a81d9db08b77aaf04.s1.eu.hivemq.cloud"; // Endereço do broker MQTT
    private const int mqttPort = 8883; // Porta segura MQTT
    private const string topic = "weightScale/data"; // Tópico MQTT para assinatura --- Caso mude o tópico no broker, é necessario mudar para o mesmo tópico aqui, caso contraio, o código não receberá as mensagens/dados...
    private const string username = "hivemq.webclient.1732295513039"; // Nome de usuário para o cluster --- Veja o seu cluster e mude aqui tambem ...
    private const string password = "t68:@Go.C45hSgr>KlOP"; // Senha para o cluster --- Veja a senha do seu cluster e mude aqui tambem ...

    // Construtor que cria uma instância do cliente MQTT
    public MqttService()
    {
        var factory = new MqttFactory();
        _mqttClient = factory.CreateMqttClient(); // Criação do cliente MQTT
    }

    // Método chamado para iniciar o serviço e conectar ao broker MQTT
    public async Task StartAsync(CancellationToken cancellationToken)
    {
        // Configurações do cliente MQTT
        var mqttClientOptions = new MqttClientOptionsBuilder()
            .WithTcpServer(mqttServer, mqttPort) // Define o servidor e a porta
            .WithCredentials(username, password) // Configura o nome de usuário e senha
            .WithTlsOptions(o =>
            {
                o.WithCertificateValidationHandler(_ => true); // Aceita todos os certificados (apenas para testes)
                o.WithSslProtocols(SslProtocols.Tls12); // Define o protocolo TLS 1.2
            })
            .WithProtocolVersion(MqttProtocolVersion.V500) // Define a versão do protocolo MQTT
            .Build(); // Criação do objeto de opções

        // Configura evento de conexão
        _mqttClient.ConnectedAsync += async e =>
        {
            Console.WriteLine("Conectado ao broker MQTT com TLS"); // Confirmação no console
            await _mqttClient.SubscribeAsync(topic); // Inscreve-se no tópico
            Console.WriteLine($"Inscrito no tópico: {topic}");
        };

        // Configura evento de desconexão
        _mqttClient.DisconnectedAsync += e =>
        {
            Console.WriteLine("Desconectado do broker MQTT");
            return Task.CompletedTask; // Retorno de tarefa para indicar desconexão
        };

        // Configura evento de recebimento de mensagens
        _mqttClient.ApplicationMessageReceivedAsync += e =>
        {
            var jsonPayload = Encoding.UTF8.GetString(e.ApplicationMessage.PayloadSegment); // Converte payload binário para string
            Console.WriteLine("Mensagem recebida: " + jsonPayload); // Exibe a mensagem no console

            try
            {
                // Desserializa a mensagem JSON para um objeto do tipo `SeuObjeto`
                var data = JsonSerializer.Deserialize<SeuObjeto>(jsonPayload);
                // Aqui você pode processar o objeto `data` conforme necessário
            }
            catch (Exception ex)
            {
                // Em caso de erro na desserialização, exibe a mensagem de erro
                Console.WriteLine("Erro ao desserializar JSON: " + ex.Message);
            }

            return Task.CompletedTask;
        };

        // Conecta ao broker MQTT com as opções configuradas
        await _mqttClient.ConnectAsync(mqttClientOptions, cancellationToken);
    }

    // Método chamado para parar o serviço e desconectar do broker MQTT
    public async Task StopAsync(CancellationToken cancellationToken)
    {
        await _mqttClient.DisconnectAsync(); // Desconecta do broker MQTT
        Console.WriteLine("Desconectado do broker MQTT");
    }
}

// Classe que define a estrutura de dados do objeto esperado nas mensagens MQTT
public class SeuObjeto
{
    public string DeviceID { get; set; } = string.Empty; // Identificador do dispositivo
    public float BatteryLevel { get; set; } // Nível de bateria do dispositivo
    public float Charge { get; set; } // Carga do dispositivo (pode ser percentual ou outro tipo de medição)
}

namespace Startup
{
    // Classe principal do programa que inicializa o aplicativo ASP.NET Core
    public class Program
    {
        public static void Main(string[] args)
        {
            // Criação e execução do host da aplicação
            CreateHostBuilder(args).Build().Run();
        }

        // Configuração do host e da aplicação ASP.NET Core
        public static IHostBuilder CreateHostBuilder(string[] args) =>
            Host.CreateDefaultBuilder(args)
                .ConfigureWebHostDefaults(webBuilder =>
                {
                    webBuilder.UseStartup<Startup>(); // Usa a classe Startup para configurar a aplicação
                });
    }
}
