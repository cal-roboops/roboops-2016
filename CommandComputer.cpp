//
// CommandComputer.cpp
// CPP Project
// 
// Created by Mitchell Oleson on 2/11/2016
//
// Made for Windows
//

#include "CommandComputer.h"

// ---------- MAIN FUNCTION -----------

// Main control function for Command Computer
int main(int argc, char **argv) {
	// Validate the parameters
    if (argc < 2) {
        printf("Usage: %s <server-name> [<port-number>]\n", argv[0]);
        return 1;
    }

    // Chcek if port is given
    if (argc == 3) {
        port = argv[2];
    }

    // No print buffering
    setvbuf (stdout, NULL, _IONBF, 0);

    // Connecting to Rover
    printf("Connecting to Rover... ");
    Client* winC = new Client(argv[1], port);
    printf("Done!\n");

    // Make sure we're connected
    winC->client_receive();
    printf("%s\n", winC->recvbuf);

    printf("Begin Command Computer Setup...\n");

    // Connect to the command computer components
    printf("Setting up Controller, Windows and Defaults... ");

    printf("Done!\n");

    printf("Command Computer Setup Complete!\n\n");

    printf("Begin Rover Setup Instructions...\n");
    ZeroMemory(winC->recvbuf, sizeof(winC->recvbuf));
    while(strstr(winC->recvbuf, "Rover Ready!") == NULL) {
        printf("Enter instruction: ");
        scanf("%s", winC->msgbuf);
        winC->client_send((const char*) &(winC->msgbuf));
        winC->client_receive();
        printf("%s\n", winC->recvbuf);
    }
    
    printf("Rover Setup Complete!\n");

	// Command Loop
	do {
        // Setup command list and mode
        printf("Enter rover mode followed by a list of comma separated commands:\n");
        scanf("%s", winC->msgbuf);

        // Send list of commands to the rover
        winC->client_send((const char*) &(winC->msgbuf));

        // Receive running status of commands
        memset(winC->recvbuf, 0, sizeof(winC->recvbuf));
        while (strstr(winC->recvbuf, complete) == NULL) {
            if (strstr(winC->recvbuf, endMsg) != NULL) {
                printf("Recieved End Message from Rover.\n");
                break;
            }
            winC->client_receive();
            printf("Message Recieved: \"%s\"\n\n", winC->recvbuf);
        }
    } while (true);

	return 0;
}