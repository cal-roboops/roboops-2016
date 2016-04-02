//
// Server.cpp - Version 2
// CPP Project
//
// Created by Andrew Pau on 1/14/2016
// Modified by Mitchell Oleson on 2/8/2016
//
// Written for Debian Linux
//

#include "Server.h"

// Server Constructor
Server::Server(char* port) {
	listening_port = port;

	//printf("Starting Server Setup...\n");

	status = server_socket();
	if (status == -1) {
		exit(1);
	}

	status = server_connect();
	if (status == -1) {
		exit(1);
	}

	//printf("Server Setup Complete!\n");
}

// Server Destroyer
Server::~Server() {
    // Cleanup
    close(ServerSocket);
}

// Socket Initializer
int Server::server_socket() {
	//printf("Setting up server socket...\n");

	memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags = AI_PASSIVE;

    // Resolve the listening address and port
    iResult = getaddrinfo(NULL, listening_port, &host_info, &host_info_list);
    if (status != 0) {
        perror("getaddrinfo error.");
        return -1;
    }

    ServerSocket = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
    if (ServerSocket == -1) {
        perror("Socket error.");
        return -1;
    }

    //printf("Server Socket Success!\n");
    return 0;
}

// Connection Initializer
int Server::server_connect() {
	//printf("Server Connecting...\n");

	// Prepare and bind socket for connection
    int reuse = 1;
    iResult = setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    iResult = bind(ServerSocket, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1){
        perror("Bind error.");
        return -1;
    }

    // Waiting for Connection
    iResult = listen(ServerSocket, BACKLOG);
    if (iResult == -1) {
        perror("Listen error");
        return -1;
    }

    // Accept Connection
    addr_size = sizeof(client_addr);
    ServerSocket = accept(ServerSocket, (struct sockaddr*) &client_addr, &addr_size);
    if (ServerSocket == -1) {
        perror("Accept error");
        return -1;
    }

    freeaddrinfo(host_info_list);

    //printf("Server Connection Success!\n");
    return 0;
}

// Send Messages
int Server::server_send(const char* msg) {
    iSendResult = send(ServerSocket, msg, (int) strlen(msg), 0);
    if (iSendResult == -1){
        perror("Sending error");
        return -1;
    }

    printf("Bytes sent: %d\n", iSendResult);
    return 0;
}

// Receive Messages
int Server::server_receive() {
    memset(recvbuf, 0, sizeof(recvbuf));
    iReceiveResult = recv(ServerSocket, recvbuf, recvbuflen, 0);
    if (iReceiveResult == 0) {
        perror("Host closed.");
        return -1;
    } else if (iReceiveResult == -1) {
        perror("Receive error");
        return -1;
    }

   printf("Bytes Received: %d\n", iReceiveResult);
   return 0;
}

// Main Method for Command Line
// Sets up initial config and begins outputting to the terminal
// Rename to main if only compiling this file
int main_server(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <port-number>\n", argv[0]);
        return -1;
    }

    printf("Making server... ");
    Server* s = new Server(argv[1]);
    printf("Done!\n\n\n");

    // Response
    const char* confirmation = "All is good";

    // Send/Receive Loop
    do {
    	s->server_receive();
        printf("Message Received: \"%s\"\n", s->recvbuf);
    	s->server_send(confirmation);
        printf("\n");
    } while (true);

    return 0;
}
