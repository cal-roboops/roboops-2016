//
// RoboClaw.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Written for Debian Linux
//

#include <poll.h>
#include <math.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include "../wiringPi/wiringPi/wiringPi.h"
#include "../wiringPi/wiringPi/wiringSerial.h"

// RoboClaw Class
class RoboClaw {
	private:
		// Private Variables
		int uart;
		uint16_t sum;
		uint8_t speed;
		uint16_t sumBytes;
		const char *deviceName;

		// Private Methods
		// transmit (command=single number defined to function in manual)
		int transmit(uint16_t address, int command, uint8_t* data, size_t n_data);

	public:
		// Public Variables
		// Public Methods
		RoboClaw(const char* dev);
		~RoboClaw();
		int setMotorSpeed(uint16_t address, int motor, float value);
};
