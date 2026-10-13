#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <stdint.h>
#include <tcp_socket.h>

// Create a TCP/IP socket and connect it to the server's address and port.
// Returns the connected socket or -1 on error.
int tcp_client_connect(const char* host, uint16_t port);

#endif /* TCP_CLIENT_H */
