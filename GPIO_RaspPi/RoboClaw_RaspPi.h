//
// RoboClaw.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Written for Debian Linux
//

#include <stdio.h>
#include <stdlib.h>
#include "../wiringPi/wiringPi/wiringPi.h"
#include "../wiringPi/wiringPi/wiringSerial.h"

// RoboClaw Class
class RoboClaw {
	private:
		// Private Variables
		int TX_pin;
		int RX_pin;
		// Private Methods

	public:
		// Public Variables
		// Public Methods
		int RoboClaw(int tx, int rx);
		int ~RoboClaw();
		int transmit(int command);
};