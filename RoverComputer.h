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

#define RIGHT_ROBOCLAW 0x80
#define LEFT_ROBOCLAW 0x81
#define ARM_ROBOCLAW 0x82

#define DRIVETRAIN_SERVO_PIN_FL 0
#define DRIVETRAIN_SERVO_PIN_BL 1
#define DRIVETRAIN_SERVO_PIN_FR 2
#define DRIVETRAIN_SERVO_PIN_BR 3

#define ARM_SERVO_PIN_ELBOW 4
#define ARM_SERVO_PIN_FOREARM 5
#define ARM_SERVO_PIN_BASE 6

#define CAMERA_SERVO_PIN_MAST 7
#define CAMERA_SERVO_PIN_X 8
#define CAMERA_SERVO_PIN_Y 9

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
