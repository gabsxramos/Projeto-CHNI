#include "HX711.h"           // Inclui a biblioteca para o sensor de peso HX711
#include <WiFi.h>            // Inclui a biblioteca para conectividade Wi-Fi
#include <WebServer.h>       // Inclui a biblioteca para criar um servidor web

const int LOADCELL_DOUT_PIN = 16;   // Define o pino DOUT do sensor de peso
const int LOADCELL_SCK_PIN = 4;     // Define o pino SCK do sensor de peso HX711
const float VALUE_1KG = 509610.00;  // Valor médio obtido na calibração para aproximadamente 1 kg
float scaleValue;                   // Variável para armazenar o valor lido da balança

const char* ssid = "Wokwi-GUEST";    // Define o SSID da rede Wi-Fi
const char* password = "";// Define a senha da rede Wi-Fi

struct {
    float load_max = 5000.0;   // Valor máximo de carga
    float load_min = 0.0;      // Valor mínimo de carga
    int sensor_used = 1;       // Sensor usado (não utilizado no código, mas definido na estrutura)
} obj_1;                       // Instancia um objeto da estrutura definida

WebServer server(80);          // Instancia um servidor web na porta 80
HX711 scale;                   // Instancia um objeto do tipo HX711 para o sensor de peso

void handleRoot() {
    scaleValue = (scale.get_value() / VALUE_1KG) * 1000;   // Calcula o valor da balança em gramas

    String html = "<html><body>";   // Inicia a string HTML para a página web
    html = "<p>Analog Value: " + String(scaleValue - 237.0) + "</p>";  // Adiciona o valor analógico da balança à página
    html += "<center><img src='CNHI-IMG'></center>"; // Adiciona uma imagem centralizada (imagem não definida)
    html += "<h1> </h1>";
    html += "<div class='cont'><h1>Dollie 1</h1>";   // Adiciona um cabeçalho e inicia uma div com classe 'cont'

    // Determina o status do Item 1 com base no valor de peso
    if (scaleValue >= obj_1.load_max / 2) {
        html += "<div class='verde'><h4>Item 1 status:</h4><p style='text-align: center;'> Ok</p></div>";
    } else if (scaleValue >= obj_1.load_min) {
        html += "<div class='amarelo'><h4>Item 1 status:</h4><p style='text-align: center;'> Reposicao</p></div>";
    } else if (scaleValue <= obj_1.load_min) {
        html += "<div class='vermelho'><h4>Item 1 status:</h4><p style='text-align: center;'> Baixo-Volume</p></div>";
    }

    // Define status fixos para Item 2 e Item 3
    html += "<div class='amarelo'><h4>Item 2 status:</h4><p style='text-align: center;'> Ok</p></div>";
    html += "<div class='vermelho'><h4>Item 3 status:</h4><p style='text-align: center;'> Ok</p></div></div>";

    // Adiciona o estilo CSS para a página
    html += "<head><style>.cont {border-radius: 25px; border: 2px outset black; background-color: grey; width: 30%; height: 300px; margin-bottom: 50px; margin-left: 35%;}";
    html += ".verde {border-radius: 15px; border: 2px outset black; width: 50%; height: 50px; text-overflow: ellipsis; background-color: green;}";
    html += ".amarelo {border-radius: 15px; border: 2px outset black; width: 50%; height: 50px; text-overflow: ellipsis; background-color: yellow;}";
    html += ".vermelho {border-radius: 15px; border: 2px outset black; width: 50%; height: 50px; text-overflow: ellipsis; background-color: red;}";
    html += "h1 {text-align: center;} h4 {text-align: center; margin: 0;} p { margin: 0; }</style></head>";
    html += "</body></html>";  // Fecha a string HTML

    server.send(200, "text/html", html);   // Envia a resposta HTTP com o conteúdo HTML
}

void setup() {
    Serial.begin(115200);                  // Inicia a comunicação serial a 115200 bps
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);  // Inicializa o HX711 com os pinos definidos

    // Inicia o ponto de acesso Wi-Fi
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();        // Obtém o endereço IP do ponto de acesso
    Serial.print("AP IP address: ");
    Serial.println(IP);                    // Imprime o endereço IP do ponto de acesso na serial

    // Inicia o servidor web
    server.on("/", handleRoot);            // Define o manipulador para a raiz ("/") do servidor web
    server.begin();                        // Inicia o servidor web
    Serial.println("Web server started");  // Imprime mensagem indicando que o servidor web foi iniciado

    scale.set_scale(2280.f);               // Define o fator de calibração da balança
}

void loop() {
    server.handleClient();                 // Processa os clientes HTTP que fazem requisições ao servidor
}

