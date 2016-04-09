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
void RoboClaw::transmit(int address, int command, int byteValue) {
	flush();
	checksum = (address+command+byteValue) & 0x7F;
	write(fd, address, sizeof(int));
	write(fd, command, sizeof(int));
	write(fd, byteValue, sizeof(int));
	write(fd, checksum, sizeof(int));
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
int int main() {
	// Create RoboClaw object
    printf("Making RoboClaw... ");
    RoboClaw* rc = new RoboClaw(argv[1]);
    printf("Done!\n\n\n");

    // Holder variables
    int add;
    int comm;
    int val;

    // Command Loop
    do {
        printf("Enter address: ");
        scanf("%d", add);
        printf("Enter Command: ");
        scanf("%d", comm);
        printf("Enter byteValue: ");
        scanf("%d", val);
        rc->transmit(add, comm, val);
    } while (true);

	return 0;
}
