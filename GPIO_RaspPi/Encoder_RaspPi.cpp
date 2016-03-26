//
// Encoder_RaspPi.cpp
// CPP Project
//
// Created by Mitchell Oleson on 2/26/2016
// 
// Copyright © 2016 Mitchell Oleson. All rights reserved.
//
// Written for Debian Linux
//

#include "Encoder_RaspPi.h"

Encoder_RaspPi::Encoder_RaspPi(int pin) {
	printf("Creating PWM...\n");

	// Setup pin
	encoder_pin = pin;

	// Create channel on pin
	pinMode(pin, INPUT);

	printf("PMW Creation Successful!\n");
}

Encoder_RaspPi::~Encoder_RaspPi() {
	;
}

int Encoder_RaspPi::value() {
	curr_value = digitalRead(encoder_pin);
	return curr_value;
}