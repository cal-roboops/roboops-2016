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
	write(fd, address, sizeof(int));
	write(fd, command, sizeof(int));
	write(fd, byteValue, sizeof(int));
	write(fd, calculate_checksum(address, command, byteValue), sizeof(int));
	// serialPuts(fd, command);
	// serialPutChar(fd, command);
}

int calculate_checksum(int address, int command, int byteValue) {
	return ((address+command+byteValue) & 0x7F);
}

// Clear the send/receive buffers
void RoboClaw::clear() {
	serialFlush(fd);
}
