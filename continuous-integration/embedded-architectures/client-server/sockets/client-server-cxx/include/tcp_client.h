#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include <string>
#include <cstdint>
#include <tcp_socket.h>

class TcpClient 
{
	public:
		// Create a TCP/IP socket and connect it to the server's address and port
		static TcpSocket connect(const std::string& host, uint16_t port);
 };

#endif /* _TCP_CLIENT_H_ */
