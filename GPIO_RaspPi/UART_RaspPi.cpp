//
// UART_RaspPi.cpp
// CPP Project
//
// Created by Mitchell Oleson on 2/23/2016
// 
// Copyright © 2016 Mitchell Oleson. All rights reserved.
//
// Written for Debian Linux
//

#include "UART_RaspPi.h"

// UART Constructor
UART_RaspPi::UART_RaspPi(char* dev) {
	printf("Connecting to UART...\n");

	// Open file stream to UART
	UART_FileStream = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
	if (UART_FileStream == -1) {
		perror("Error - Unable to open UART.");
	}

	// Setup UART options
	uart_setOptions();

	printf("UART Connection Successful!\n");
}

// UART_RaspPi Destructor
UART_RaspPi::~UART_RaspPi() {
	// Close the UART Filestream
	close(UART_FileStream);
}

// Connection Initializer
UART_RaspPi::uart_setOptions() {
	// Initilaize options structure
	tcgetattr(UART_FileStream, &options);

	// Set baud rate and options (see some choices below)
	// B9600, B19200, B38400, B57600, B115200
	options.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;

	// Flush current buffer and prepare for send receive
	tcflush(UART_FileStream, TCIFLUSH);
	tcsetattr(UART_FileStream, TCSANOW, &options);
}

// Send UART commands
int UART_RaspPi::TX(unsigned char tx) {
	if (UART_FileStream != -1) {
		int count = write(UART_FileStream, &tx[0], (int) strlen((char*) tx));
		if (count < 0) {
			perror("UART TX error");
			return 1;
		}
	}
	return 0;
}

// Receive UART response
int UART_RaspPi::RX() {
	// Empty receive buffer
	memset(recvbuf, 0, sizeof(recvbuf));

	// Filestream, buffer to store in, number of bytes to read (max)
	int rx_length = read(UART_FileStream, recvbuf, recvbuflen);

	// Check received values & act
	if (rx_length < 0) {
		perror("UART RX error");
		return 1;
	} else if (rx_length == 0) {
		printf("No Data Waiting.\n")
	} else {
		recvbuf[rx_length] = '\0';
		printf("%i bytes read : %s\n", rx_length, recvbuf);
	}

	return 0;
}