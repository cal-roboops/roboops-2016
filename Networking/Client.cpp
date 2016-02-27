//
// Client.cpp - Version 2
// CPP Project
//
// Created by Andrew Pau on 1/14/2016
// Modified by Mitchell Oleson on 2/8/2016
// 
// Copyright © 2016 Andrew Pau. All rights reserved.
//
// Written for Windows
//

#include "Client.h"

// Client Constructor
Client::Client(char* ip, char* port) {
    server_ip = ip;
    server_port = port;

    printf("Starting Client Setup...\n");

	status = client_connect();
	if (status == -1) {
		exit(1);
	}

    printf("Client Setup Complete!\n");
}

// Client Destructor
Client::~Client() {
	iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("Shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
    }

    // Cleanup
    closesocket(ClientSocket);
    WSACleanup();
}

// Connection Initializer
int Client::client_connect() {
    printf("Setting up client socket...\n");

	// Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return -1;
    }

    ZeroMemory( &host_info, sizeof(host_info) );
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_protocol = IPPROTO_TCP;

    // Resolve the Server address and port
    iResult = getaddrinfo(server_ip, server_port, &host_info, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return -1;
    }

    printf("Client Socket Success!\n");
    printf("Client Connecting...\n");

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ClientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ClientSocket == INVALID_SOCKET) {
            printf("Socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            return -1;
        }

        // Connect to server.
        iResult = connect(ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ClientSocket);
            ClientSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ClientSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return -1;
    }

    printf("Client Connection Success!\n");
    return 0;
}

// Send messages
int Client::client_send(const char* msg) {
	iSendResult = send(ClientSocket, msg, (int) strlen(msg), 0 );
    if (iSendResult == SOCKET_ERROR) {
        printf("Send failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return -1;
    }

    printf("Bytes sent: %d\n", iSendResult);
    return 0;
}

// Recieve messages
int Client::client_receive() {
    ZeroMemory(recvbuf, sizeof(recvbuf));
	iReceiveResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    if (iReceiveResult > 0) {
        printf("Bytes Received: %d\n", iReceiveResult);
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return -1;
    }

    return 0;
}

// Main Method for Command Line
// Sets up intial config and begins 
// ouputing to the terminal
int main(int argc, char **argv) {
    // Validate the parameters
    if (argc != 3) {
        printf("Usage: %s <server-name> <port-number>\n", argv[0]);
        return -1;
    }

    // No print buffering
    setvbuf (stdout, NULL, _IONBF, 0);

    printf("Making client...\n");
    Client* c = new Client(argv[1], argv[2]);
    printf("Client Success!\n\n\n");

    // Message holder
    char msg[DEFAULT_BUFLEN];

    // Send/Receive Loop
    do {
        printf("Enter message: ");
        scanf("%s", msg);
        c->client_send((const char *) &msg);
        c->client_receive();
        printf("Message Recieved: \"%s\"\n\n", c->recvbuf);
    } while (true);

	return 0;
}