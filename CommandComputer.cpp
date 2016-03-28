//
// CommandComputer.cpp
// CPP Project
//
// Made for mid-project review code performance test
// 
// Created by Mitchell Oleson on 2/11/2016
//
// Made for Windows
//

#define DEFAULT_BUFLEN 512

#include <stdio.h>
#include <stdlib.h>
#include "Networking/Client.h"

int main(int argc, char **argv) {
	// Validate the parameters
    if (argc != 3) {
        printf("Usage: %s <server-name> <port-number>\n", argv[0]);
        return 1;
    }

    // No print buffering
    setvbuf (stdout, NULL, _IONBF, 0);

    // Connect to the command computer components
    printf("Setting up Controller, Windows and Defaults...");
    // Default Messages
    // const char* endMsg = "Done!";
    const char* complete = "Finished running commands.";
    printf("Setup Success!");

    // Create command client and connect to Rover Server
    printf("Making client...\n");
    Client* winC = new Client(argv[1], argv[2]);
    printf("Client Success!\n\n\n");

	// Command Loop
	do {
        // Enter command list and mode
        printf("Enter rover mode followed by a list of space separated commands:\n");
        scanf("%s", winC->msgbuf);

        // Send list of commands to the rover
        winC->client_send((const char *) &(winC->msgbuf));

        // Receive running status of commands
        memset(winC->recvbuf, 0, sizeof(winC->recvbuf));
        while (strstr(winC->recvbuf, complete) == NULL) {
            winC->client_receive();
            printf("Message Recieved: \"%s\"\n\n", winC->recvbuf);
        }
    } while (true);

	return 0;

}