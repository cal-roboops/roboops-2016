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

#include <iostream>
#include <cstring>
#include <winsock.h>
//#include <windows.h>

// Server Class
class Server {
 	private:
 		// Private Variables
 		int port;
 		int ip;
 		int st;
 		SOCKET serverSock;
 		sockaddr_in sockAddr;
 		int connect(SOCKET s, const struct sockaddr *name, int namelen);
 		int send(SOCKET s, const char *buf, int len, int flags);
 		int recv(SOCKET s, char *buf, int len, int flags);
 		int listen(SOCKET s, int backlog);
 		int closesocket(SOCKET s);
 		SOCKET socket(int af, int type, int protocol);
 		int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
		int WSACleanup();
 		int bind(SOCKET s, const struct sockaddr *name, int namelen);
 		SOCKET accept(SOCKET s, struct sockaddr *addr, int *addrlen);

 		// Private Methods
 		int start_socket(char ip, int port);
 		int connect();

 	public:
 		// Public Variables
 		// Public Methods
 		Server(char ip, int port);
 		~Server();
 		int send(char message);
};

// Server Constructor
Server::Server(char ip, int port) {
	std::cout<<"Setting up socket..."<<std::endl;
	st = start_socket(ip, port);
	if (st == -1) {
		exit(1);
	}
	std::cout<<"Setup Success! Connecting..."<<std::endl;
	st = connect();
	if (st == -1) {
		exit(1);
	}
	std::cout<<"Connection Success!"<<std::endl;
	std::cout<<"Setup Complete!"<<std::endl;
}

// Server Destructor
Server::~Server() {
	closesocket(serverSock);
}

int Server::start_socket(char ip, int port) {
	WSADATA wsaData;
	int iReqWinsockVer = 2;
	if (WSAStartup(MAKEWORD(iReqWinsockVer,0), &wsaData)==0) {
	    // Check if major version is at least iReqWinsockVer
	    if (LOBYTE(wsaData.wVersion) >= iReqWinsockVer) {
	        serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (serverSock==INVALID_SOCKET) {
			    std::cout << "Invalid Socket" << std::endl;
			    return -1;
			}
			// Setup Socket Address
			sockAddr.sin_family = AF_INET;
			sockAddr.sin_port = htons(port);
			sockAddr.sin_addr.S_un.S_addr = inet_addr((const char*) &ip);
			return 1;
	    } else {
	        std::cout << "Required socket version not available" << std::endl;
	        return -1;
	    }
	    // Cleanup winsock
	    if (WSACleanup()!=0) {
	        // cleanup failed
	    }
	} else {
		std::cout << "Socket Startup Failed" << std::endl;
		return -1;
	}
	return -1;
}

int Server::connect() {
	// Connect to client
	if (connect(serverSock, (sockaddr*)(&sockAddr), sizeof(sockAddr))!=0) {
		std::cout << "Connection Failed." << std::endl;
	    return -1;
	}
	return 1;
}

int Server::send(char message) {
	if (send(serverSock, (const char*) &message, sizeof(message), 0)==SOCKET_ERROR)
    {
        // error handling code
    }
}

// Main Method for Command Line
// Sets up intial config and begins 
// ouputing to the terminal
int main() {
	char ip;
	int port;
	std::cout << "Destination IP Address:" << std::endl;
	std::cin>>ip;
	std::cout << "Destination Port: " << std::endl;
	std::cin>>port;
	Server server(ip, port);
}