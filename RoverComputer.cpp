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

// Stop Roboclaw
void stop_roboclaws() {
    const char* temp = "roboclaw.ForwardBackwardMixed(128, 0)\n";
    PyRun_SimpleString(temp);
//    PyObject_CallFunction(roboclaw_FB, (char*) "(ii)", (char*) RIGHT_ROBOCLAW, (char*) ZERO);
//    PyObject_CallFunction(roboclaw_FB, (char*) "(ii)", (char*) LEFT_ROBOCLAW, (char*) ZERO);
}

// Set Servos Straight
void reset_chassis_servos() {
    softServoWrite(CHASSIS_SERVO_PINFL, 500);
    softServoWrite(CHASSIS_SERVO_PINBL, 500);
    softServoWrite(CHASSIS_SERVO_PINFR, 500);
    softServoWrite(CHASSIS_SERVO_PINBR, 500);
}

// ---------- INITIALIZE -----------

// Initialize and setup the rover from its folded state
int initialize() {
    // Set wheel servos to straight
    stop_roboclaws();
    reset_chassis_servos();
    return 0;
}

// ---------- ACTION MODES -----------

// Command Transmission form (drive):
// Right_RoboClaw, Left_RoboClaw, CServoFL, CServoBL, CServoFR, CServoBR, CameraServo
int drive(char* action[]) {
    PyObject_CallFunction(roboclaw_FB, (char*) "(ii)", (char*) RIGHT_ROBOCLAW, action[0]);
    PyObject_CallFunction(roboclaw_FB, (char*) "(ii)", (char*) LEFT_ROBOCLAW, action[1]);
    softServoWrite(CHASSIS_SERVO_PINFL, strtol(action[2], NULL, 10));
    softServoWrite(CHASSIS_SERVO_PINBL, strtol(action[3], NULL, 10));
    softServoWrite(CHASSIS_SERVO_PINFR, strtol(action[4], NULL, 10));
    softServoWrite(CHASSIS_SERVO_PINBR, strtol(action[5], NULL, 10));
    return 0;
}

// Command Transmission form (arm):
// BaseSwivel, BaseJoint, ElbowJoint, ArmExtend, Claw
int arm(char* action[]) {
    return 0;
}

// ---------- ACTION SELECTOR -----------

// Takes in the desired mode/actions and acts accordingly
int act(char* action[], int mode) {
    switch (mode) {
        case 0:
        case 1: return drive(action); // Drive using car mode
        case 2: return arm(action); // Move the arm
        default: return -1;
    }
}

// ---------- ZERO MODES -----------

// Zeros the drive mode
int zero_drive() {
    stop_roboclaws();
    reset_chassis_servos();
    return 0;
}

// Zeros the arm mode
int zero_arm() {
    return 0;
}

// ---------- ZERO SELECTOR -----------

// Stops actions/movements of the previous mode
int stop(int mode) {
    switch (mode) {
        case 0:
        case 1: return zero_drive();
        case 2: return zero_arm();
        default: return -1;
    }
}

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
    raspPi = new Server(port);
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

    // Indexing & result variables
    int i;
    int res;

    // Motors (Using embedded python code)
    // Setup Python
    Py_Initialize();
    // Import Modules and Set Path
    PyRun_SimpleString("import sys\n");
    PyRun_SimpleString("sys.path.append(\"./GPIO_RaspPi\")\n");
    PyRun_SimpleString("import roboclaw\n");
    PyRun_SimpleString("roboclaw.Open(\"/dev/ttyAMA0\", 38400)\n");
//    PyRun_SimpleString("roboclaw.ForwardBackwardMixed(128, 50)\n");
//    roboclaw_module = PyImport_Import(PyString_FromString((char*) "roboclaw"));
//    roboclaw_Open = PyObject_GetAttrString(roboclaw_module, (char*) "Open");
//    roboclaw_FB = PyObject_GetAttrString(roboclaw_module, (char*) "ForwardBackwardMixed");
//    if (roboclaw_Open == NULL || roboclaw_FB == NULL) {
//	printf("Failed to import Python");
//    }
    // Open UART
//    PyObject_CallMethodObjArgs(roboclaw_module, roboclaw_Open, (char*) "(si)", (char*) UART_PI2, (char*) BAUDRATE);
//    PyObject_CallMethodObjArgs(roboclaw_module, roboclaw_FB, (char*) "(ii)", (char*) RIGHT_ROBOCLAW, (char*) "50");
//    PyObject_CallMethodObjArgs(roboclaw_FB, PyTuple_Pack(2, PyInt_FromString((char*) RIGHT_ROBOCLAW, NULL, 10), PyInt_FromString((char*) "50", NULL, 10)));

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
            if (stop(prev_mode) != 0) {
                printf("Couldn't stop previous mode.\n");
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
