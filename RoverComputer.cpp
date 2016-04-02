//
// RoverComputer.cpp
// CPP Project
//
// Created by Mitchell Oleson on 2/11/2016
//
// Made for Debian
//

#include "RoverComputer.h"

// Command Transmission form (car):
// RightRoboClaw, LeftRoboClaw, Servo1, Servo2, Servo3, Servo4, CameraServo
int car(int[] action) {
    return action[0];
}

// Command Transmission form (tank):
// RightRoboClaw, LeftRoboClaw, Servo1, Servo2, Servo3, Servo4, CameraServo
int tank(int[] action) {
    return action[0];
}

// Command Transmission form (arm):
// BaseSwivel, 
int arm(int[] action) {
    return action[0];
}

// Takes in the action and acts accordingly
int act(int[] action, int mode) {
    switch (mode) {
        case 0: return car(action); // Drive using car mode
        case 1: return tank(action); // Drive using tank mode
        case 2: return arm(action); // Move the arm
    }
    return mode;
}

// Initialize and setup the rover from its folded state
int initialize() {
    return 0;
}

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
    wiringPiSetup();
    printf("Done!\n");

    // Connect to the rover components
    printf("Setting up UART, Motors, Encoders, Servos and Defaults... ");
    // Default modes
    int mode = 0;
    int prev_mode = 0;

    // Default Messages
    const char* endMsg = "Done!";
    const char* complete = "Finished running commands.";
    const char* good = "All is good.";

    // Command list for different modes
    char* command;
    int command_list[10];

    // Indexing variable
    int i;

    // Status variable
    int status;

    // Motors
    RoboClaw_Raspi roboclaws[2];
    roboclaws[0] = new RoboClaw_Raspi(ROBOCLAW0);
    roboclaws[1] = new RoboClaw_Raspi(ROBOCLAW1)

    // Servos
    Servo_Raspi servos[4];
    servos[0] = new Servo_Raspi(SERVO0PIN, DEFAULTRANGE);
    servos[1] = new Servo_Raspi(SERVO1PIN, DEFAULTRANGE);
    servos[2] = new Servo_Raspi(SERVO2PIN, DEFAULTRANGE);
    servos[3] = new Servo_Raspi(SERVO3PIN, DEFAULTRANGE);

    // Encoders
    Encoder_Raspi encoders[4];
    encoders[0] = new Encoder_Raspi(ENCODER0PIN);
    encoders[1] = new Encoder_Raspi(ENCODER1PIN);
    encoders[2] = new Encoder_Raspi(ENCODER2PIN);
    encoders[3] = new Encoder_Raspi(ENCODER3PIN);

    printf("Done!\n");

    // Create rover server and connect to command computer
    printf("Setting up connection... ");
    Server* raspPi = new Server(argv[1]);
    printf("Done!\n");

    // Initialize the rover
    printf("Setting up physical Rover... ");
    status = initialize();
    if (status != 0) {
        printf("Could not initialize rover.\n");
        return 1;
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
            return 0;
        }

        // Set mode to first value
        prev_mode = mode;
        mode = strtol(strtok(raspPi->recvbuf, ","), NULL, 10);

        if (mode != prev_mode) {
            ;
        }

        // Reset index
        i = 0;
        // Parse the comma seperated command list
        command = strtok(NULL, ",");
        while (command != NULL) {
            // Translate command to int and store in command list
            command_list[i] = strtol(command, NULL, 16);
            // Get next command
            command = strtok(NULL, ",");
            i++;
        }
        
        // Act on the list of hexadecimal command
        char* res = act(command_list, mode);

        // Check for successful completion of command
        if (res != 0) {
            // Send bad status and corresponding hex command for failures
            strcpy(raspPi->msgbuf, "Something isn't right: ");
            strcat(raspPi->msgbuf, res);
            raspPi->server_send(raspPi->msgbuf);
        } else {
            // Send confirmation for each successful command
            raspPi->server_send(good);
        }

        // Send command completion message
    	raspPi->server_send(complete);
        printf("\n");
    } while (true);

    return 0;
}
