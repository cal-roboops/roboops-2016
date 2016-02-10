//
// Client.h
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

// Client Class
class Client {
 	private:
 		// Private Variables
		int status;
		int iResult;
 		WSADATA wsaData;
		static const int iReqWinsockVer = 2;

    	SOCKET ListenSocket = INVALID_SOCKET;
    	SOCKET ClientSocket = INVALID_SOCKET;

    	struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

	    char recvbuf[DEFAULT_BUFLEN];
	    static const int recvbuflen = DEFAULT_BUFLEN;
	    static const int sendbuflen = DEFAULT_BUFLEN;

 		// Private Methods
 		int cSocket(char* ip);

 	public:
 		// Public Variables
 		int iSendResult;
 		int iReceiveResult;

 		// Public Methods
 		Client(char* ip);
 		~Client();
 		int cReceive();
 		int cSend(const char* message);
};