#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <stddef.h>
#include <sys/types.h>

// Functions working on a connected TCP socket (file descriptor).
// All functions return -1 on error, tcp_error() then describes the cause.

// Description of the last error, e.g. "connect() failed: Connection refused"
const char* tcp_error(void);

// Send all len bytes of data. Returns 0 on success.
int tcp_send(int fd, const char* data, size_t len);

// Receive up to max_size bytes into buffer. Returns the number of bytes
// received (0 = peer closed the connection). The buffer is not
// null-terminated.
ssize_t tcp_receive(int fd, char* buffer, size_t max_size);

// Remote address as ('host', port) - same format as Python
int tcp_peer_address(int fd, char* buffer, size_t size);

void tcp_close(int fd);

#endif /* TCP_SOCKET_H */
