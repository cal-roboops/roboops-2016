//
// RoverComputer.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Debian
//

#define UART_PI2 "/dev/ttyAMA0"
#define UART_PI3 "/dev/ttyS0"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Networking/Server.h"
#include "GPIO_RaspPi/Encoder.h"
#include "GPIO_RaspPi/RoboClaw.h"
#include "Helpers/sharedDefinitions.h"
#include "../wiringPi/wiringPi/wiringPi.h"
#include "../wiringPi/wiringPi/softServo.h"

Server* raspPi;
RoboClaw* roboclaw;
Encoder* encoders[4];
