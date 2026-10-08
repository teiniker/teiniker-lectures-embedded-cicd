#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <tcp_socket.h>

using namespace std;

TcpSocket::~TcpSocket(void)
{
	close();
}

TcpSocket::TcpSocket(TcpSocket&& other) noexcept : _fd{other._fd}
{
	other._fd = -1;
}

TcpSocket& TcpSocket::operator=(TcpSocket&& other) noexcept
{
	if(this != &other)
	{
		close();
		_fd = other._fd;
		other._fd = -1;
	}
	return *this;
}

void TcpSocket::send(const string& data)
{
	size_t sent = 0;
	while(sent < data.size())
	{
		ssize_t n = ::send(_fd, data.data() + sent, data.size() - sent, MSG_NOSIGNAL);
		if(n < 0)
		{
			if(errno == EINTR)
				continue;
			throw SocketException(string("send() failed: ") + strerror(errno));
		}
		sent += static_cast<size_t>(n);
	}
}

string TcpSocket::receive(size_t max_size)
{
	string buffer(max_size, '\0');
	ssize_t n;
	do
	{
		n = ::recv(_fd, buffer.data(), max_size, 0);
	} while(n < 0 && errno == EINTR);

	if(n < 0)
		throw SocketException(string("recv() failed: ") + strerror(errno));

	buffer.resize(static_cast<size_t>(n));
	return buffer;
}

string TcpSocket::peerAddress(void) const
{
	sockaddr_in addr{};
	socklen_t len = sizeof(addr);
	if(::getpeername(_fd, reinterpret_cast<sockaddr*>(&addr), &len) < 0)
		throw SocketException(string("getpeername() failed: ") + strerror(errno));

	char host[INET_ADDRSTRLEN];
	::inet_ntop(AF_INET, &addr.sin_addr, host, sizeof(host));
	return "('" + string(host) + "', " + to_string(ntohs(addr.sin_port)) + ")";
}

void TcpSocket::close(void)
{
	if(_fd >= 0)
	{
		::close(_fd);
		_fd = -1;
	}
}
