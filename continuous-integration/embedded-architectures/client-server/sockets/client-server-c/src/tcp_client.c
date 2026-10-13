#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <tcp_client.h>
#include "tcp_error.h"

int tcp_client_connect(const char* host, uint16_t port)
{
	// Resolve the host name (e.g. "localhost") to an IPv4 address
	struct addrinfo hints = {0};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	struct addrinfo* result = NULL;
	char service[6];
	snprintf(service, sizeof(service), "%u", port);
	int rc = getaddrinfo(host, service, &hints, &result);
	if(rc != 0)
	{
		tcp_set_error("getaddrinfo()", gai_strerror(rc));
		return -1;
	}

	// Create a TCP/IP socket
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
		tcp_set_error("socket()", strerror(errno));
		freeaddrinfo(result);
		return -1;
	}

	// Connect the socket to the server's address and port
	rc = connect(fd, result->ai_addr, result->ai_addrlen);
	freeaddrinfo(result);
	if(rc < 0)
	{
		tcp_set_error("connect()", strerror(errno));
		close(fd);
		return -1;
	}
	return fd;
}
