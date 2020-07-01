#include <stdio.h>
#include <winsock2.h>
#pragma warning(disable:4996)

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc, char* argv[])
{
	WSADATA wsa;
	int retCode = 0;

	// Initierar Winsock Version 2.2
	retCode = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (retCode != 0) {
		printf("WSAStartup failed to initialize, error %d\n", retCode);
	}
	else {
		printf("The Winsock .dll was found!\n");
		printf("Current status: %s.\n", wsa.szSystemStatus);
	}

	// Create a socket called m_socket;
	SOCKET m_socket;

	// Call the socket function and return its value to the m_socket variable
	// Using Internet address family, streaming sockets, and the TCP/IP protocol.
	// Using AF_INET family, TCP socket type and protocol of the AF_INET - IPv4
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Check for errors to ensure that the socket is a valid socket
	if (m_socket == INVALID_SOCKET) {
		printf("Error at m_socket: %ld\n", WSAGetLastError());
	}
	else
		printf("m_socket is OK!\n");

	// bind()
	// Create a sockaddr_in object and set its values.
	SOCKADDR_IN service;

	// AF_INET is the Internet address family.
	service.sin_family = AF_INET;
	// "127.0.0.1" is the local IP address to which the socket will be bound.
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	// 55555 is the port number to wich the socket will be bound.
	service.sin_port = htons(55555);

	// Call the bind function, passing the created socket and the sockaddr_in structure as parameters.
	// Check for general errors.
	if (bind(m_socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		printf("bind() failed, %ld.\n", WSAGetLastError());
	}
	else
		printf("bind() was successfull!\n");

	// Call the listen function, passing the created socket and the maximum number of
	// allowed connections to accept as parameters. Check for general errors.

	if (listen(m_socket, 1) == SOCKET_ERROR) {
		printf("listen() failed, %ld.\n", WSAGetLastError());
	}
	else
		printf("listen() is OK, waiting for connections...\n");

    // Connect to a server.
    SOCKADDR_IN clientService;

    clientService.sin_family = AF_INET;

    // Just test using the localhost, you can try other IP address
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(55555);

    if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {

        printf("Client: connect() - Failed to connect.\n");
    } 
	else {
        printf("Client: connect() is OK.\n");
        printf("Client: Can start sending and receiving data...\n");
    }

    // Send and receive data.
    int bytesSent;
    int bytesRecv = SOCKET_ERROR;

    // Be careful with the array bound, provide some checking mechanism...
    char sendbuf[200] = "This is a test string from client";
    char recvbuf[200] = "";



    // Receives some test string to server...
	while (bytesRecv == SOCKET_ERROR) {
		bytesRecv = recv(m_socket, recvbuf, 200, 0);
		if (bytesRecv == 0 || bytesRecv == WSAECONNRESET) {
			printf("Client: Connection Closed.\n");
			break;
		}

		if (bytesRecv > 0) {
			printf("Client: recv() is OK.\n");
			printf("Client: Received data is: \"%s\"\n", recvbuf);
			printf("Client: Bytes received is: %ld.\n", bytesRecv);
		}
	}
    // Sends some test data to server...
    bytesSent = send(m_socket, sendbuf, strlen(sendbuf), 0);

	if (bytesSent == SOCKET_ERROR) {
		printf("Client: send() error %ld.\n", WSAGetLastError());
	}
    else {
        printf("Client: send() is OK - Bytes sent: %ld\n", bytesSent);
        printf("Client: The test string sent: \"%s\"\n", sendbuf);
    }
	closesocket(m_socket);
	WSACleanup();

	return 0;
}