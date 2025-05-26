# Passo a Passo: Executando o Sistema Completo (Publisher MQTT, API FastAPI e Deploy)

## 1️⃣ Configurar e Rodar o Publicador MQTT (ESP32)

### 📌 Requisitos:
- Placa ESP32
- Sensor de peso **HX711**
- Conexão com Wi-Fi
- Broker MQTT público (usando `test.mosquitto.org`)

### 📌 Como Rodar:
1. **Configure sua rede Wi-Fi** no código `Publisher.cpp`:
   ```cpp
   const char* ssid = "NOME_DA_REDE";  
   const char* password = "SENHA_DO_WIFI";
   ```
2. **Compile e envie o código para o ESP32** (usando Arduino IDE ou PlatformIO).
3. **Abra o Monitor Serial** para verificar se está conectando corretamente ao Wi-Fi e publicando os dados no tópico MQTT `"data/weight"`.

---

## 2️⃣ Rodar a API Localmente (FastAPI + MQTT Subscriber)

### 📌 Requisitos:
- Python 3.8+
- Instalar dependências:
   ```bash
   pip install -r requirements.txt
   ```
- Dependências necessárias (listadas em `requirements.txt`):
  - `fastapi`
  - `paho-mqtt`
  - `uvicorn`

### 📌 Como Rodar:
1. **Execute o servidor FastAPI**:
   ```bash
   uvicorn main:app --host 0.0.0.0 --port 8000
   ```
2. **Verifique se a API está rodando**:
   - Acesse `http://localhost:8000/data` no navegador.
   - Deve retornar o último dado recebido via MQTT.

---

## 3️⃣ Deploy da API no Render

### 📌 Como Publicar a API:
1. **Crie um repositório no GitHub** e adicione os arquivos `main.py`, `mqtt_client.py` e `requirements.txt`.
2. **Acesse o Render** ([https://render.com](https://render.com)) e crie um novo serviço **Web Service**.
3. **Configure o serviço**:
   - Escolha o repositório do GitHub.
   - Linguagem: **Python 3.x**.
   - Comando de inicialização:  
     ```bash
     uvicorn main:app --host 0.0.0.0 --port 10000
     ```
   - Defina a porta do serviço para **10000** ou outra porta permitida.
4. **Finalize e aguarde o deploy**.
5. **Teste o endpoint**:
   - Acesse `https://picking.onrender.com/data` no navegador.

---

## 🔄 Fluxo Completo
1. O **ESP32** publica dados de peso no broker MQTT (`test.mosquitto.org`).
2. O **MQTT Subscriber** (`mqtt_client.py`) recebe e armazena as mensagens.
3. O **FastAPI** expõe esses dados na API via endpoint `/data`.
4. A API é publicada no **Render**, permitindo acesso remoto aos dados.

---

## 🚀 Teste Final
- Acesse `https://picking.onrender.com/data` para verificar se os dados MQTT estão sendo recebidos corretamente.
