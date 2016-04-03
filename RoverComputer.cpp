//
// RoverComputer.cpp
// CPP Project
//
// Created by Mitchell Oleson on 2/11/2016
//
// Made for Debian
//

#include "RoverComputer.h"

// Main control function for the Rover
int main(int argc, char **argv) {
    // Check command line arguments
	if (argc != 2) {
        printf("Usage: %s <port-number>\n", argv[0]);
        return 1;
    }

    printf("Begin Rover Setup...\n");

    // Setup the wiring pi interface
    printf("Setting up WiringPi... ");
    if (wiringPiSetup() == -1) {
        printf("WiringPi failed to start.\n");
        exit(1);
    }
    printf("Done!\n");

    // Connect to the rover components
    printf("Setting up UART, Motors, Encoders, Servos and Defaults... ");
    // Default modes
    int mode = 0;
    int prev_mode = 0;

    // Default Messages
    const char* endMsg = "Done!\n";
    const char* complete = "Finished running commands.\n";
    const char* good = "All is good.\n";
    const char* bad = "Something broke...\n";

    // Command list for different modes
    char* command;
    char* command_list[25];

    // Indexing & result variables
    int i;
    int res;

    // Motors
    RoboClaw* roboclaw = new RoboClaw(ROBOCLAWDEVICE, BAUDRATE);

    // Servos (use softServoWrite(pin, value) to control)
    softServoSetup(CHASSISSERVOPINFL, CHASSISSERVOPINBL, CHASSISSERVOPINFR, 
        CHASSISSERVOPINBR, 0, 0, 0, 0);

    // Encoders
    Encoder* encoders[4];
    encoders[0] = new Encoder(ENCODERPIN0);
    encoders[1] = new Encoder(ENCODERPIN1);
    encoders[2] = new Encoder(ENCODERPIN2);
    encoders[3] = new Encoder(ENCODERPIN3);

    printf("Done!\n");

    // Create rover server and connect to command computer
    printf("Setting up connection... ");
    Server* raspPi = new Server(argv[1]);
    printf("Done!\n");

    // Initialize the rover
    printf("Setting up physical Rover... ");
    if (initialize() != 0) {
        printf("Could not initialize rover.\n");
        exit(1);
    }
    printf("Done!\n");

    printf("Rover Setup Complete!\n\n\n");

    // Command Loop
    do {
        // Receive commands from the main computer (Space seperated list)
    	raspPi->server_receive();
        printf("Message Recieved: \"%s\"\n", raspPi->recvbuf);

        // End connection if received endMsg command
        if (strcmp(raspPi->recvbuf, endMsg) == 0) {
            stop(mode);
            break;
        }

        // Set mode to first value
        prev_mode = mode;
        mode = strtol(strtok(raspPi->recvbuf, ","), NULL, 10);

        // Stop the previous modes commands
        if (mode != prev_mode) {
            if (stop(prev_mode) != 0) {
                printf("Couldn't stop previous mode.\n");
                //exit(1);
            }
        }

        // Reset index
        i = 0;
        // Parse the comma seperated command list
        command = strtok(NULL, ",");
        while (command != NULL) {
            // Translate command to int and store in command list
            command_list[i] = command;
            // Get next command and incrememnt i
            command = strtok(NULL, ",");
            i++;
        }

        // Act on the list of hexadecimal command
        res = act(command_list, mode);

        // Check for successful completion of command
        if (res == -1) {
            // Send confirmation for each successful command
            raspPi->server_send(bad);
        } else {
            // Send bad status and corresponding hex command for failures
            raspPi->server_send(good);
        }

        // Send command completion message
    	raspPi->server_send(complete);
        printf("\n");
    } while (true);

    return 0;
}
