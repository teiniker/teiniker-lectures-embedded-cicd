#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <tcp_server.h>

using namespace std;

TcpServer::TcpServer(const string& host, uint16_t port, int backlog)
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
	_fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(_fd < 0)
	{
		::freeaddrinfo(result);
		throw SocketException(string("socket() failed: ") + strerror(errno));
	}

	// Allow restarting the server immediately (no "Address already in use")
	int on = 1;
	::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	// Bind the socket to a local address and a port
	rc = ::bind(_fd, result->ai_addr, result->ai_addrlen);
	::freeaddrinfo(result);
	if(rc < 0)
	{
		string cause = string("bind() failed: ") + strerror(errno);
		::close(_fd);
		throw SocketException(cause);
	}

	// Listen for incoming connections (backlog = queue size)
	if(::listen(_fd, backlog) < 0)
	{
		string cause = string("listen() failed: ") + strerror(errno);
		::close(_fd);
		throw SocketException(cause);
	}
}

TcpServer::~TcpServer(void)
{
	::close(_fd);
}

TcpSocket TcpServer::accept(void)
{
	int client_fd;
	do
	{
		client_fd = ::accept(_fd, nullptr, nullptr);
	} while(client_fd < 0 && errno == EINTR);

	if(client_fd < 0)
		throw SocketException(string("accept() failed: ") + strerror(errno));

	return TcpSocket(client_fd);
}

uint16_t TcpServer::port(void) const
{
	sockaddr_in addr{};
	socklen_t len = sizeof(addr);
	if(::getsockname(_fd, reinterpret_cast<sockaddr*>(&addr), &len) < 0)
		throw SocketException(string("getsockname() failed: ") + strerror(errno));
	return ntohs(addr.sin_port);
}
