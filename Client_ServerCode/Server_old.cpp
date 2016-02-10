//
//  Server.cpp
//  Cpp project
//
//  Created by Andrew Pau on 1/14/16.
//  Copyright © 2016 Andrew Pau. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <winsock.h>

#define PORT "5000" // Port to be used
#define BACKLOG 3   // Number of pending connections allowed in queue

void server(){
    int status;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    
    memset(&host_info, 0, sizeof(host_info));
    
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags = AI_PASSIVE;
    
    status = getaddrinfo(NULL, PORT, &host_info, &host_info_list);
    if (status != 0) {
        perror("getaddrinfo error");
        exit(1);
    }
    
    int socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
    
    if (socketfd == -1) {
        perror("Socket error");
        exit(1);
    } else {
        printf("Created socket %d\n",socketfd);
    }
    
    int yes = 1;
    status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1){
        perror("Bind error");
        exit(1);
    }
    
    status = listen(socketfd, BACKLOG);
    
    if (status == -1) {
        perror("Listen error");
        exit(1);
    }
    printf("Awaiting connection...\n");
    int new_sockfd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    new_sockfd = accept(socketfd, (struct sockaddr*) &their_addr, &addr_size);
    
    if (new_sockfd == -1){
        perror("Listen error");
        exit(1);
    } else {
        printf("Connection successful!\n");
    }
    
    printf("Waiting to receive data...\n");
    ssize_t bytes_recieved;
    char incoming_data_buffer[1000];
    bytes_recieved = recv(new_sockfd, incoming_data_buffer, 1000, 0);
    if (bytes_recieved == 0) {
        perror("Host closed.");
        exit(1);
    }
    if (bytes_recieved == -1) {
        perror("Receive error");
        exit(1);
    }
    printf("Bytes recieved : %zd\n", bytes_recieved);
    std::cout << incoming_data_buffer << std::endl;
    incoming_data_buffer[bytes_recieved] = '\0';
    
    freeaddrinfo(host_info_list);
    close(socketfd);
    close(new_sockfd);
    
}
