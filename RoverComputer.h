//
// RoverComputer.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Debian
//

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "Definitions.h"
#include "Networking/Server.h"

int car(int[] action);
int tank(int[] action);
int arm(int[] action);
int act(int[] action, int mode);
int initialize();