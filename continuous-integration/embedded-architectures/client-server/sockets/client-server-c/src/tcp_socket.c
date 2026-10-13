#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <tcp_socket.h>
#include "tcp_error.h"

static char error_message[256] = "";

void tcp_set_error(const char* call, const char* cause)
{
	snprintf(error_message, sizeof(error_message), "%s failed: %s", call, cause);
}

const char* tcp_error(void)
{
	return error_message;
}

int tcp_send(int fd, const char* data, size_t len)
{
	size_t sent = 0;
	while(sent < len)
	{
		ssize_t n = send(fd, data + sent, len - sent, MSG_NOSIGNAL);
		if(n < 0)
		{
			if(errno == EINTR)
				continue;
			tcp_set_error("send()", strerror(errno));
			return -1;
		}
		sent += (size_t)n;
	}
	return 0;
}

ssize_t tcp_receive(int fd, char* buffer, size_t max_size)
{
	ssize_t n;
	do
	{
		n = recv(fd, buffer, max_size, 0);
	} while(n < 0 && errno == EINTR);

	if(n < 0)
		tcp_set_error("recv()", strerror(errno));
	return n;
}

int tcp_peer_address(int fd, char* buffer, size_t size)
{
	struct sockaddr_in addr = {0};
	socklen_t len = sizeof(addr);
	if(getpeername(fd, (struct sockaddr*)&addr, &len) < 0)
	{
		tcp_set_error("getpeername()", strerror(errno));
		return -1;
	}

	char host[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr.sin_addr, host, sizeof(host));
	snprintf(buffer, size, "('%s', %u)", host, ntohs(addr.sin_port));
	return 0;
}

void tcp_close(int fd)
{
	if(fd >= 0)
		close(fd);
}
