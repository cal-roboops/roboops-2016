//
// RoboClaw.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Written for Debian Linux
//

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
		int calculate_checksum(int address, int command, int byteValue);

	public:
		// Public Variables
		// Public Methods
		RoboClaw(char* dev, int baud);
		~RoboClaw();
		void transmit(char* command);
		void clear();
};
