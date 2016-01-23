//
//  Server.cpp
//  Cpp project
//
//  Created by Andrew Pau on 1/14/16.
//  Copyright © 2016 Andrew Pau. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>

void server(){
    int status;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    
    memset(&host_info, 0, sizeof(host_info));
    
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags = AI_PASSIVE;
    
    status = getaddrinfo(NULL, "PORT_NUMBER", &host_info, &host_info_list);
    
    if (status != 0) {
        perror("getaddrinfo error");
        exit(1);
    }
    
    int socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
    
    if (socketfd == -1) {
        perror("Socket error");
        exit(1);
    }
    
    int yes = 1;
    status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1){
        perror("Bind error");
        exit(1);
    }
    
    int backlog = 3;
    status = listen(socketfd, backlog);
    
    if (status == -1) {
        perror("Listen error");
        exit(1);
    }
    int new_sockfd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    new_sockfd = accept(socketfd, (struct sockaddr*) &their_addr, &addr_size);
    
    if (new_sockfd == -1){
        perror("Listen error");
        exit(1);
    } else {
        std::cout << "Connection successful." <<std:: endl;
    }
    
    std::cout<< "Waiting to receive data..." <<std:: endl;
    ssize_t bytes_recieved;
    char incoming_data_buffer[1000];
    bytes_recieved = recv(new_sockfd, incoming_data_buffer, 1000, 0);
    if (bytes_recieved == 0) std::cout << "Host closed." << std::endl;
    if (bytes_recieved == -1) {
        perror("Receive error");
        exit(1);
    }
    std::cout << "Bytes recieved : " << bytes_recieved << std:: endl;
    incoming_data_buffer[bytes_recieved] = '\0';
    
    freeaddrinfo(host_info_list);
    close(socketfd);
    close(new_sockfd);
    
}
