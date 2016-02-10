//
// Server.cpp - Version 2
// CPP Project
//
// Created by Andrew Pau on 1/14/2016
// Modified by Mitchell Oleson on 2/8/2016
// 
// Copyright © 2016 Andrew Pau. All rights reserved.
//
//

#include "Server.h"

// Server Constructor
Server::Server() {
	printf("Setting up socket...\n");
	status = sSocket();
	if (status == 1) {
		exit(1);
	}
	
	printf("Setup Success!\n");
	printf("Connecting...\n");
	status = sConnect();
	if (status == 1) {
		exit(1);
	}
	printf("Connection Success!\n");
	printf("Setup Complete!\n");
}

// Server Destructor
Server::~Server() {
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
int Server::sSocket() {
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    return 0;
}

// Connect client and server ports
int Server::sConnect() {
    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);

    return 0;
}

// Send messages
int Server::sSend(const char* message) {
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
int Server::sReceive() {
	iReceiveResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    if (iReceiveResult > 0) {
        printf("Bytes received: %d\n", iReceiveResult);
    }
    else if (iReceiveResult == 0) {
        printf("Connection closing...\n");
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
int main(void) {
    Server* s = new Server();
    const char* test = "This is a test message.";
    do {
        s->sSend(test);
        s->sReceive();
    } while (s->iReceiveResult > 0 && s->iSendResult > 0);
	return 0;
}