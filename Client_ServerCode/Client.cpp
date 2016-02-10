//
// Client.cpp - Version 2
// CPP Project
//
// Created by Andrew Pau on 1/14/2016
// Modified by Mitchell Oleson on 2/8/2016
// 
// Copyright © 2016 Andrew Pau. All rights reserved.
//
//

#include "Client.h"

// Client Constructor
Client::Client(char* ip) {
    printf("Starting Setup...\n");
	status = cSocket(ip);
	if (status == 1) {
		exit(1);
	}
    printf("Setup Complete!\n");
}

// Client Destructor
Client::~Client() {
	iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

}

// Socket Initializer
int Client::cSocket(char* ip) {
    printf("Setting up socket...\n");

	// Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the Server address and port
    iResult = getaddrinfo(ip, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    printf("Socket Success!\n");
    printf("Connecting...\n");

    // Attempt to Client to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for Clienting to server
        ClientSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ClientSocket == INVALID_SOCKET) {
            printf("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Client to server.
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
        return 1;
    }

    printf("Connection Success!\n");

    return 0;
}

// Send messages
int Client::cSend(const char* message) {
	iSendResult = send(ClientSocket, message, (int) strlen(message), 0 );
    if (iSendResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
    printf("Bytes sent: %d\n", iSendResult);

    return 0;
}

// Recieve messages
int Client::cReceive() {
	iReceiveResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    if (iReceiveResult > 0) {
        printf("Bytes received: %d\n", iReceiveResult);
    }
    else  {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    return 0;
}

// Main Method for Command Line
// Sets up intial config and begins 
// ouputing to the terminal
int main(int argc, char **argv) {
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    Client* c = new Client(argv[1]);

    const char* test = "This is a test message.";
    do {
        c->cSend(test);
        c->cReceive();
    } while (true);
	return 0;
}