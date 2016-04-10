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
    if (argc != 3) {
        printf("Usage: %s <server-name> <port-number>\n", argv[0]);
        return 1;
    }

    // No print buffering
    setvbuf (stdout, NULL, _IONBF, 0);

    printf("Begin Command Computer Setup...\n");

    // Connect to the command computer components
    printf("Setting up Controller, Windows and Defaults... ");
    // Default Messages
    // const char* endMsg = "Done!";
    const char* complete = "Finished running commands.";

    // Controller
    s_X52 = new Saitek_X52();

    printf("Done!\n");

    // Create command client and connect to Rover Server
    printf("Making client... ");
    Client* winC = new Client(argv[1], argv[2]);
    printf("Done!\n");

    printf("Command Computer Setup Complete!\n\n\n");

	// Command Loop
	do {
        // Setup command list and mode
        printf("Enter rover mode followed by a list of comma separated commands:\n");
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