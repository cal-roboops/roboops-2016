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
		int checksum;
		char* deviceAddress;

		// Private Methods
		void flush();

	public:
		// Public Variables
		// Public Methods
		RoboClaw(char* dev, int baud);
		~RoboClaw();
		void RoboClaw::transmit(uint8_t address, uint8_t command, 
								uint8_t *data, size_t n_data)
};
