//
// RoverComputer.h
// CPP Project
//
// Created by Mitchell Oleson on 3/26/2016
// 
// Copyright © 2016 Mitchell Oleson. All rights reserved.
//
// Written for Debian Linux
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
#include <WiringPi.h>
#include "../Client_ServerCode/Server.h"

private:
	// Private Variables
	int res; // Result of carrying out operation
	int mode; // Car, Tank, or Arm Modes (0, 1, 2)
	char* command; // Command parsed from recieved data

    // Rover Responses
    const char* endMsg = "Done!";
    const char* good = "All is good.";
    const char* bad = "Something isn't right: ";

	// Private Methods
	int act(int* action);

public:
	// Public Variables

	// Public Methods
	int main(int argc,char **argv);