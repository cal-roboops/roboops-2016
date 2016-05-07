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
    bool right = roboclaw->CombinedForwardBackward(RIGHT_ROBOCLAW, RC_COMBINEDFB_ZERO);
    bool left = roboclaw->CombinedForwardBackward(LEFT_ROBOCLAW, RC_COMBINEDFB_ZERO);
    bool arm_base = roboclaw->CombinedForwardBackward(ARM_BASE_ROBOCLAW, RC_COMBINEDFB_ZERO);
    bool arm_extend = roboclaw->CombinedForwardBackward(ARM_EXTEND_ROBOCLAW, RC_COMBINEDFB_ZERO);
    // Make sure all roboclaws are working otherwise there'll be an error
    return true; //(right & left & arm_base & arm_extend);
}

// Set Servos Straight
bool reset_chassis_servos() {
    softServoWrite(DRIVETRAIN_SERVO_PIN_FLBR, SERVO_CENTER);
    softServoWrite(DRIVETRAIN_SERVO_PIN_FRBL, SERVO_CENTER);
    prev_servo_val = SERVO_CENTER;
    return true;
}

// Make Camera Level
bool level_camera() {
    softServoWrite(CAMERA_SERVO_PIN_X, SERVO_CENTER);
    softServoWrite(CAMERA_SERVO_PIN_Y, SERVO_CENTER);
    return true;
}

// Unfold the Rover
bool unfold() {
    // Raise Camera Mast
    softServoWrite(CAMERA_SERVO_PIN_MAST, 1250);
    return true;
}

// Refold the rover (Doesn't do anything currently)
bool fold() {
    return true;
}

// ---------- INITIALIZE -----------

// Initialize and setup the rover from its folded state
bool initialize() {
    // Unfold the rover
    bool setup = unfold();
    // Level the camera
    bool level = level_camera();
    // Stop all roboclaws
    bool robo = stop_roboclaws();
    // Set straighten wheel servos
    bool servo = reset_chassis_servos();

    return (setup &level & robo & servo);
}

// ---------- ACTION MODES -----------

// Command Transmission form (drive):
// Right_RoboClaw, Left_RoboClaw, DT_ServoFL, DT_ServoBL, DT_ServoFR, DT_ServoBR, CameraServoX, CameraServoY
bool drive(char* action[]) {
    // Set servos and wait if changed (currently assumes all servos will be set to the same)
    // Future iterations should check each servo individual for the drive mode
    if (strtol(action[8], NULL, 10) != prev_servo_val) {
	stop_roboclaws();
        softServoWrite(DRIVETRAIN_SERVO_PIN_FLBR, strtol(action[8], NULL, 10));
        softServoWrite(DRIVETRAIN_SERVO_PIN_FRBL, strtol(action[9], NULL, 10));

        // Update saved servo value
        prev_servo_val = strtol(action[8], NULL, 10);

        // Wait for servos to move to position
        delay(2000);

        // Clear any built up commands
        raspPi->server_receive();
        memset(raspPi->recvbuf, 0, sizeof(raspPi->recvbuf));
    }

    // Set Roboclaw Speed and Direction
    bool right = roboclaw->CombinedForwardBackward(RIGHT_ROBOCLAW, strtol(action[0], NULL, 10));
    bool left = roboclaw->CombinedForwardBackward(LEFT_ROBOCLAW, strtol(action[1], NULL, 10));

    // Move Mast Camera
    softServoWrite(CAMERA_SERVO_PIN_X, strtol(action[10], NULL, 10));
    softServoWrite(CAMERA_SERVO_PIN_Y, strtol(action[11], NULL, 10));

    // Make sure all roboclaws are working otherwise there'll be an error
    return true;//(right & left);
}

// Command Transmission form (arm):
// BaseSwivel, BaseJoint, ElbowJoint, ArmExtend, Claw
bool arm(char* action[]) {
    bool arm_baseM1 = roboclaw->ForwardBackwardM1(ARM_BASE_ROBOCLAW, strtol(action[0], NULL, 10));
    bool arm_baseM2 = roboclaw->ForwardBackwardM2(ARM_BASE_ROBOCLAW, strtol(action[1], NULL, 10));
    bool arm_extendM1 = roboclaw->CombinedForwardBackward(ARM_EXTEND_ROBOCLAW, strtol(action[2], NULL, 10));
    bool arm_extendM2 = roboclaw->CombinedForwardBackward(ARM_EXTEND_ROBOCLAW, strtol(action[3], NULL, 10));
    int close_claw = strtol(action[16], NULL, 10) + strtol(action[17], NULL, 10);
    bool claw = true;
    if (close_claw == 1) {
        ; // Close claw halfway
    } else if (close_claw == 2) {
        ; // Close claw fully
    }
    return true;//(arm_baseM1 & arm_baseM2 & arm_extendM1 & arm_extendM2 & claw);
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

// ---------- STOP MOVEMENT -----------

// Stops actions/movements when switching modes or exiting
bool stop(bool done) {
    // Stop roboclaws
    if (!stop_roboclaws()) {
        return false;
    }

    // Set wheel servos to straight
    if (!reset_chassis_servos()) {
        return false;
    }

    // If done running
    if (done) {
        // Set camera straight
        if (!level_camera()) {
            return false;
        }

        // Refold the rover
        if (!fold()) {
            return false;
        }
    }

    return true;
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
        raspPi->server_send("Waiting...");
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

    // Variables
    int i;

    // Motors (Make sure correct UART selected)
    roboclaw = new RoboClaw(UART_PI2);

    // Servos
    softServoSetup(DRIVETRAIN_SERVO_PIN_FLBR, DRIVETRAIN_SERVO_PIN_FRBL,
                    CAMERA_SERVO_PIN_X, CAMERA_SERVO_PIN_Y,
                    0, 0, 0, 0);

    // Encoders (Don't have any encoders)
    //encoders[0] = new Encoder(ENCODER_PIN0);
    //encoders[1] = new Encoder(ENCODER_PIN1);
    //encoders[2] = new Encoder(ENCODER_PIN2);
    //encoders[3] = new Encoder(ENCODER_PIN3);

    printf("Done!\n");

    // Send done starting up
    raspPi->server_send("Setup complete!");

    // Get confirmation to begin unfolding
    memset(raspPi->recvbuf, 0, sizeof(raspPi->recvbuf));
    while (strstr(raspPi->recvbuf, "Unfold!") == NULL) {
        raspPi->server_receive();
        printf("%s\n", raspPi->recvbuf);
        raspPi->server_send("Waiting...");
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
        // mode, motor0, motor1, motor2, motor3, servo0, servo1, servo2, 
        // servo3, servo4, servo5, servo6, servo7
    	raspPi->server_receive();
        printf("Message Received: \"%s\"\n", raspPi->recvbuf);

        // End connection if received endMsg command
        if (strstr(raspPi->recvbuf, endMsg) != NULL) {
            stop(true);
            printf("Received End Message from Command.\n");
            break;
        }

        // Set mode to first value
        prev_mode = mode;
        mode = strtol(strtok(raspPi->recvbuf, ","), NULL, 10);

        // Stop the previous modes commands
        if (mode != prev_mode) {
            if (!stop(false)) {
                printf("Couldn't stop previous mode.\n");
		        raspPi->server_send(failedMsg);
                raspPi->server_send(endMsg);
                break;
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

    // Double check that the roboclaws are stopped
    while (!stop_roboclaws()) {
        // If doesn't stop keeping trying to stop
        ;
    }

    return 0;
}
