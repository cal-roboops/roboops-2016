//
// sharedDefinitions.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Windows/Debian
//

#define DEFAULT_BUFLEN 512
#define RMIXED_ZERO 64
#define SERVO_CENTER 500
#define LEFT_ROBOCLAW 0x80
#define RIGHT_ROBOCLAW 0x81

#define CHASSIS_SERVO_PINFL 0
#define CHASSIS_SERVO_PINBL 1
#define CHASSIS_SERVO_PINFR 2
#define CHASSIS_SERVO_PINBR 3

#define ENCODER_PIN0 6
#define ENCODER_PIN1 7
#define ENCODER_PIN2 8
#define ENCODER_PIN3 9

#define CAMERA_SERVO_PIN 10

char* port = (char*) "8080";
const char* endMsg = "Done!";
const char* failedMsg = "Failed...";
const char* complete = "Finished running commands.";