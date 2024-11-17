using MQTTnet;
using MQTTnet.Client;
using System.Text;
using System.Text.Json;
//using Microsoft.AspNetCore.Hosting;
//using Microsoft.Extensions.Hosting;
//using System.Threading;
//using System.Threading.Tasks;
//using System;

/// <summary>
/// Serviço MQTT implementado como um serviço hospedado no .NET.
/// Este serviço conecta-se a um broker MQTT, assina um tópico e processa mensagens recebidas.
/// </summary>
public class MqttService : IHostedService
{
    private readonly IMqttClient _mqttClient; // Cliente MQTT
    private const string mqttServer = "broker.hivemq.com"; // Endereço do broker MQTT
    private const int mqttPort = 1883; // Porta do broker MQTT
    private const string topic = "weightScale/data"; // Tópico MQTT para assinatura

    /// <summary>
    /// Construtor da classe MqttService. Inicializa o cliente MQTT.
    /// </summary>
    public MqttService()
    {
        var factory = new MqttFactory();
        _mqttClient = factory.CreateMqttClient();
    }

    /// <summary>
    /// Inicia o serviço e conecta ao broker MQTT.
    /// </summary>
    /// <param name="cancellationToken">Token de cancelamento para gerenciar o ciclo de vida do serviço.</param>
    public async Task StartAsync(CancellationToken cancellationToken)
    {
        var options = new MqttClientOptionsBuilder()
            .WithTcpServer(mqttServer, mqttPort) // Configura o endereço e a porta do broker
            .Build();

        // Evento disparado ao conectar ao broker
        _mqttClient.ConnectedAsync += async e =>
        {
            Console.WriteLine("Conectado ao broker MQTT"); // Log de conexão
            await _mqttClient.SubscribeAsync(topic); // Assina o tópico MQTT
            Console.WriteLine($"Inscrito no tópico: {topic}"); // Log de assinatura
        };

        // Evento disparado ao receber mensagens
        _mqttClient.ApplicationMessageReceivedAsync += e =>
        {
            var jsonPayload = Encoding.UTF8.GetString(e.ApplicationMessage.PayloadSegment); // Decodifica a mensagem recebida
            Console.WriteLine("Mensagem recebida: " + jsonPayload); // Log da mensagem recebida

            try
            {
                // Desserializa o JSON em um objeto do tipo SeuObjeto
                var data = JsonSerializer.Deserialize<SeuObjeto>(jsonPayload);
                // Processa o objeto `data` conforme necessário
            }
            catch (Exception ex)
            {
                Console.WriteLine("Erro ao desserializar JSON: " + ex.Message); // Log de erro ao desserializar
            }

            return Task.CompletedTask;
        };

        await _mqttClient.ConnectAsync(options, cancellationToken); // Conecta ao broker MQTT
    }

    /// <summary>
    /// Finaliza o serviço e desconecta do broker MQTT.
    /// </summary>
    /// <param name="cancellationToken">Token de cancelamento para gerenciar o ciclo de vida do serviço.</param>
    public async Task StopAsync(CancellationToken cancellationToken)
    {
        await _mqttClient.DisconnectAsync(); // Desconecta do broker MQTT
        Console.WriteLine("Desconectado do broker MQTT"); // Log de desconexão
    }
}

/// <summary>
/// Classe que representa a estrutura do JSON recebido via MQTT.
/// </summary>
public class SeuObjeto
{
    /// <summary>
    /// ID do dispositivo que enviou os dados.
    /// </summary>
    public string DeviceID { get; set; } = string.Empty; // Inicialização padrão para evitar nulabilidade

    /// <summary>
    /// Nível da bateria do dispositivo.
    /// </summary>
    public float BatteryLevel { get; set; }

    /// <summary>
    /// Carga (ou peso) enviada pelo dispositivo.
    /// </summary>
    public float Charge { get; set; }

    // Adicione outras propriedades conforme o JSON recebido
}

/// <summary>
/// Classe principal do programa para iniciar o host da aplicação.
/// </summary>
namespace Startup // Certifique-se de que o namespace corresponde ao seu projeto
{
    public class Program
    {
        /// <summary>
        /// Ponto de entrada do programa.
        /// </summary>
        /// <param name="args">Argumentos da linha de comando.</param>
        public static void Main(string[] args)
        {
            CreateHostBuilder(args).Build().Run(); // Cria e executa o host da aplicação
        }

        /// <summary>
        /// Configura o builder do host da aplicação.
        /// </summary>
        /// <param name="args">Argumentos da linha de comando.</param>
        /// <returns>Um IHostBuilder configurado.</returns>
        public static IHostBuilder CreateHostBuilder(string[] args) =>
            Host.CreateDefaultBuilder(args)
                .ConfigureWebHostDefaults(webBuilder =>
                {
                    webBuilder.UseStartup<Startup>(); // Configura a classe Startup para o host
                });
    }
}
