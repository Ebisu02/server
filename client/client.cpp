// CLIENT SIDE

#include <iostream>
#include <WinSock2.h>
#include <string>

using namespace std;

#pragma warning(disable: 4996)
#pragma comment(lib,"ws2_32.lib") // Winsock Library

#define SERVER "127.0.0.1" // ip address of UDP server
#define BUFFER_LENGTH 512 // max length of answer
#define PORT 8888 // the port which we listening for incoming data

int main()
{
	system("Title UDP Client");

	// initialise winsock
	WSADATA ws;
	//std::cout << "\nInitialising winsock...";
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
		std::cout << "\nFailed initialising winsock.\nError code: " + WSAGetLastError();
		return 1;
	}
	//std::cout << "\nInitialised";

	// create socket
	sockaddr_in server;
	int client_socket;
	if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) { // <<< UDP SOCKET 
		std::cout << "\nSocket() failed with error code: " + WSAGetLastError();
		return 2;
	}

	// setup address structure
	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.S_un.S_addr = inet_addr(SERVER);

	// start connecting
	while (true) {
		//char message[BUFFER_LENGTH];
		std::cout << "\nEnter a number for value 'i': ";
		//cin.getline(message, BUFFER_LENGTH);
		int iv;
		try {
			std::cin >> iv;
		}
		catch (const std::exception e) {
			std::cerr << e.what();
		}

		for (int i = 0; i < iv; ++i) {
			char message[BUFFER_LENGTH];
			itoa(i, message, 10);
			// send the message
			if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR) {
				std::cout << "\nSendto() failed with error code: " << WSAGetLastError();
				return 3;
			}
		}
		// receive a reply & print it
		// clear the answer by filling null
		// it might have previously received data
		char answer[BUFFER_LENGTH] = {};

		// try to receive some data, this is a blocking call
		int slen = sizeof(sockaddr_in);
		int answer_length;
		if (answer_length = recvfrom(client_socket, answer, BUFFER_LENGTH, 0, (sockaddr*)&server, &slen) == SOCKET_ERROR) {
			std::cout << "\nRecvfrom() failed with error code: " << WSAGetLastError();
			exit(0);
		}
		std::cout << "\n" << answer << "\n";
	}
	closesocket(client_socket);
	WSACleanup();
}
