//
//  Client.cpp
//  Cpp project
//
//  Created by Andrew Pau on 1/11/16.
//  Copyright © 2016 Andrew Pau. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>

#define PORT "5000" // Port that we will connect to

void client(){
    int status;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    
    char msg[] = "This is the message.";
    long len = strlen(msg);
    printf("The length of the message is %zd bytes.\n", len);
    
    memset(&host_info, 0, sizeof(host_info));
    
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    status = getaddrinfo("127.0.0.1", PORT, &host_info, &host_info_list);
    
    if (status != 0) {
        perror("getaddrinfo error");
        exit(1);
    }
    
    int socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
    
    if (socketfd == -1) {
        perror("Socket error");
        exit(1);
    } else {
        printf("Created socket %d\n", socketfd);
    }
    
    status = connect(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    
    if (status == -1) {
        perror("Connect error");
        exit(1);
    } else {
        printf("Connect successful\n");
    }
    
    ssize_t bytes_sent = send(socketfd, msg, len, 0);
    
    if (bytes_sent == -1){
        perror("Sending error");
        exit(1);
    }
    
    std::cout << "Bytes sent : " << bytes_sent << std:: endl;
    freeaddrinfo(host_info_list);
    close(socketfd);
    
}
