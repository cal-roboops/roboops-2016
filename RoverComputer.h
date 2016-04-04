//
// RoverComputer.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Debian
//

#define BAUDRATE 38400 // Can also be set to 9600, 115200, 19200, 38400
#define ROBOCLAWDEVICE "/dev/ttyAMA0"

#include <stdio.h>
#include <stdlib.h>
#include "Networking/Server.h"
#include "GPIO_RaspPi/Encoder.h"
#include "GPIO_RaspPi/RoboClaw.h"
#include "Helpers/sharedDefinitions.h"
#include "../wiringPi/wiringPi/wiringPi.h"
#include "../wiringPi/wiringPi/softServo.h"