#include <iostream>
#include <string>

#include <tcp_client.h>

using namespace std;

int main(void)
{
	try
	{
		// Create a TCP/IP socket and connect it to the server's address and port
		TcpSocket client_socket = TcpClient::connect("localhost", 9090);

		// Send data to the server
		string message = "Hello from client!";
		cout << "Sending: " << message << endl;
		client_socket.send(message);

		// Look for the response
		string response = client_socket.receive(1024);
		cout << "Received: " << response << endl;

		// The socket is closed when client_socket goes out of scope
	}
	catch(const SocketException& e)
	{
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	return 0;
}
