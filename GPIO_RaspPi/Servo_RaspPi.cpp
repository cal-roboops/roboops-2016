//
// Servo_RaspPi.cpp
// CPP Project
//
// Created by Mitchell Oleson on 2/26/2016
//
// Written for Debian Linux
//

#include "Servo_RaspPi.h"

// Servo Constructor
Servo_RaspPi::Servo_RaspPi(int pin, int range) {
	//printf("Creating Servo...\n");

	// Setup pin
	servo_pin = pin;
	servo_range = range;
	curr_spin = 0;

	// Create PWM channel on pin
	int status = softPwmCreate(pin, 0, range);
	if (status != 0) {
		perror("Error creating software PWM.");
	}

	//printf("Servo Creation Successful!\n");
}

// Servo Destructor
Servo_RaspPi::~Servo_RaspPi() {
	;
}

// Spin servo to the value of curr_spin
Servo_RaspPi::spin() {
	softPwmWrite(servo_pin, curr_spin);
}

// Update the servo spin by X
void Servo_RaspPi::update(int x) {
	curr_spin += x;
	spin();
}

// Set servo spin to X
void Servo_RaspPi::set(int x) {
	curr_spin = x;
	spin();
}