# TCP Client-Server in C

C implementation of the socket examples in `../client-server-python`.
All implementations use the same wire protocol, so C, C++ and Python 
clients and servers can be mixed freely:

* Address: `localhost:9090` (IPv4, TCP)
* Client sends `"Hello from client!"` (UTF-8 bytes, no terminator)
* Server replies `"Hello from server!"` and closes the connection
* Each side reads at most 1024 bytes


## Structure

```
include/
    tcp_socket.h    send, receive, peer address and close a connected socket
    tcp_server.h    socket + bind + listen, accept() returns a socket
    tcp_client.h    socket + connect, returns a socket
src/
    tcp_*.c         library "sockets"
    server.c        equivalent of server.py
    client.c        equivalent of client.py
```

## Build and Run Client and Server

```
$ mkdir build && cd build
$ cmake ..
$ make
```

```
$ ./src/server      # or: python ../../client-server-python/server.py
$ ./src/client      # or: python ../../client-server-python/client.py
```

_Example:_ C server with a Python client
```
$ ./src/server
Server listening on localhost:9090...
Connection from ('127.0.0.1', 50574)
Received: Hello from client!
```
```
$ python ../../client-server-python/client.py
Sending: Hello from client!
Received: Hello from server!
```

All servers set `SO_REUSEADDR`, so you can stop one server and start
another one immediately without getting `Address already in use`.

_Egon Teiniker, 2025-2026, GPL v3.0_
