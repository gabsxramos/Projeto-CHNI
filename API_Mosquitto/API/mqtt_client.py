import threading
import paho.mqtt.client as mqtt
from datetime import datetime

class MQTTClient:
    def __init__(self, broker="test.mosquitto.org", port=1883, topic="data/weight"):
        self.broker = broker
        self.port = port
        self.topic = topic
        self.client = mqtt.Client()
        self.latest_message = None
        self.timestamp = None

        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

    def on_connect(self, client, userdata, flags, rc):
        print(f"Connected to MQTT broker {self.broker} with result code {rc}")
        self.client.subscribe(self.topic)

    def on_message(self, client, userdata, msg):
        try:
            import json
            decoded_message = json.loads(msg.payload.decode())
            self.latest_message = decoded_message
            self.timestamp = datetime.now().isoformat()  # Registra o momento do recebimento
            print(f"Valid message received at {self.timestamp}: {self.latest_message}")
        except json.JSONDecodeError:
            print("Received invalid JSON, ignoring...")

    def start(self):
        thread = threading.Thread(target=self._run)
        thread.daemon = True
        thread.start()

    def _run(self):
        self.client.connect(self.broker, self.port, 60)
        self.client.loop_forever()

    def get_latest_message(self):
        """
        Retorna a última mensagem válida e o momento em que foi recebida.
        """
        return self.latest_message, self.timestamp
