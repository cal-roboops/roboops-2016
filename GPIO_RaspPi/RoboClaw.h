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
		uint16_t speed;
		uint16_t sumBytes;
		const char *deviceName;

		// Private Methods
		int transmit(uint8_t address, uint8_t command,
						uint8_t *data, size_t n_data);

		// enum
		// Command functions from manual
		enum e_command {
			// simple drive commands
			CMD_DRIVE_FWD_1 = 0,
			CMD_DRIVE_REV_1 = 1,
			CMD_DRIVE_FWD_2 = 4,
			CMD_DRIVE_REV_2 = 5,
		}

	public:
		// Public Variables
		// Public Methods
		RoboClaw(const char* dev, int baud);
		~RoboClaw();
		int setMotorSpeed(uint8_t address, int motor, float value);
};
