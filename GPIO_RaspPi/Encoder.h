//
// Encoder_RaspPi.h
// CPP Project
//
// Created by Mitchell Oleson on 2/26/2016
//
// Written for Debian Linux
//

#include <stdio.h>
#include <stdlib.h>
#include "../wiringPi/wiringPi/wiringPi.h"

// Encoder_RaspPi class
class Encoder {
	private:
		// Private Variables
		int encoder_pin;

		// Private Methods

	public:
		// Public Variables

		// Public Methods
		Encoder(int pin);
		~Encoder();
		int value();
};
