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

// ---------- ACTION SELECTOR -----------

// Takes in the desired mode/actions and acts accordingly
int act(char* action[], int mode) {
    switch (mode) {
        case 0: return car(action); // Drive using car mode
        case 1: return tank(action); // Drive using tank mode
        case 2: return arm(action); // Move the arm
        default: return -1;
    }
}

// ---------- ACTION MODES -----------

// Command Transmission form (car):
// RightRoboClaw, LeftRoboClaw, CServoFL, CServoBL, CServoFR, CServoBR, CameraServo
int car(char* action[]) {
    roboclaw->transmit(action[0]);
    roboclaw->transmit(action[1]);
    softServoWrite(CHASSISSERVOPINFL, strtol(action[2], NULL, 10));
    softServoWrite(CHASSISSERVOPINBL, strtol(action[3], NULL, 10));
    softServoWrite(CHASSISSERVOPINFR, strtol(action[4], NULL, 10));
    softServoWrite(CHASSISSERVOPINBR, strtol(action[5], NULL, 10));
    return 0;
}

// Command Transmission form (tank):
// RightRoboClaw, LeftRoboClaw, CServoFL, CServoBL, CServoFR, CServoBR, CameraServo
int tank(char* action[]) {
    roboclaw->transmit(action[0]);
    roboclaw->transmit(action[1]);
    softServoWrite(CHASSISSERVOPINFL, strtol(action[2], NULL, 10));
    softServoWrite(CHASSISSERVOPINBL, strtol(action[3], NULL, 10));
    softServoWrite(CHASSISSERVOPINFR, strtol(action[4], NULL, 10));
    softServoWrite(CHASSISSERVOPINBR, strtol(action[5], NULL, 10));
    return 0;
}

// Command Transmission form (arm):
// BaseSwivel, BaseJoint, ElbowJoint, ArmExtend, Claw
int arm(char* action[]) {
    return 0;
}

// ---------- ZERO SELECTOR -----------

// Stops actions/movements of the previous mode
int stop(int mode) {
    switch (mode) {
        case 0: return zero_car();
        case 1: return zero_tank();
        case 2: return zero_arm();
        default: return -1;
    }
}

// ---------- ZERO MODES -----------

// Zeros the car mode
int zero_car() {
    return 0;
}

// Zeros the tank mode
int zero_tank() {
    return 0;
}

// Zeros the arm mode
int zero_arm() {
    return 0;
}