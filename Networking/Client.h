//
// Client.h
// CPP Project
//
// Created by Mitchell Oleson on 2/8/2016
// 
// Copyright © 2016 Mitchell Oleson. All rights reserved.
//
// Written for Windows
//

#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 512
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
 		char* server_ip;
 		char* server_port;

		int status;
		int iResult;
 		WSADATA wsaData;
		static const int iReqWinsockVer = 2;

    	SOCKET ListenSocket = INVALID_SOCKET;
    	SOCKET ClientSocket = INVALID_SOCKET;

    	struct addrinfo *result = NULL;
        struct addrinfo *ptr = NULL;
        struct addrinfo host_info;

        int iSendResult;
 		int iReceiveResult;
	    static const int recvbuflen = DEFAULT_BUFLEN;
	    static const int sendbuflen = DEFAULT_BUFLEN;

 		// Private Methods
 		int client_connect();

 	public:
 		// Public Variables
 		char recvbuf[DEFAULT_BUFLEN];

 		// Public Methods
 		Client(char* ip, char* port);
 		~Client();
 		int client_receive();
 		int client_send(const char* msg);
};