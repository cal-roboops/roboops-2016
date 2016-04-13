//
// RoverComputer.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Debian
//

// Baudrate = 2400, 9600, 19200, 38400
#define SETUP_UART_PI2 "roboclaw.Open(\"/dev/ttyAMA0\", 38400)\n"
#define SETUP_UART_PI3 "roboclaw.Open(\"/dev/ttyS0\", 38400)\n"
#define R1_ZERO "roboclaw.ForwardBackwardMixed(128, 64)"
#define R2_ZERO "roboclaw.ForwardBackwardMixed(129, 64)"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Python.h>
#include "Networking/Server.h"
#include "GPIO_RaspPi/Encoder.h"
#include "Helpers/sharedDefinitions.h"
#include "../wiringPi/wiringPi/wiringPi.h"
#include "../wiringPi/wiringPi/softServo.h"