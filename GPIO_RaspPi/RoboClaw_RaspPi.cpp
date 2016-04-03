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
RoboClaw::RoboClaw(int tx, int rx) {
	TX_pin = tx;
	RX_pin = rx;
}

// RoboClaw Destructor
RoboClaw::~RoboClaw() {
	;
}

// Send Commands
int RoboClaw::transmit(int command) {
	serialWrite(tx_pin, command);
	return 0;
}