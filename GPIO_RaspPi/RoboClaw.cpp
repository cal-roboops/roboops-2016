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
void RoboClaw::transmit(char* command) {
	write(fd, command, sizeof(char)*strlen(command));
	// serialPuts(fd, command);
	// serialPutChar(fd, command);
}

// Clear the send/receive buffers
void RoboClaw::clear() {
	serialFlush(fd);
}
