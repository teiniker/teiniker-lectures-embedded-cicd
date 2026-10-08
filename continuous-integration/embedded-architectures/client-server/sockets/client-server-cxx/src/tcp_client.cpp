#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <tcp_client.h>

using namespace std;

TcpSocket TcpClient::connect(const string& host, uint16_t port)
{
	// Resolve the host name (e.g. "localhost") to an IPv4 address 
	addrinfo hints{};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	addrinfo* result = nullptr;
	int rc = ::getaddrinfo(host.c_str(), to_string(port).c_str(), &hints, &result);
	if(rc != 0)
		throw SocketException("getaddrinfo() failed: " + string(gai_strerror(rc)));

	// Create a TCP/IP socket
	int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
		::freeaddrinfo(result);
		throw SocketException(string("socket() failed: ") + strerror(errno));
	}
	TcpSocket socket(fd);	// closes fd automatically if connect() fails

	// Connect the socket to the server's address and port
	rc = ::connect(fd, result->ai_addr, result->ai_addrlen);
	::freeaddrinfo(result);
	if(rc < 0)
		throw SocketException(string("connect() failed: ") + strerror(errno));

	return socket;
}
