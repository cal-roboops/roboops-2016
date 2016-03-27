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

#include "RoverComputer.h"

int act(int action) {
    int res = UART->send(action);
    if (res == -1) {
        return action;
    }
	return 0;
}

int main(int argc, char **argv) {
	if (argc != 2) {
        printf("Usage: %s <port-number>\n", argv[0]);
        return 1;
    }

    printf("Setting up WiringPi...\n");
    wiringPiSetup();
    printf("Wiring Pi Success!\n");

    printf("Setting up UART, Motors, Encoders and Defaults...\n");
    mode = 0;
    printf("Setup Success!\n")

    printf("Making server...\n");
    Server* raspPi = new Server(argv[1]);
    printf("Server Success!\n\n\n");
    
    // Command Loop
    do {
    	raspPi->server_receive();
        printf("Message Recieved: \"%s\"\n", raspPi->recvbuf);
        if (strcmp(raspPi->recvbuf, endMsg) == 0) {
            break;
        }
        mode = strtol(raspPi->recvbuf, &command, 10);
        while (command != NULL) {
            res = act(strtol(command, &command, 16));
            if (res != 0) {
                raspPi->server_send(bad);
                memset(raspPi->msgbuf, 0, sizeof(raspPi->msgbuf));
                itoa(res, raspPi->msgbuf, 16);
                raspPi->server_send((const char*) &(raspPi->msgbuf));
            } else {
                raspPi->server_send(good);
            }
        }
    	raspPi->server_send(endMsg);
        printf("\n");
    } while (true);

    return 0;
}