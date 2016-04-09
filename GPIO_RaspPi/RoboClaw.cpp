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
RoboClaw::RoboClaw(char* dev, int baud) {
	deviceAddress = dev;
	baudrate = baud;

	// Open and initialize serial port
	fd = serialOpen(deviceAddress, baudrate);
	if (fd == -1) {
		printf("Failed to open %s\n", strerror(errno));
		exit(1);
	}
}

// RoboClaw Destructor
RoboClaw::~RoboClaw() {
	serialClose(fd);
}

// Send Commands
void RoboClaw::transmit(uint8_t address, uint8_t command,
			uint8_t *data, size_t n_data) {
	flush();
	uint8_t buf[n_data + 3];
	buf[0] = address;
	buf[1] = command;

	for (size_t i = 0; i < n_data; i++) {
	    buf[i + 2] = data[n_data - i - 1];
	}

	uint16_t sum = 0;

	for (size_t i = 0; i < n_data; i++) {
	    sum += buf[i];
	}

	buf[n_data + 2] = sum & 0x7F;

	write(fd, buf, n_data+3);
	// serialPuts(fd, command);
	// serialPutChar(fd, command);
}

// Clear the send/receive buffers
void RoboClaw::flush() {
	serialFlush(fd);
}

// Main method for RoboClaw testing
// Sets up initial config and begins outputting to the terminal
// Rename to main if compiling only this file
int main() {
    // Create RoboClaw object
    printf("Making RoboClaw... ");
    RoboClaw* rc = new RoboClaw("/dev/ttyAMA0", 38400);
    printf("Done!\n\n\n");

    // Holder variables
    int add;
    int add1 = 0x80;
    int add2 = 0x81;
    int comm;
    int val;

    // Command Loop
    do {
        printf("Enter address (1 or 2): ");
        scanf("%d", &add);
        if (add == 1) {
            add = add1;
        } else {
            add = add2;
        }
        printf("Enter Command: ");
        scanf("%d", &comm);
        printf("Enter byteValue: ");
        scanf("%d", &val);
        rc->transmit((uint8_t) add, (uint8_t) comm, (uint8_t *) &val, 1);
    } while (true);

	return 0;
}
