//
// roverHelper.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Debian
//

#include <stdio.h>
#include <stdlib.h>

// ---------- INITIALIZE -----------
int initialize();

// ---------- ACTION SELECTOR -----------
int act(char* action[], int mode);

// ---------- ACTION MODES -----------
int car(char* action[]);
int tank(char* action[]);
int arm(char* action[]);

// ---------- ZERO SELECTOR -----------
int stop(int mode);

// ---------- ZERO MODES -----------
int zero_car();
int zero_tank();
int zero_arm();