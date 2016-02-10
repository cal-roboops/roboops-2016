//
// Server.h - Version 2
// CPP Project
//
// Created by Mitchell Oleson on 2/8/2016
//
// Copyright © 2016 Andrew Pau. All rights reserved.
//
// Written for Debian Linux
//

#define BACKLOG 3
#define DEFAULT_BUFLEN 512

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>

// Server Class
class Server {
	private:
	    // Private Variables
	    char* listening_port;

	    int status;
	    int iResult;
	    int ServerSocket;
	    socklen_t addr_size;
	    struct addrinfo host_info;
	    struct addrinfo *host_info_list;
	    struct sockaddr_storage client_addr;

	    int iSendResult;
 	    int iReceiveResult;
	    char recvbuf[DEFAULT_BUFLEN];
	    static const int recvbuflen = DEFAULT_BUFLEN;
	    static const int sendbuflen = DEFAULT_BUFLEN;

	    // Private Methods
	    int sSocket();
	    int sConnect();

	public:
	    // Public Variables
	    // Public Methods
	    Server(char* port);
	    ~Server();
	    int sReceive();
	    int sSend(const char* msg);
};
