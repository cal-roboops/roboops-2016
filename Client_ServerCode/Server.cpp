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

using namespace std;

#include "Server.h"

// Server Constructor
Server::Server() {
	printf("Setting up socket...\n");
	st = startSocket();
	if (st == 1) {
		exit(1);
	}
	
	printf("Setup Success!\n");
	printf("Connecting...\n");
	st = serverConnect();
	if (st == 1) {
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
int Server::startSocket() {
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

    return 0;
}

// Connect client and server ports
int Server::serverConnect() {
    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
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
int Server::serverSend(const char* message) {
	iSendResult = send(ClientSocket, message, sendbuflen, 0 );
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
int Server::serverReceive() {
	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	if (iResult == 0)
        printf("Connection closing...\n");
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
int main() {
	return 0;
}