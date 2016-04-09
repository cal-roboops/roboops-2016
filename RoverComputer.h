//
// RoverComputer.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Debian
//

// Baudrate set to 9600, 115200 (Default), 19200, 38400
#define BAUDRATE 115200
#define ROBOCLAWDEVICEPI2 "/dev/ttyAMA0"
#define ROBOCLAWDEVICEPI3 "/dev/ttyS0"

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
