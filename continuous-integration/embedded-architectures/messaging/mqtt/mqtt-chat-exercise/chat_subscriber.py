import datetime
import paho.mqtt.client as mqtt

# mosquitto_pub -h 127.0.0.1 -t mqtt/chat -m "Hello everybody!"
BROKER = "127.0.0.1"
PORT = 1883
TOPIC = "mqtt/chat"

# TODO: implement MQTT chat subscriber logic here
