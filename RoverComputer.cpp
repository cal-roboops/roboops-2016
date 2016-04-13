//
// RoverComputer.cpp
// CPP Project
//
// Created by Mitchell Oleson on 2/11/2016
//
// Made for Debian
//

#include "RoverComputer.h"

// ---------- MAIN FUNCTION -----------

// Main control function for the Rover
int main(int argc, char **argv) {
    // Check command line arguments
    if (argc == 2) {
        port = argv[1];
    }

    printf("Begin Rover Setup...\n");

    // Create rover server and connect to command computer
    printf("Setting up connection... ");
    Server* raspPi = new Server(port);
    printf("Done!\n");

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
    const char* complete = "Finished running commands.";
    const char* good = "All is good.";
    const char* bad = "Something broke...";

    // Command list for different modes
    char* command;
    char* command_list[DEFAULT_BUFLEN];

    // Indexing variable
    int i;

    // Servos
    softServoSetup(CHASSIS_SERVO_PINFL, CHASSIS_SERVO_PINBL,
                    CHASSIS_SERVO_PINFR, CHASSIS_SERVO_PINBR,
                    0, 0, 0, 0);

    // Encoders
    Encoder* encoders[4];
    encoders[0] = new Encoder(ENCODER_PIN0);
    encoders[1] = new Encoder(ENCODER_PIN1);
    encoders[2] = new Encoder(ENCODER_PIN2);
    encoders[3] = new Encoder(ENCODER_PIN3);

    printf("Done!\n");

    // Initialize the rover
    printf("Setting up physical Rover... ");
    // Set Roboclaws to Zero
    // Setup Python
    Py_Initialize();
    // Import Modules and Set Path
    PyRun_SimpleString("import sys\n");
    PyRun_SimpleString("sys.path.append(\"./GPIO_RaspPi\")\n");
    PyRun_SimpleString("import roboclaw\n");
    // Open UART
    PyRun_SimpleString(SETUP_UART_PI2);
    PyRun_SimpleString(R1_ZERO);
    PyRun_SimpleString(R2_ZERO);
    // Shutdown Python
    Py_Finalize();
    // Set Servos to Dead Straigt
    softServoWrite(CHASSIS_SERVO_PINFL, SERVO_CENTER);
    softServoWrite(CHASSIS_SERVO_PINBL, SERVO_CENTER);
    softServoWrite(CHASSIS_SERVO_PINFR, SERVO_CENTER);
    softServoWrite(CHASSIS_SERVO_PINBR, SERVO_CENTER);

    printf("Done!\n");

    printf("Rover Setup Complete!\n\n\n");

    // Command Loop
    do {
        // Receive commands from the main computer (Space seperated list)
    	raspPi->server_receive();
        printf("Message Received: \"%s\"\n", raspPi->recvbuf);

        // End connection if received endMsg command
        if (strstr(raspPi->recvbuf, endMsg) != NULL) {
            printf("Received End Message from Command.\n");
            break;
        }

        // Set mode to first value
        prev_mode = mode;
        mode = strtol(strtok(raspPi->recvbuf, ","), NULL, 10);

        // Stop the previous modes commands
        if (mode != prev_mode) {
            // Set Roboclaws to Zero
            // Setup Python
            Py_Initialize();
            // Import Modules and Set Path
            PyRun_SimpleString("import sys\n");
            PyRun_SimpleString("sys.path.append(\"./GPIO_RaspPi\")\n");
            PyRun_SimpleString("import roboclaw\n");
            // Open UART
            PyRun_SimpleString(SETUP_UART_PI2);
            // Run commands
            PyRun_SimpleString(R1_ZERO);
            PyRun_SimpleString(R2_ZERO);
            // Shutdown Python
            Py_Finalize();
            // Set Servos to Dead Straigt
            softServoWrite(CHASSIS_SERVO_PINFL, SERVO_CENTER);
            softServoWrite(CHASSIS_SERVO_PINBL, SERVO_CENTER);
            softServoWrite(CHASSIS_SERVO_PINFR, SERVO_CENTER);
            softServoWrite(CHASSIS_SERVO_PINBR, SERVO_CENTER);
        }

        // Parse the comma seperated command list
	    i = 0;
        command = strtok(NULL, ",");
        while (command != NULL) {
            // Translate command to int and store in command list
            command_list[i] = command;
            // Get next command and incrememnt i
            command = strtok(NULL, ",");
            i++;
        }

        // Act on the list of commands
        if (mode == 0 || mode == 1) {
            // Set Roboclaws
            Py_Initialize();
            // Import Modules and Set Path
            PyRun_SimpleString("import sys\n");
            PyRun_SimpleString("sys.path.append(\"./GPIO_RaspPi\")\n");
            PyRun_SimpleString("import roboclaw\n");
            // Open UART
            PyRun_SimpleString(SETUP_UART_PI2);
            // Run Commands
            sprintf(command, "roboclaw.ForwardBackwardMixed(128, %d)", strtol(command_list[0], NULL, 10));
            PyRun_SimpleString((const char*) command);
            sprintf(command, "roboclaw.ForwardBackwardMixed(129, %s)", strtol(command_list[1], NULL, 10));
            PyRun_SimpleString((const char*) command);
            // Shutdown Python
            Py_Finalize();

            // Set Servos
            softServoWrite(CHASSIS_SERVO_PINFL, strtol(command_list[2], NULL, 10));
            softServoWrite(CHASSIS_SERVO_PINBL, strtol(command_list[3], NULL, 10));
            softServoWrite(CHASSIS_SERVO_PINFR, strtol(command_list[4], NULL, 10));
            softServoWrite(CHASSIS_SERVO_PINBR, strtol(command_list[5], NULL, 10));

            // Send confirmation
            raspPi->server_send(good);
        } else {
            // Send bad command
            raspPi->server_send(bad);
        }

        // Send command completion message
    	raspPi->server_send(complete);
        printf("\n");
    } while (true);

    return 0;
}
