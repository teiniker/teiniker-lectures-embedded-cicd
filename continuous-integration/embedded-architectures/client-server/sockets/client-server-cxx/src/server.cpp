#include <iostream>
#include <string>

#include <tcp_server.h>

using namespace std;

int main(void)
{
	// Create a TCP/IP socket, bind it to a local address and a port and listen
	const string host = "localhost";
	const uint16_t port = 9090;
	TcpServer server(host, port, 10);
	cout << "Server listening on " << host << ":" << port << "..." << endl;

	while(true)
	{
		// Wait for a connection
		TcpSocket client_socket = server.accept();
		try
		{
			cout << "Connection from " << client_socket.peerAddress() << endl;

			// Receive the data and send a response
			string data = client_socket.receive(1024);
			if(!data.empty())
			{
				cout << "Received: " << data << endl;
				string response = "Hello from server!";
				client_socket.send(response);
			}
		}
		catch(const SocketException& e)
		{
			cerr << "Error: " << e.what() << endl;
		}
		// The connection is closed when client_socket goes out of scope
	}
}
