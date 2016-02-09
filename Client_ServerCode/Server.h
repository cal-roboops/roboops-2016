//
// Server.h
// CPP Project
//
// Created by Mitchell Oleson on 2/8/2016
// 
// Copyright © 2016 Mitchell Oleson. All rights reserved.
//
//

#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

// Server Class
class Server {
 	private:
 		// Private Variables
 		WSADATA wsaData;
		int iReqWinsockVer = 2;
		int iResult;
		int st;

    	SOCKET ListenSocket = INVALID_SOCKET;
    	SOCKET ClientSocket = INVALID_SOCKET;

    	struct addrinfo *result = NULL;
	    struct addrinfo hints;

	    int iSendResult;
	    int sendbuflen = DEFAULT_BUFLEN;

	    char recvbuf[DEFAULT_BUFLEN];
	    int recvbuflen = DEFAULT_BUFLEN;

 		// Private Methods
 		int startSocket();
 		int serverConnect();
 		int serverReceive();

 	public:
 		// Public Variables
 		// Public Methods
 		Server();
 		~Server();
 		int serverSend(const char* message);
};