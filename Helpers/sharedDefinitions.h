//
// sharedDefinitions.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Windows/Debian
//

#define DEFAULT_BUFLEN 512
#define RC_COMBINEDFB_ZERO 64
#define SERVO_CENTER 500
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

#define CAMERA_SERVO_PIN_MAST 9
#define CAMERA_SERVO_PIN_X 10
#define CAMERA_SERVO_PIN_Y 11

char* port = (char*) "8080";
char* ip = (char*) "192.168.137.40";
const char* endMsg = "Done!";
const char* failedMsg = "Failed...";
const char* complete = "Finished running commands.";