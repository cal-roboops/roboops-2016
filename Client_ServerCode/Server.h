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
#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Server Class
class Server {
 	private:
 		// Private Variables
		int status;
		int iResult;
 		WSADATA wsaData;
		static const int iReqWinsockVer = 2;

    	SOCKET ListenSocket = INVALID_SOCKET;
    	SOCKET ClientSocket = INVALID_SOCKET;

    	struct addrinfo *result = NULL;
	    struct addrinfo hints;

	    char recvbuf[DEFAULT_BUFLEN];
	    static const int recvbuflen = DEFAULT_BUFLEN;
	    static const int sendbuflen = DEFAULT_BUFLEN;

 		// Private Methods
 		int sSocket();
 		int sConnect();

 	public:
 		// Public Variables
 		int iSendResult;
 		int iReceiveResult;

 		// Public Methods
 		Server();
 		~Server();
 		int sReceive();
 		int sSend(const char* message);
};