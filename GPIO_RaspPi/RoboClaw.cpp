//
// RoboClaw.cpp
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Written for Debian Linux
//

#include "RoboClaw.h"

// RoboClaw Constructor
RoboClaw::RoboClaw(const char *dev) {
	// Set constants
	deviceName = dev;

	// Open and initialize serial port
	uart = open(deviceName, O_RDWR | O_NOCTTY);
	if (uart < 0) {
		printf("Failed to open device: %s\n", strerror(errno));
		exit(1);
	}

	int ret = 0;
	struct termios uart_config;
	ret = tcgetattr(uart, &uart_config);
	if (ret < 0) {
		printf("failed to get attr.\n");
		exit(1);
	}

	uart_config.c_oflag &= ~ONLCR;
	ret = cfsetispeed(&uart_config, B38400);
	if (ret < 0) {
		printf("failed to set input speed.\n");
		exit(1);
	}

	ret = cfsetospeed(&uart_config, B38400);
	if (ret < 0) {
		printf("failed to set output speed.\n");
		exit(1);
	}

	ret = tcsetattr(uart, TCSANOW, &uart_config);
	if (ret < 0) {
		printf("failed to set attr.\n");
		exit(1);
	}
}

// RoboClaw Destructor
RoboClaw::~RoboClaw() {
	close(uart);
}

// Set Motor Speed
int RoboClaw::setMotorSpeed(uint8_t address, int motor, float value) {
	sum = 0;

	// Bound
	if (value > 1) {
		value = 1;
	} else if (value < 1) {
		value = -1;
	}

	// Set speed
	speed = fabs(value) * 127;

	// Send the command
	if (motor == 1) {
		if (value > 0) {
			return transmit(&address, 0, &speed, 1);
		} else {
			return transmit(&address, 1, &speed, 1);
		}
	} else if (motor == 2) {
		if (value > 0) {
			return transmit(&address, 4, &speed, 1);
		} else {
			return transmit(&address, 5, &speed, 1);
		}
	}

	return -1;
}

// Send Commands
int RoboClaw::transmit(uint8_t* address, int command,
			uint8_t* data, size_t n_data) {
	// Clear buffers of any pending data
	tcflush(uart, TCIOFLUSH);

	// Setup command list
	uint8_t buf[n_data + 3];
	buf[0] = *address;
	buf[1] = command;

	// Create the transmission list
	for (size_t i = 0; i < n_data; i++) {
	    buf[i + 2] = data[n_data - i - 1];
	}

	// Sum buffer bytes
	sumBytes = 0;
	for (size_t i = 0; i < n_data; i++) {
	    sumBytes += buf[i];
	}

	// Create checksum
	sum += sumBytes;
	buf[n_data + 2] = sumBytes & 0x7F;

	return write(uart, buf, n_data + 3);
}

// Main method for RoboClaw testing
// Sets up initial config and begins outputting to the terminal
// Rename to main if compiling only this file
int main() {
    // Create RoboClaw object
    printf("Making RoboClaw... ");
    RoboClaw* rc = new RoboClaw("/dev/ttyAMA0");
    printf("Done!\n\n\n");

    // Holder variables
    int add;
    uint8_t add1 = 0x80;
    uint8_t add2 = 0x81;
    int mot;
    float val;

    // Command Loop
    do {
        printf("Enter address (1 or 2): ");
        scanf("%d", &add);
        printf("Enter Motor (1 or 2): ");
        scanf("%d", &mot);
        printf("Enter byteValue (-1 < x < 1): ");
        scanf("%f", &val);
        printf("%d\n", mot);
        printf("%d\n", mot);
        printf("%f\n", val);
        if (add == 1) {
            rc->setMotorSpeed(add1, mot, val);
        } else if (add == 2) {
            rc->setMotorSpeed(add2, mot, val);
        }
    } while (true);

	return 0;
}
