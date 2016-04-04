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
		int fd; // File descriptor
		int baudrate;
		char* deviceAddress;

		// Private Methods

	public:
		// Public Variables
		// Public Methods
		RoboClaw(char* dev);
		~RoboClaw();
		void transmit(char* command);
		void clear();
};