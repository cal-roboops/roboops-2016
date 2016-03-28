//
// RoverComputer.cpp
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
#define MOTOR1 0x80
#define MOTOR2 0x81
#define MOTOR3 0x82
#define MOTOR4 0x83
#define FR MOTOR1
#define BR MOTOR2
#define FL MOTOR3
#define BL MOTOR4

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "Networking/Server.h"

int car(int action) {
    return action;
}

int tank(int action) {
    return action;
}

int arm(int action) {
    return action;
}

// Takes in the action and acts accordingly
int act(int action, int mode) {
    switch (mode) {
        case 0: return car(action); // Drive using car mode
        case 1: return tank(action); // Drive using tank mode
        case 2: return arm(action); // Move the arm
    }
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
    // Default modes
    int mode = 0;
    int prev_mode = 0;
    // Default Messages
    const char* endMsg = "Done!";
    const char* complete = "Finished running commands.";
    const char* good = "All is good.";
    char* command;
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
        prev_mode = mode;
        mode = strtol(strtok(raspPi->recvbuf, ","), NULL, 10);

        if (mode != prev_mode) {
            ;
        }

        // Parse comma seperated command list
        command = strtok(NULL, ",");
        while (command != NULL) {
            // Act on each hexadecimal command
            int res = act(strtol(command, NULL, 16), mode);

            // Check for successful completion of command
            if (res != 0) {
                // Send bad status and corresponding hex command for failures
                strcpy(raspPi->msgbuf, "Something isn't right: ");
                strcat(raspPi->msgbuf, command);
                raspPi->server_send(raspPi->msgbuf);
            } else {
                // Send confirmation for each successful command
                raspPi->server_send(good);
            }
            command = strtok(NULL, ",");
        }

        // Send command completion message
    	raspPi->server_send(complete);
        printf("\n");
    } while (true);

    return 0;
}
