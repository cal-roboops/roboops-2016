//
// RoverComputer.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Debian
//

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
PyObject* pResult;
char* port;
char* pArgs;
