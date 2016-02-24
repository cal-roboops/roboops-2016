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
UART_RaspPi::UART_RaspPi(char* dev, int baud) {
	printf("Connecting to UART...\n");

	UART_FileStream = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
	if (UART_FileStream == -1) {
		printf("");
	}

	printf("UART Connection Successful!\n");
}