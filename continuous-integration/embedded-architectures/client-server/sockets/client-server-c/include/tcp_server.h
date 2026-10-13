#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdint.h>
#include <tcp_socket.h>

// Create a TCP/IP socket, bind it to host:port and listen.
// Port 0 lets the operating system choose a free port.
// Returns the server socket or -1 on error.
int tcp_server_create(const char* host, uint16_t port, int backlog);

// Wait for a connection. Returns the connected socket or -1 on error.
int tcp_server_accept(int server_fd);

// The port the server is actually bound to (or -1 on error)
int tcp_server_port(int server_fd);

#endif /* TCP_SERVER_H */
