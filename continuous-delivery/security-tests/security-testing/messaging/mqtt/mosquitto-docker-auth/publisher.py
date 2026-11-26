import ssl
import paho.mqtt.client as mqtt

BROKER_HOST = "localhost"
BROKER_PORT = 8883
CA_CERT = "certs/ca.crt"
USERNAME = "homer"
PASSWORD = "Passw0rd"
TOPIC = "sensors/distance"
MESSAGE = "666cm"

def on_connect(client, userdata, flags, rc):
    print(f"[DEBUG] Connected with result code {rc}")

def on_publish(client, userdata, mid):
    print(f"[DEBUG] Message published (mid={mid})")

client = mqtt.Client()

# Set username/password
client.username_pw_set(USERNAME, PASSWORD)

# TLS setup
client.tls_set(ca_certs=CA_CERT,
               certfile=None,
               keyfile=None,
               tls_version=ssl.PROTOCOL_TLS_CLIENT)

client.on_connect = on_connect
client.on_publish = on_publish

print("[DEBUG] Connecting to broker...")
client.connect(BROKER_HOST, BROKER_PORT, keepalive=60)

client.loop_start()

print(f"[DEBUG] Publishing '{MESSAGE}' to '{TOPIC}'...")
client.publish(TOPIC, MESSAGE)

client.loop_stop()
client.disconnect()
print("[DEBUG] Disconnected.")
