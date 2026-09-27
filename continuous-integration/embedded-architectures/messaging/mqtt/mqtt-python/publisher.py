import paho.mqtt.client as mqtt

# mosquitto_pub -h 127.0.0.1 -t sensors/distance -m "10cm" 
BROKER = "127.0.0.1"
PORT = 1883

def on_connect(client, userdata, flags, rc):
    print("Connected with result code:", rc)

client = mqtt.Client()
client.on_connect = on_connect

client.connect(BROKER, PORT, keepalive=60)

# Publish the message
TOPIC = "sensors/distance"
MESSAGE = "7cm"
client.publish(TOPIC, MESSAGE)
print(f"Published '{MESSAGE}' to topic '{TOPIC}'")

# Disconnect cleanly
client.disconnect()
