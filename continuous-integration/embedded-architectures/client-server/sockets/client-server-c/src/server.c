#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <tcp_server.h>

#define BUFFER_SIZE 1024

// Receive the data and send a response
static void handle_client(int client_socket)
{
	char address[64];
	if(tcp_peer_address(client_socket, address, sizeof(address)) < 0)
	{
		fprintf(stderr, "Error: %s\n", tcp_error());
		return;
	}
	printf("Connection from %s\n", address);

	char data[BUFFER_SIZE + 1];		// +1 for the string terminator
	ssize_t n = tcp_receive(client_socket, data, BUFFER_SIZE);
	if(n < 0)
	{
		fprintf(stderr, "Error: %s\n", tcp_error());
		return;
	}
	if(n > 0)
	{
		data[n] = '\0';
		printf("Received: %s\n", data);
		const char response[] = "Hello from server!";
		if(tcp_send(client_socket, response, sizeof(response) - 1) < 0)
			fprintf(stderr, "Error: %s\n", tcp_error());
	}
}

int main(void)
{
	// Create a TCP/IP socket, bind it to a local address and a port and listen
	const char* host = "localhost";
	const uint16_t port = 9090;
	int server_socket = tcp_server_create(host, port, 10);
	if(server_socket < 0)
	{
		fprintf(stderr, "Error: %s\n", tcp_error());
		return 1;
	}
	printf("Server listening on %s:%u...\n", host, port);

	while(true)
	{
		// Wait for a connection
		int client_socket = tcp_server_accept(server_socket);
		if(client_socket < 0)
		{
			fprintf(stderr, "Error: %s\n", tcp_error());
			continue;
		}

		handle_client(client_socket);
		tcp_close(client_socket);
	}
}
