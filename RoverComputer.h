//
// RoverComputer.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Debian
//

// Baudrate set to 2400, 9600, 19200, 38400 (for ROBOCLAW)
#define BAUDRATE 38400
#define STOP_ROBOCLAW1
#define STOP_ROBOCLAW2
#define ROBOCLAW_DEVICE_PI2 "/dev/ttyAMA0"
#define ROBOCLAW_DEVICE_PI3 "/dev/ttyS0"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Networking/Server.h"
#include "GPIO_RaspPi/Encoder.h"
#include "GPIO_RaspPi/RoboClaw.h"
#include "Helpers/sharedDefinitions.h"
#include "../wiringPi/wiringPi/wiringPi.h"
#include "../wiringPi/wiringPi/softServo.h"

RoboClaw* roboclaw;
Server* raspPi;
Encoder* encoders[4];
