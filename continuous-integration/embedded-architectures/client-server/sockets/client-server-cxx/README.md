# TCP Client-Server in C++

C++ implementation of the Python socket example in `../client-server`.
Both implementations use the same wire protocol, so C++ and Python
clients and servers can be mixed freely:

* Address: `localhost:9090` (IPv4, TCP)
* Client sends `"Hello from client!"` (UTF-8 bytes, no terminator)
* Server replies `"Hello from server!"` and closes the connection
* Each side reads at most 1024 bytes


## Structure

```
include/
    tcp_socket.h    RAII wrapper for a connected socket (send, receive, close)
    tcp_server.h    socket + bind + listen, accept() returns a TcpSocket
    tcp_client.h    socket + connect, returns a TcpSocket
src/
    tcp_*.cpp       library "sockets"
    server.cpp      equivalent of server.py
    client.cpp      equivalent of client.py
```

## Build and Run Client and Server

```
$ mkdir build && cd build
$ cmake ..
$ make
```

```
$ ./src/server                          # or: python3 ../../client-server/server.py
$ ./src/client                          # or: python3 ../../client-server/client.py
```

_Example:_ C++ server with a Python client
```
$ ./src/server
Server listening on localhost:9090...
Connection from ('127.0.0.1', 50574)
Received: Hello from client!
```
```
$ python3 ../../client-server/client.py
Sending: Hello from client!
Received: Hello from server!
```

Both servers set `SO_REUSEADDR`, so you can stop one server and start the 
other one immediately without getting `Address already in use`.

_Egon Teiniker, 2025-2026, GPL v3.0_   