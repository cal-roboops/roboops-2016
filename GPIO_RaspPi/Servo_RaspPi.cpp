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

	// Create Software Servo channel on pin
	int status = softServoSetup(pin);
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
	softServoWrite(servo_pin, curr_spin);
}

// Update the servo spin by X
void Servo_RaspPi::update(int x) {
	set(x + servo_range);
}

// Set servo spin to X
void Servo_RaspPi::set(int x) {
	if (x < servo_range) {
		curr_spin = x;
		spin();
	} else {
		printf("%d is beyond max servo range (%d).\n", x, servo_range);
	}
}

// Main method for Servo testing
// Sets up basic servo config and outputs data terminal
// Rename to main if compiling only this file
int main(int argc, char **argv) {
	// Validte parameters
	if (argc != 3) {
        printf("Usage: %s <pin> <range>\n", argv[0]);
        return -1;
    }

    // Create Encoder object
    printf("Making Servo... ");
    Servo_RaspPi s = new Servo_RaspPi(argv[1], argv[2]);
    printf("Done!\n");

    // Hold variables
    int mode;
    int value;

    // Servo Control Loop
    while (true) {
    	printf("Set (1) or update (2) mode: ");
    	scanf("%d", mode);
    	printf("To value: ");
    	scanf("%d", value);
    	switch (mode) {
    		case 1: s->set(value);
    		case 2: s->update(value);
    		case 3: return 0;
    	}
    	printf("%d\n", s->value());
    	delay(1000);
    }

	return 0;
}