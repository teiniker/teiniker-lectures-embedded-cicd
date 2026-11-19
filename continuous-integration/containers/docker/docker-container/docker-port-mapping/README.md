# Docker Port Mapping

Docker port mapping is the mechanism which makes **network services
running inside a container reachable from outside the container**.

By default, each container is connected to a private virtual network
(the `bridge` network) and gets its own IP address in this network,
for example `172.17.0.3`. A server inside the container, like a web
server on port 80, listens on this private address. The host can reach
it, but:
* the address changes with every new container,
* it can't be reached from other machines in our network,
* `localhost` on the host is **not** `localhost` inside the container.

Port mapping (also called **publishing a port**) solves that problem by
forwarding a port of the host to a port of the container. It is
configured with the `-p` flag when the container is created:

```bash
-p <host-port>:<container-port>
```

For example, `-p 8080:80` forwards port 8080 of the host to port 80 of
the container. Clients connect to the host, and Docker passes the
connection on to the container.


## Without Port Mapping

We start an Nginx web server **without** the `-p` flag:

```bash
$ docker run -d --name web nginx

$ docker container ls
CONTAINER ID   IMAGE   COMMAND                  CREATED         STATUS         PORTS     NAMES
3f1c2a9e8b7d   nginx   "/docker-entrypoint.…"   5 seconds ago   Up 4 seconds   80/tcp    web
```

The `PORTS` column shows `80/tcp`: the container uses port 80, but there
is **no arrow** (`->`), so the port is not published on the host.
The `80/tcp` comes from the `EXPOSE 80` instruction in the Dockerfile of
the `nginx` image:

```bash
$ docker image inspect nginx --format '{{json .Config.ExposedPorts}}'
{"80/tcp":{}}
```

Note that `EXPOSE` **only documents** which ports the application uses.
It does not publish any port.

Therefore, the web server can't be reached via `localhost`:

```bash
$ curl http://localhost:80
curl: (7) Failed to connect to localhost port 80 after 0 ms: Could not connect to server
```

We can find the IP address of the container in the `bridge` network:

```bash
$ docker container inspect web --format '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}'
172.17.0.3

$ curl http://172.17.0.3:80
<!DOCTYPE html>
<html>
<head>
<title>Welcome to nginx!</title>
...
```

On a Linux host, this works because the host is also connected to the
`bridge` network (via the `docker0` interface, address `172.17.0.1`).
But it is not practical: the IP address is assigned dynamically, and
other machines can't reach this network at all.

```bash
$ docker rm -f web
```


## Publishing a Port

Now we start the web server with the port mapping `-p 8080:80`:

```bash
$ docker run -d --name web1 -p 8080:80 nginx

$ docker container ls
CONTAINER ID   IMAGE   COMMAND                  CREATED         STATUS         PORTS                                     NAMES
8a38dbf20ee7   nginx   "/docker-entrypoint.…"   5 seconds ago   Up 4 seconds   0.0.0.0:8080->80/tcp, [::]:8080->80/tcp   web1
```

The `PORTS` column now shows `0.0.0.0:8080->80/tcp`: port 8080 on all
IPv4 addresses of the host (`0.0.0.0`) is forwarded to port 80/tcp of
the container. `[::]:8080` is the same for IPv6.

The `docker port` command lists the mappings of a container:

```bash
$ docker port web1
80/tcp -> 0.0.0.0:8080
80/tcp -> [::]:8080
```

Now we can access the web server via the host port:

```bash
$ curl -I http://localhost:8080
HTTP/1.1 200 OK
Server: nginx/1.31.9
...
```

The mapping is also stored in the container configuration:

```bash
$ docker container inspect web1 --format '{{json .NetworkSettings.Ports}}'
{"80/tcp":[{"HostIp":"0.0.0.0","HostPort":"8080"},{"HostIp":"::","HostPort":"8080"}]}
```

### How Does It Work?

On the host, port 8080 is now open:

```bash
$ ss -ltn | grep 8080
LISTEN 0      4096         0.0.0.0:8080       0.0.0.0:*
LISTEN 0      4096            [::]:8080          [::]:*
```

Docker implements the forwarding in two ways:
* **Firewall rules** (`iptables` or `nftables`): Network packets which
    arrive at port 8080 of the host are redirected (NAT) to the IP
    address and port of the container. This is the fast path used for
    connections from other machines.
* **docker-proxy**: A small user space process per mapping, which
    accepts connections on the host port and forwards them to the
    container. It is used, for example, for connections to `localhost`:

```bash
$ ps -eo pid,args | grep docker-proxy
  25641 /usr/bin/docker-proxy -proto tcp -host-ip 0.0.0.0 -host-port 8080 -container-ip 172.17.0.3 -container-port 80 ...
  25649 /usr/bin/docker-proxy -proto tcp -host-ip :: -host-port 8080 -container-ip 172.17.0.3 -container-port 80 ...
```

Note that the forwarding targets the container IP `172.17.0.3`, which we
found in the previous section.


## Multiple Containers

Every container has its own network namespace and therefore its **own
port 80**. So we can run several web servers in parallel, as long as we
map them to **different host ports**:

```bash
$ docker run -d --name web2 -p 8081:80 nginx

$ docker container ls
CONTAINER ID   IMAGE   COMMAND                  CREATED          STATUS          PORTS                                     NAMES
b2d4e6f8a0c1   nginx   "/docker-entrypoint.…"   3 seconds ago    Up 2 seconds    0.0.0.0:8081->80/tcp, [::]:8081->80/tcp   web2
8a38dbf20ee7   nginx   "/docker-entrypoint.…"   2 minutes ago    Up 2 minutes    0.0.0.0:8080->80/tcp, [::]:8080->80/tcp   web1
```

A host port can only be used **once**. If we try to map port 8080 a
second time, the container is created, but it can't be started:

```bash
$ docker run -d --name web3 -p 8080:80 nginx
docker: Error response from daemon: failed to set up container networking: driver failed
programming external connectivity on endpoint web3 (...): Bind for 0.0.0.0:8080 failed:
port is already allocated

$ docker container ls -a --filter name=web3
CONTAINER ID   IMAGE   COMMAND                  CREATED          STATUS    PORTS     NAMES
c3e5f7a9b1d2   nginx   "/docker-entrypoint.…"   10 seconds ago   Created             web3
```

The same happens if a program on the host (not a container) already uses
the port. Then the message ends with `address already in use`. In both
cases, we choose another host port.

```bash
$ docker rm -f web2 web3
```


## Publishing to a Specific Interface

By default, a published port is reachable on **all** network interfaces
of the host (`0.0.0.0`). So also other machines in our network can
access it. This is what we want for a server, but not for a database
used during development.

We can restrict the mapping to one IP address of the host:

```bash
-p <host-ip>:<host-port>:<container-port>
```

With `127.0.0.1`, the port is only reachable from the host itself:

```bash
$ docker run -d --name web4 -p 127.0.0.1:8082:80 nginx

$ docker port web4
80/tcp -> 127.0.0.1:8082
```

We compare the access via `127.0.0.1` and via the IP address of the
host in the local network (here `10.0.2.15`):

```bash
$ hostname -I
10.0.2.15 ...

$ curl -s -o /dev/null -w "%{http_code}\n" http://127.0.0.1:8082
200

$ curl http://10.0.2.15:8082
curl: (7) Failed to connect to 10.0.2.15 port 8082 after 0 ms: Could not connect to server

$ curl -s -o /dev/null -w "%{http_code}\n" http://10.0.2.15:8080
200
```

The container `web4` is only reachable via `127.0.0.1`, while `web1`
(published to `0.0.0.0`) is reachable via both addresses.

```bash
$ docker rm -f web4
```


## Random Host Ports

If we only specify the container port, Docker chooses a free host port:

```bash
$ docker run -d --name web5 -p 80 nginx

$ docker port web5 80
0.0.0.0:32768
[::]:32768
```

With the `-P` (`--publish-all`) flag, Docker publishes **all ports
declared with `EXPOSE`** in the image to random host ports:

```bash
$ docker run -d --name web6 -P nginx

$ docker port web6
80/tcp -> 0.0.0.0:32769
80/tcp -> [::]:32769

$ curl -s -o /dev/null -w "%{http_code}\n" http://localhost:32769
200
```

Random ports avoid conflicts, for example when tests start several
containers in parallel on a CI server. The test then asks Docker for the
port with `docker port`.

```bash
$ docker rm -f web1 web5 web6
```


## TCP and UDP

Without a protocol, a mapping is for **TCP**. Services which use UDP
(like DNS or some IoT protocols) need the `/udp` suffix:

```bash
$ docker run -d --name udp -p 5000:5000/udp nginx

$ docker container ls --filter name=udp --format '{{.Ports}}'
80/tcp, 0.0.0.0:5000->5000/udp, [::]:5000->5000/udp

$ docker rm -f udp
```

To publish both protocols, we use two `-p` flags, for example
`-p 5000:5000/tcp -p 5000:5000/udp`.


## Port Mapping in Docker Compose

In a `docker-compose.yml` file, the port mappings of a service are listed
under `ports`, with the same syntax as the `-p` flag:

```yaml
services:
  web:
    image: nginx
    container_name: web
    ports:
      - "8080:80"
      - "127.0.0.1:8082:80"
```

```bash
$ docker compose ps --format '{{.Name}} {{.Ports}}'
web 0.0.0.0:8080->80/tcp, 127.0.0.1:8082->80/tcp, [::]:8080->80/tcp
```

We should always put the mappings in **quotes**, as recommended by the
Docker documentation. Older YAML parsers (YAML 1.1) read unquoted values
like `22:22` as a number in base 60, which leads to surprising errors.

Note that containers in the **same** Docker Compose project don't need
port mappings to talk to each other. They are connected to a common
network and use the container port directly, with the service name as
host name (for example `mariadb:3306`). Port mappings are only needed
for clients **outside** of Docker, like a browser or `curl` on the host.


## Summary

| Option                     | Meaning                                       |
|----------------------------|-----------------------------------------------|
| (none)                     | Port not reachable via the host               |
| `EXPOSE 80` (Dockerfile)   | Documents the port, publishes nothing         |
| `-p 8080:80`               | Host port 8080 (all interfaces) to port 80    |
| `-p 127.0.0.1:8080:80`     | Same, but only reachable from the host itself |
| `-p 80`                    | Random host port to port 80                   |
| `-P`                       | All `EXPOSE` ports to random host ports       |
| `-p 5000:5000/udp`         | UDP instead of TCP                            |


## References

* [Docker: Port publishing and mapping](https://docs.docker.com/engine/network/port-publishing/)
* [Docker: docker container port](https://docs.docker.com/reference/cli/docker/container/port/)
* [Docker Compose: ports](https://docs.docker.com/reference/compose-file/services/#ports)
* [DockerHub: nginx](https://hub.docker.com/_/nginx)

*Egon Teiniker, 2025-2026, GPL v3.0*
