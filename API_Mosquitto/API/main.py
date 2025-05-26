from fastapi import FastAPI
from mqtt_client import MQTTClient

app = FastAPI()

# Inicializando cliente MQTT
mqtt_client = MQTTClient()
mqtt_client.start()

@app.get("/data")
async def get_data():
    """
    Retorna o último JSON recebido do broker MQTT.
    """
    return {"data": mqtt_client.get_latest_message()}
