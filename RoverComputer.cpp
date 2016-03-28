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

// Takes in the action and acts accordingly
int act(int action) {
    switch (mode) {
        case 0: ret = car(action); // Drive using car mode
        case 1: ret = tank(action); // Drive using tank mode
        case 2: ret = arm(action); // Move the arm
    }
    return ret;
}

int car(int action) {
    return action;
}

int tank(int action) {
    return action;
}

int arm(int action) {
    return action;
}

// Main control function for the Rover
int main(int argc, char **argv) {
    // Check command line arguments
	if (argc != 2) {
        printf("Usage: %s <port-number>\n", argv[0]);
        return 1;
    }

    // Setup the wiring pi interface
    printf("Setting up WiringPi...\n");
    wiringPiSetup();
    printf("Wiring Pi Success!\n");

    // Connect to the rover components
    printf("Setting up UART, Motors, Encoders and Defaults...\n");
    mode = 0;
    printf("Setup Success!\n");

    // Create rover server and connect to command computer
    printf("Making server...\n");
    Server* raspPi = new Server(argv[1]);
    printf("Server Success!\n\n\n");
    
    // Command Loop
    do {
        // Receive commands from the main computer (Space seperated list)
    	raspPi->server_receive();
        printf("Message Recieved: \"%s\"\n", raspPi->recvbuf);

        // End connection if received endMsg command
        if (strcmp(raspPi->recvbuf, endMsg) == 0) {
            return 0;
        }

        // Set mode to first value
        mode = strtol(raspPi->recvbuf, &command, 10);

        // Parse space seperated command list
        while (command != NULL) {
            // Act on each hexadecimal command
            res = act(strtol(command, &command, 0));

            // Check for successful completion of command
            if (res != 0) {
                // Send bad status and corresponding hex command for failures
                raspPi->server_send(bad);
                memset(raspPi->msgbuf, 0, sizeof(raspPi->msgbuf));
                itoa(res, raspPi->msgbuf, 16);
                raspPi->server_send((const char*) &(raspPi->msgbuf));
            } else {
                // Send confirmation for each successful command
                raspPi->server_send(good);
            }
        }

        // Send command completion message
    	raspPi->server_send(complete);
        printf("\n");
    } while (true);

    return 0;
}