#ifndef _TCP_SOCKET_H_
#define _TCP_SOCKET_H_

#include <string>
#include <cstddef>
#include <stdexcept>

class SocketException : public std::runtime_error
{
	public:
		SocketException(const std::string& cause) : std::runtime_error{cause} {}
};

// RAII wrapper for a connected TCP socket (file descriptor).
// The socket is closed when the object goes out of scope.
class TcpSocket 
{
	private:
		int _fd;

	public:
		explicit TcpSocket(int fd) : _fd{fd} {}
		~TcpSocket(void);

		// A socket can be moved but not copied
		TcpSocket(const TcpSocket&) = delete;
		TcpSocket& operator=(const TcpSocket&) = delete;
		TcpSocket(TcpSocket&& other) noexcept;
		TcpSocket& operator=(TcpSocket&& other) noexcept;

		// Send all bytes of the given (UTF-8) string
		void send(const std::string& data);

		// Receive up to max_size bytes (empty string = peer closed the connection)
		std::string receive(std::size_t max_size = 1024);

		// Remote address as ('host', port) - same format as Python
		std::string peerAddress(void) const;

		void close(void);
 };

#endif /* _TCP_SOCKET_H_ */
