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

#define DRIVE_ROBOCLAW 0x80
#define ARM_BASE_ROBOCLAW 0x81
#define ARM_EXTEND_ROBOCLAW 0x82

#define DRIVETRAIN_SERVO_PIN_FRBL 0
#define DRIVETRAIN_SERVO_PIN_FLBR 1

#define CAMERA_SERVO_PIN_X 2
#define CAMERA_SERVO_PIN_Y 3

#define CLAW_PIN 4

#define ARM_MAX_SPEED 5

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
int prev_servo_val = -1;
