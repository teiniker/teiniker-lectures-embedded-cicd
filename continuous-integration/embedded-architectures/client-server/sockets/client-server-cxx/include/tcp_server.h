#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <string>
#include <cstdint>
#include <tcp_socket.h>

class TcpServer 
{
	private:
		int _fd;

	public:
		// Create a TCP/IP socket, bind it to host:port and listen.
		// Port 0 lets the operating system choose a free port.
		TcpServer(const std::string& host, uint16_t port, int backlog = 10);
		~TcpServer(void);

		// A TCP server cannot be copied or assigned.
		TcpServer(const TcpServer&) = delete;
		TcpServer& operator=(const TcpServer&) = delete;

		// Wait for a connection
		TcpSocket accept(void);

		// The port the server is actually bound to
		uint16_t port(void) const;
 };

#endif /* _TCP_SERVER_H_ */
