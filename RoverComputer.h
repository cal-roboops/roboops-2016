//
// RoverComputer.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Debian
//

// Baudrate = 2400, 9600, 19200, 38400
#define BAUDRATE 38400
#define UART_PI2 "/dev/ttyAMA0"
#define UART_PI3 "/dev/ttyS0"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Python.h>
#include "Networking/Server.h"
#include "GPIO_RaspPi/Encoder.h"
#include "Helpers/sharedDefinitions.h"
#include "../wiringPi/wiringPi/wiringPi.h"
#include "../wiringPi/wiringPi/softServo.h"

Server* raspPi;
Encoder* encoders[4];
PyObject* roboclaw_module;
PyObject* roboclaw_O, roboclaw_FB;
PyObject* pArgs;