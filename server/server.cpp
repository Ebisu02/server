#include <iostream>
#include <WinSock2.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib") // Winsock Library
#pragma warning(disable: 4996)

#define SERVER "127.0.0.1" // ip address of UDP server
#define BUFFER_LENGTH 512 // max length of answer
#define PORT 8888 // the port which we listening for incoming data

int main()
{
	system("Title UDP Server");
	sockaddr_in server, client;

	// Initialise winsock
	WSADATA wsa;
	std::cout << "\nInitialising Winsock...";
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cout << "\nFailed. Error code: " << WSAGetLastError();
		exit(0);
	}
	std::cout << "\nInitialised.\n";

	// Create a socket
	SOCKET server_socket;
	if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		std::cout << "\nCould not create socket: " << WSAGetLastError();
	}
	std::cout << "\nSocket created.\n";

	// Preparing sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	// Binding
	if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		std::cout << "\nBind failed with error code: " << WSAGetLastError();
		exit(EXIT_FAILURE);
	}
	puts("Bind done.");

	while (true) {
		std::cout << "\nWaiting for data...\n";
		fflush(stdout);
		char message[BUFFER_LENGTH] = {};

		// Try to receive some data, this is a blocking call
		int message_length;
		int slength = sizeof(sockaddr_in);
		if (message_length = recvfrom(server_socket, message, BUFFER_LENGTH, 0, (sockaddr*)&client, &slength) == SOCKET_ERROR) {
			std::cout << "\nRecvfrom() failed with error code: " << WSAGetLastError();
			exit(0);
		}

		// Print details of the client/peer and the data received
		std::cout << "\nReceived packet from " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << "\n";
		std::cout << "Data: " << message << "\n";

		// Reply the client with the same data
		if (sendto(server_socket, message, strlen(message), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) {
			std::cout << "Sendto() failed with error code: " << WSAGetLastError();
			return 3;
		}
	}
	closesocket(server_socket);
	WSACleanup();
}