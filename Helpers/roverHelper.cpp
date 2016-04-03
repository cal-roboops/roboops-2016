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
    // Set wheel servos to straight
    softServoWrite(SERVOPIN0, 500);
    softServoWrite(SERVOPIN1, 500);
    softServoWrite(SERVOPIN2, 500);
    softServoWrite(SERVOPIN3, 500);
    return 0;
}

// ---------- DRIVE MODES -----------

// Command Transmission form (car):
// RightRoboClaw, LeftRoboClaw, Servo0, Servo1, Servo2, Servo3, CameraServo
int car(int[] action) {
    return action[0];
}

// Command Transmission form (tank):
// RightRoboClaw, LeftRoboClaw, Servo0, Servo1, Servo2, Servo3, CameraServo
int tank(int[] action) {
    return action[0];
}

// Command Transmission form (arm):
// BaseSwivel, Base
int arm(int[] action) {
    return action[0];
}

// ---------- ZERO SELECTOR -----------

// Stops actions/movements of the previous mode
int stop(int mode) {
    switch (mode) {
        case 0: return zero_car();
        case 1: return zero_tank();
        case 2: return zero_arm();
        default: return 1;
    }
}

// ---------- ACTION SELECTOR -----------

// Takes in the action and acts accordingly
int act(int[] action, int mode) {
    switch (mode) {
        case 0: return car(action); // Drive using car mode
        case 1: return tank(action); // Drive using tank mode
        case 2: return arm(action); // Move the arm
        default: return 1;
    }
}