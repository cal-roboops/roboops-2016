//
// Encoder_RaspPi.cpp
// CPP Project
//
// Created by Mitchell Oleson on 2/26/2016
//
// Written for Debian Linux
//

#include "Encoder_RaspPi.h"

Encoder_RaspPi::Encoder_RaspPi(int pin) {
	//printf("Creating Encoder...\n");

	// Setup pin
	encoder_pin = pin;

	// Create channel on pin
	pinMode(pin, INPUT);

	//printf("Encoder Creation Successful!\n");
}

Encoder_RaspPi::~Encoder_RaspPi() {
	;
}

int Encoder_RaspPi::value() {
	return digitalRead(encoder_pin);
}

// Main method for Encoder testing
// Sets up basic encoder config and outputs data terminal
// Rename to main if compiling only this file
int main_encoder(int argc, char **argv) {
	// Validte parameters
	if (argc != 2) {
        printf("Usage: %s <pin>\n", argv[0]);
        return -1;
    }

    // Create Encoder object
    printf("Making Encoder... ");
    Encoder_RaspPi e = new Encoder_RaspPi(argv[1]);
    printf("Done!\n");

    // Output value to terminal once per second
    while (true) {
    	printf("%d\n", e->value());
    	delay(1000);
    }

	return 0;
}