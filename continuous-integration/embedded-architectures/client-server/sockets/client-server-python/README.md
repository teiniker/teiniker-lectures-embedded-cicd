# Socket Programming in Python

Python's standard library contains the module **`socket`**, a thin wrapper
around the BSD socket API of the operating system. The function names
(`socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `send()`,
`recv()`, `close()`) are the same as in C, so the Python examples map
directly to the C and C++ implementations in `../client-server-c` and
`../client-server-cxx`.

No installation is needed, we simply import the module:

```python
import socket
```

A socket is created with two constants:

* `socket.AF_INET`: address family IPv4, an address is a tuple
  `(host, port)`, e.g. `('localhost', 9090)`.
* `socket.SOCK_STREAM`: a stream socket, which means **TCP**
  (connection oriented, reliable, ordered byte stream).

Sockets transfer **bytes**, not strings. Therefore, strings are converted
with `encode('utf-8')` before sending and with `decode('utf-8')` after
receiving.

Client and server use the following simple protocol:

* Address: `localhost:9090` (IPv4, TCP)
* Client sends `"Hello from client!"` (UTF-8 bytes, no terminator)
* Server replies `"Hello from server!"` and closes the connection
* Each side reads at most 1024 bytes


## Server-Side

The server in `server.py` goes through the typical sequence of a TCP
server: `socket()`, `bind()`, `listen()` and then `accept()` in a loop.

```python
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
```

We create a TCP/IP socket. The option `SO_REUSEADDR` allows us to restart
the server immediately. Without it, the port stays blocked for a while
after the server has been stopped and `bind()` fails with
`Address already in use`.

```python
server_address = ('localhost', 9090)
server_socket.bind(server_address)
server_socket.listen(10)
```

* `bind()` assigns the local address and port to the socket. Using
  `localhost` means that only clients on the same machine can connect.
  To accept connections from other machines, use `''` or `'0.0.0.0'`
  (all network interfaces).

* `listen()` turns the socket into a **listening socket**. The argument
  is the backlog, the maximum number of pending connections which are
  queued until the server accepts them.

```python
while True:
    client_socket, client_address = server_socket.accept()
    try:
        print(f"Connection from {client_address}")
        data = client_socket.recv(1024).decode('utf-8')
        if data:
            print(f"Received: {data}")
            response = "Hello from server!"
            client_socket.send(response.encode('utf-8'))
    finally:
        client_socket.close()
```

* `accept()` blocks until a client connects. It returns a **new socket**
  for the communication with this client and the client's address
  `(ip, port)`. The listening socket remains open for further clients.

* `recv(1024)` blocks until data arrives and returns at most 1024 bytes.
  An empty result means that the client has closed the connection.

* `send()` sends the response. Note that `send()` may send fewer bytes
  than given and returns the number of bytes sent. For larger messages,
  `sendall()` should be used.

* The `finally` block ensures that the client socket is closed, even if
  an exception occurs.

The server handles one client after the other (**iterative server**).
While it talks to one client, other clients wait in the backlog queue.

### Running the Server

```
$ python server.py
Server listening on localhost:9090...
```

The server runs until it is stopped with `Ctrl+C`. For each client, it
prints the client's address and the received message:

```
Connection from ('127.0.0.1', 50574)
Received: Hello from client!
```


## Client-Side

The client in `client.py` needs only two steps to establish a
connection: `socket()` and `connect()`.

```python
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('localhost', 9090)
client_socket.connect(server_address)
```

`connect()` establishes the TCP connection to the server (three-way
handshake). The operating system assigns a free local port to the client
socket automatically, which is the port the server prints as part of
`client_address`. If no server is listening, `connect()` raises a
`ConnectionRefusedError`.

```python
try:
    message = "Hello from client!"
    print(f"Sending: {message}")
    client_socket.send(message.encode('utf-8'))

    response = client_socket.recv(1024).decode('utf-8')
    print(f"Received: {response}")
finally:
    client_socket.close()
```

The client sends its message, waits for the response with `recv()` and
finally closes the socket.

### Running the Client

The server must be started first. Then we run the client in a second
terminal:

```
$ python client.py
Sending: Hello from client!
Received: Hello from server!
```

Because all implementations use the same protocol, the Python client also
works with the C and C++ servers, and vice versa.


_Egon Teiniker, 2025-2026, GPL v3.0_
