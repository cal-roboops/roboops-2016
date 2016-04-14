//
// RoverComputer.cpp
// CPP Project
//
// Created by Mitchell Oleson on 2/11/2016
//
// Made for Debian
//

#include "RoverComputer.h"

// ---------- HELPERS -----------

// Stop RoboClaw
bool stop_roboclaws() {
    roboclaw->ForwardBackwardMixed(RIGHT_ROBOCLAW, RMIXED_ZERO);
    roboclaw->ForwardBackwardMixed(LEFT_ROBOCLAW, RMIXED_ZERO);
    return true;
}

// Set Servos Straight
bool reset_chassis_servos() {
    softServoWrite(CHASSIS_SERVO_PINFL, 500);
    softServoWrite(CHASSIS_SERVO_PINBL, 500);
    softServoWrite(CHASSIS_SERVO_PINFR, 500);
    softServoWrite(CHASSIS_SERVO_PINBR, 500);
    return true;
}

// ---------- INITIALIZE -----------

// Initialize and setup the rover from its folded state
bool initialize() {
    // Stop roboclaws
    if (!stop_roboclaws()) {
        return false;
    }

    // Set wheel servos to straight
    if (!reset_chassis_servos()) {
        return false;
    }

    return true;
}

// ---------- ACTION MODES -----------

// Command Transmission form (drive):
// Right_RoboClaw, Left_RoboClaw, CServoFL, CServoBL, CServoFR, CServoBR, CameraServo
bool drive(char* action[]) {
    roboclaw->ForwardBackwardMixed(RIGHT_ROBOCLAW, strtol(action[0], NULL, 10));
    roboclaw->ForwardBackwardMixed(LEFT_ROBOCLAW, strtol(action[1], NULL, 10));
    softServoWrite(CHASSIS_SERVO_PINFL, strtol(action[2], NULL, 10));
    softServoWrite(CHASSIS_SERVO_PINBL, strtol(action[3], NULL, 10));
    softServoWrite(CHASSIS_SERVO_PINFR, strtol(action[4], NULL, 10));
    softServoWrite(CHASSIS_SERVO_PINBR, strtol(action[5], NULL, 10));
    return true;
}

// Command Transmission form (arm):
// BaseSwivel, BaseJoint, ElbowJoint, ArmExtend, Claw
bool arm(char* action[]) {
    return true;
}

// ---------- ACTION SELECTOR -----------

// Takes in the desired mode/actions and acts accordingly
bool act(char* action[], int mode) {
    switch (mode) {
        case 0:
        case 1: return drive(action); // Drive using car mode
        case 2: return arm(action); // Move the arm
        default: return false;
    }
}

// ---------- ZERO MODES -----------

// Zeros the drive mode
bool zero_drive() {
    // Stop roboclaws
    if (!stop_roboclaws()) {
        return false;
    }

    // Set wheel servos to straight
    if (!reset_chassis_servos()) {
        return false;
    }

    return true;
}

// Zeros the arm mode
bool zero_arm() {
    return true;
}

// ---------- ZERO SELECTOR -----------

// Stops actions/movements of the previous mode
bool stop(int mode) {
    switch (mode) {
        case 0:
        case 1: return zero_drive();
        case 2: return zero_arm();
        default: return false;
    }
}

// ---------- MAIN FUNCTION -----------

// Main control function for the Rover
int main(int argc, char **argv) {
    // Check command line arguments
    if (argc == 2) {
        port = argv[1];
    }

    // Create rover server and connect to command computer
    printf("Connecting to Command Computer... ");
    raspPi = new Server(port);
    printf("Done!\n");

    // Send first message
    raspPi->server_send("Connected!");

    // Get confirmation to setup systems
    memset(raspPi->recvbuf, 0, sizeof(raspPi->recvbuf));
    while (strstr(raspPi->recvbuf, "Setup!") == NULL) {
        raspPi->server_receive();
        printf("%s\n", raspPi->recvbuf);
    }

    // Send start of initializing
    raspPi->server_send("Setting up... ");

    printf("Begin Rover Setup...\n");

    // Setup the WiringPi interface
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

    // Command list for different modes
    char* command;
    char* command_list[DEFAULT_BUFLEN];

    // Indexing variable
    int i;

    // Motors
    roboclaw = new RoboClaw(UART_PI2, (uint32_t) 10);

    // Servos
    softServoSetup(CHASSIS_SERVO_PINFL, CHASSIS_SERVO_PINBL,
                    CHASSIS_SERVO_PINFR, CHASSIS_SERVO_PINBR,
                    0, 0, 0, 0);

    // Encoders
    encoders[0] = new Encoder(ENCODER_PIN0);
    encoders[1] = new Encoder(ENCODER_PIN1);
    encoders[2] = new Encoder(ENCODER_PIN2);
    encoders[3] = new Encoder(ENCODER_PIN3);

    printf("Done!\n");

    // Send done starting up
    raspPi->server_send("Setup complete!");

    // Get confirmation to begin unfolding
    memset(raspPi->recvbuf, 0, sizeof(raspPi->recvbuf));
    while (strstr(raspPi->recvbuf, "Unfold!") == NULL) {
        raspPi->server_receive();
        printf("%s\n", raspPi->recvbuf);
    }

    // Send start to unfold
    raspPi->server_send("Unfolding... ");

    // Initialize the rover
    printf("Setting up physical Rover... ");
    if (!initialize()) {
        printf("Could not initialize rover.\n");
        exit(1);
    }
    printf("Done!\n");

    // Send done unfolding
    raspPi->server_send("Unfolding Complete!");

    printf("Rover Ready!\n\n\n");
    raspPi->server_send("Rover Ready!");
    memset(raspPi->recvbuf, 0, sizeof(raspPi->recvbuf));

    // Command Loop
    do {
        // Receive commands from the main computer (Space seperated list)
    	raspPi->server_receive();
        printf("Message Received: \"%s\"\n", raspPi->recvbuf);

        // End connection if received endMsg command
        if (strstr(raspPi->recvbuf, endMsg) != NULL) {
            stop(mode);
            printf("Received End Message from Command.\n");
            break;
        }

        // Set mode to first value
        prev_mode = mode;
        mode = strtol(strtok(raspPi->recvbuf, ","), NULL, 10);

        // Stop the previous modes commands
        if (mode != prev_mode) {
            if (!stop(prev_mode)) {
                printf("Couldn't stop previous mode.\n");
		        raspPi->server_send(failedMsg);
                raspPi->server_send(endMsg);
                exit(1);
            }
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

        // Act on the commands and check for successful completion
        if (!act(command_list, mode)) {
            // Send failed status for bad commands
            raspPi->server_send(failedMsg);
        }

        // Send command completion message
    	raspPi->server_send(complete);
        printf("\n");
    } while (true);

    return 0;
}
