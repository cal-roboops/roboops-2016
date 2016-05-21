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
#define RC_FB_ZERO 0
#define RC_FB_MAX 127
#define SERVO_CENTER 500
#define SERVO_45_Degrees 315
#define SERVO_90_Degrees 105

#define MODE0 0
#define MODE1 1
#define MODE2 2

char* port = (char*) "8080";
char* ipve = (char*) "192.168.137.52";
char* ipv4 = (char*) "25.83.200.132";
char* ipv6 = (char*) "2601:644:102:7600::c692";
const char* endMsg = "Done!";
const char* failedMsg = "Failed...";
const char* complete = "Finished running commands.";
