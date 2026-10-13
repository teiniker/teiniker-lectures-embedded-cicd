#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <tcp_server.h>
#include "tcp_error.h"

int tcp_server_create(const char* host, uint16_t port, int backlog)
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

	// Allow restarting the server immediately (no "Address already in use")
	int on = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		tcp_set_error("setsockopt()", strerror(errno));
		freeaddrinfo(result);
		close(fd);
		return -1;
	}

	// Bind the socket to a local address and a port
	rc = bind(fd, result->ai_addr, result->ai_addrlen);
	freeaddrinfo(result);
	if(rc < 0)
	{
		tcp_set_error("bind()", strerror(errno));
		close(fd);
		return -1;
	}

	// Listen for incoming connections (backlog = queue size)
	if(listen(fd, backlog) < 0)
	{
		tcp_set_error("listen()", strerror(errno));
		close(fd);
		return -1;
	}
	return fd;
}

int tcp_server_accept(int server_fd)
{
	int client_fd;
	do
	{
		client_fd = accept(server_fd, NULL, NULL);
	} while(client_fd < 0 && errno == EINTR);

	if(client_fd < 0)
		tcp_set_error("accept()", strerror(errno));
	return client_fd;
}

int tcp_server_port(int server_fd)
{
	struct sockaddr_in addr = {0};
	socklen_t len = sizeof(addr);
	if(getsockname(server_fd, (struct sockaddr*)&addr, &len) < 0)
	{
		tcp_set_error("getsockname()", strerror(errno));
		return -1;
	}
	return ntohs(addr.sin_port);
}
