//
// UART_RaspPi.h
// CPP Project
//
// Created by Mitchell Oleson on 2/23/2016
// 
// Copyright © 2016 Mitchell Oleson. All rights reserved.
//
// Written for Debian Linux
//

#define DEFAULT_BUFLEN 256
#define BAUDRATE B38400

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

// UART_RaspPi Class
class UART_RaspPi {
	private:
		// Private Variables
		int UART_FileStream = -1;
		struct termios options;
		static const int recvbuflen = DEFAULT_BUFLEN;

		// Private Methods
		uart_setOptions();

	public:
		// Public Variables
		char recvbuf[DEFAULT_BUFLEN];

		// Public Methods
		UART_RaspPi::UART_RaspPi();
		UART_RaspPi::~UART_RaspPi();
		int TX();
		int RX();
};