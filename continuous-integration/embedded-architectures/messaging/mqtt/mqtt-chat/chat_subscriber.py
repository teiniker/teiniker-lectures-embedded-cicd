import datetime
import paho.mqtt.client as mqtt

# mosquitto_pub -h 127.0.0.1 -t mqtt/chat -m "Hello everybody!"
BROKER = "127.0.0.1"
PORT = 1883
TOPIC = "mqtt/chat"

def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code.is_failure:
        print(f"Connection failed: {reason_code}")
        return
    print(f"Connected to {BROKER}:{PORT}")
    # Subscribe here, so the subscription is renewed after a reconnect
    client.subscribe(TOPIC)
    print(f"Listening on topic '{TOPIC}' (press Ctrl+C to quit)")

def on_message(client, userdata, msg):
    timestamp = datetime.datetime.now().strftime("%H:%M:%S")
    text = msg.payload.decode("utf-8", errors="replace")
    print(f"[{timestamp}] {text}")

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT, keepalive=60)

try:
    # Blocking loop to process network traffic and dispatch callbacks
    client.loop_forever()
except KeyboardInterrupt:
    print("\nChat closed.")
    client.disconnect()
