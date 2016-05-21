//
// RoverComputer.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Debian
//

#define UART_PI2 "/dev/ttyAMA0"
// Pi 3 UART does not work since it is controlled by
// the CPU clock which too unstable
#define UART_PI3 "/dev/ttyS0"

#define RIGHT_ROBOCLAW 0x80
#define LEFT_ROBOCLAW 0x81
#define ARM_BASE_ROBOCLAW 0x82
#define ARM_EXTEND_ROBOCLAW 0x83

#define DRIVETRAIN_SERVO_PIN_FLBR 0
#define DRIVETRAIN_SERVO_PIN_FRBL 1

#define ARM_SERVO_PIN_ELBOW 4
#define ARM_SERVO_PIN_FOREARM 5
#define ARM_SERVO_PIN_BASE 6

#define CAMERA_SERVO_PIN_MAST 7
#define CAMERA_SERVO_PIN_X 8
#define CAMERA_SERVO_PIN_Y 9

#define ARM_MAX_SPEED 5

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Networking/Server.h"
#include "GPIO_RaspPi/RoboClaw.h"
#include "Helpers/sharedDefinitions.h"
#include "../wiringPi/wiringPi/wiringPi.h"
#include "../wiringPi/wiringPi/softServo.h"

Server* raspPi;
RoboClaw* roboclaw;
int prev_servo_val = -1;