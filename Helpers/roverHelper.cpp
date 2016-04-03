//
// roverHelper.cpp
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Made for Debian
//


// ---------- INITIALIZE -----------

// Initialize and setup the rover from its folded state
int initialize() {
    return 0;
}

// ---------- DRIVE MODES -----------

// Command Transmission form (car):
// RightRoboClaw, LeftRoboClaw, Servo1, Servo2, Servo3, Servo4, CameraServo
int car(int[] action) {
    return action[0];
}

// Command Transmission form (tank):
// RightRoboClaw, LeftRoboClaw, Servo1, Servo2, Servo3, Servo4, CameraServo
int tank(int[] action) {
    return action[0];
}

// Command Transmission form (arm):
// BaseSwivel, Base
int arm(int[] action) {
    return action[0];
}

// Stops actions/movements of the previous mode
int stop(int mode) {
    return 0;
}

// ---------- DRIVE SELECTOR -----------

// Takes in the action and acts accordingly
int act(int[] action, int mode) {
    switch (mode) {
        case 0: return car(action); // Drive using car mode
        case 1: return tank(action); // Drive using tank mode
        case 2: return arm(action); // Move the arm
        default: return mode;
    }
}