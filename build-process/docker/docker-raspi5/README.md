# Docker on Raspberry Pi 5 

## Setup 

Install using the **convenience script**:

```bash
$ cd Downloads/
$ curl -fsSL https://get.docker.com -o get-docker.sh
$ sudo sh get-docker.sh
```

We can verify whether the Docker Engine has been enabled:

```bash
$ sudo systemctl status docker
● docker.service - Docker Application Container Engine
    Loaded: loaded (/lib/systemd/system/docker.service; enabled; preset: enabled)
    Active: active (running) since Sun 2025-11-02 12:14:39 CET; 4min 51s ago
TriggeredBy: ● docker.socket
    Docs: https://docs.docker.com
Main PID: 100174 (dockerd)
    Tasks: 10
        CPU: 247ms
    CGroup: /system.slice/docker.service
            └─100174 /usr/bin/dockerd -H fd:// --containerd=/run/containerd/containerd.sock
```


Finally, grant a **non-root user access** to the Docker Engine:

```bash
# Create the docker group (should already exist)
$ sudo groupadd docker  

# Add our user to the docker group
$ sudo usermod -aG docker $USER    

# Run the following command to activate the changes to groups
$ newgrp docker
```

As a simple test, start a `hello-world` container:

```bash
$ docker run --rm hello-world
```

With Docker now available on the Raspberry Pi 5, we can use the 
**same commands as on the Debian VM** to run containers and build images.


## References

* [Install Docker Engine on Debian](https://docs.docker.com/engine/install/debian/)

*Egon Teiniker, 2025, GPL v3.0*