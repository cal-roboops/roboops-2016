//
// RaspPi.cpp
// CPP Project
//
// Made for mid-project review code performance test
// 
// Created by Mitchell Oleson on 2/11/2016
//
// Made for Debian
//

#define TXD_UART 14
#define RXD_UART 15

#include <WiringPi.h>
#include "../Client_ServerCode/Server.h"

int act(int action) {
	;
}

int main(int argc, char **argv) {
	if (argc != 2) {
        printf("Usage: %s <port-number>\n", argv[0]);
        return 1;
    }

    printf("Setting up WiringPi...\n");
    wiringPiSetup();
    printf("Wiring Pi Success!\n");

    printf("Making server...\n");
    Server* rp = new Server(argv[1]);
    printf("Server Success!\n\n\n");


    // Response
    const char* confirmation = "All is good";
    
    // Command Loop
    do {
    	rp->sReceive();
        printf("Message Recieved: \"%s\"\n", s->recvbuf);
        rp->act(stoi(recvbuf, nullptr, 16));
    	rp->sSend(confirmation);
        printf("\n");
    } while (true);

    return 0;
}