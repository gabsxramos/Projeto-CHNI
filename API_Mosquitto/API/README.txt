Os códigos consistem em um sistema que integra **FastAPI** com **MQTT**, permitindo que um servidor HTTP consulte os 
últimos dados recebidos de um broker MQTT. Aqui está um resumo do que cada arquivo faz:

---------------------------------------------------------------------------------------------------------------------

### `mqtt_client.py` (Cliente MQTT) ###
Este módulo gerencia a comunicação com um broker MQTT. Ele:
- **Se conecta** ao broker MQTT (`test.mosquitto.org` na porta `1883`).
- **Se inscreve** no tópico `"data/weight"` para receber mensagens.
- **Armazena** a última mensagem recebida junto com o timestamp de quando foi recebida.
- **Roda em uma thread separada**, garantindo que a escuta do MQTT não bloqueie outras operações.

📌 **Pontos-chave:**
- Usa `paho-mqtt` para gerenciar a conexão e escuta.
- A função `on_message` processa as mensagens recebidas e armazena o conteúdo se for JSON válido.
- A função `get_latest_message()` retorna a última mensagem e o timestamp correspondente.

---------------------------------------------------------------------------------------------------------------------

### 2. `main.py` (API FastAPI) ###
Este arquivo cria uma **API com FastAPI** para expor os dados do MQTT através de um endpoint HTTP. Ele:
- **Inicia um servidor FastAPI**.
- **Inicializa o cliente MQTT** (`mqtt_client = MQTTClient()`) e o executa em segundo plano (`mqtt_client.start()`).
- **Cria um endpoint `GET /data`**, que retorna a última mensagem JSON recebida do MQTT.

📌 **Pontos-chave:**
- O servidor é assíncrono (`async def get_data()`).
- A API retorna `{"data": último JSON recebido}` ao ser acessada.

---------------------------------------------------------------------------------------------------------------------

### 3. `requirements.txt` ###
Lista as dependências do projeto:
- `fastapi` → Framework web para criar APIs.
- `paho-mqtt` → Biblioteca MQTT para comunicação com o broker.
- `uvicorn` → Servidor ASGI para rodar o FastAPI.

---------------------------------------------------------------------------------------------------------------------

Fluxo de Funcionamento:
1. O `mqtt_client.py` se conecta ao broker e começa a escutar mensagens do tópico `"data/weight"`.
2. Quando uma mensagem chega, ela é armazenada e associada a um timestamp.
3. O `main.py` expõe um endpoint HTTP (`/data`), que retorna a última mensagem MQTT recebida.
4. Você pode acessar `http://localhost:8000/data` (se estiver rodando localmente) para obter o último dado recebido.

