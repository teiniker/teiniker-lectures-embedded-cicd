#include <stdio.h>
#include <string.h>

#include <tcp_client.h>

#define BUFFER_SIZE 1024

int main(void)
{
	// Create a TCP/IP socket and connect it to the server's address and port
	int client_socket = tcp_client_connect("localhost", 9090);
	if(client_socket < 0)
	{
		fprintf(stderr, "Error: %s\n", tcp_error());
		return 1;
	}

	// Send data to the server
	const char* message = "Hello from client!";
	printf("Sending: %s\n", message);
	if(tcp_send(client_socket, message, strlen(message)) < 0)
		goto error;

	// Look for the response (+1 for the string terminator)
	char response[BUFFER_SIZE + 1];
	ssize_t n = tcp_receive(client_socket, response, BUFFER_SIZE);
	if(n < 0)
		goto error;
	response[n] = '\0';
	printf("Received: %s\n", response);

	tcp_close(client_socket);
	return 0;

error:
	fprintf(stderr, "Error: %s\n", tcp_error());
	tcp_close(client_socket);
	return 1;
}
