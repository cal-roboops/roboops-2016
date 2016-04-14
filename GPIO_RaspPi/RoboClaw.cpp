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
RoboClaw::RoboClaw(const char* dev, uint32_t tout) {
	// Set constants
	deviceName = dev;
	timeout = tout;

	// Open and initialize serial port
	uart = open(deviceName, O_RDWR | O_NOCTTY | O_NDELAY);
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

// RoboClaw Clear Checksum
void RoboClaw::crc_clear() {
	crc = 0;
}

// RoboClaw Update Checksum
void RoboClaw::crc_update(uint8_t data) {
	int i;
	crc = crc ^ ((uint16_t) data << 8);
	for (i=0; i<8; i++) {
		if (crc & 0x8000) {
			crc = (crc << 1) ^ 0x1021;
		} else {
			crc <<= 1;
		}
	}
}

// RoboClaw Get Checksum
uint16_t RoboClaw::crc_get() {
	return crc;
}

// RoboClaw Write
bool RoboClaw::write_n(uint8_t cnt, ... ) {
	uint8_t trys=MAXRETRY;

	do {
		// Clear Checksum
		crc_clear();

		// Send data with crc
		va_list marker;
		// Initialize variable arguments
		va_start(marker, cnt);
		for (uint8_t index=0; index<cnt; index++) {
			uint8_t data = va_arg(marker, int);
			crc_update(data);
			write(uart, &data, 1);
		}

		// Reset variable arguments
		va_end(marker);
		uint16_t crc = crc_get();
		uint16_t c = crc >> 8;
		write(uart, &c, 1);
		write(uart, &crc, 1);

	} while(trys--);

	return false;
}

// RoboClaw M1 Forward
bool RoboClaw::ForwardM1(uint8_t address, uint8_t speed) {
	return write_n(3,address,M1FORWARD,speed);
}

// RoboClaw M1 Backward
bool RoboClaw::BackwardM1(uint8_t address, uint8_t speed) {
	return write_n(3,address,M1BACKWARD,speed);
}

// RoboClaw Set Min Voltage Level
bool RoboClaw::SetMinVoltageMainBattery(uint8_t address, uint8_t voltage){
	return write_n(3,address,SETMINMB,voltage);
}

// RoboClaw Set Max Voltage Level
bool RoboClaw::SetMaxVoltageMainBattery(uint8_t address, uint8_t voltage){
	return write_n(3,address,SETMAXMB,voltage);
}

// RoboClaw M2 Forward
bool RoboClaw::ForwardM2(uint8_t address, uint8_t speed) {
	return write_n(3,address,M2FORWARD,speed);
}

// RoboClaw M2 Backward
bool RoboClaw::BackwardM2(uint8_t address, uint8_t speed) {
	return write_n(3,address,M2BACKWARD,speed);
}

// RoboClaw M1 ForwardBackWard
bool RoboClaw::ForwardBackwardM1(uint8_t address, uint8_t speed){
	return write_n(3,address,M17BIT,speed);
}

// RoboClaw M2 ForwardBackWard
bool RoboClaw::ForwardBackwardM2(uint8_t address, uint8_t speed){
	return write_n(3,address,M27BIT,speed);
}

// RoboClaw Combined Forward
bool RoboClaw::CombinedForward(uint8_t address, uint8_t speed) {
	if (!ForwardM1(address, speed)) {
		return false;
	} else if (!ForwardM2(address, speed)) {
		return false;
	} else {
		return true;
	}
}

// RoboClaw Combined Backward
bool RoboClaw::CombinedBackward(uint8_t address, uint8_t speed) {
	if (!BackwardM1(address, speed)) {
		return false;
	} else if (!BackwardM2(address, speed)) {
		return false;
	} else {
		return true;
	}
}

// RoboClaw Combined ForwardBackward
bool RoboClaw::CombinedForwardBackward(uint8_t address, uint8_t speed) {
	if (!ForwardBackwardM1(address, speed)) {
		return false;
	} else if (!ForwardBackwardM2(address, speed)) {
		return false;
	} else {
		return true;
	}
}

// Main method for RoboClaw testing
// Sets up initial config and begins outputting to the terminal
// Rename to main if compiling only this file
int main_roboclaw() {
    // Create RoboClaw object
    printf("Making RoboClaw... ");
    RoboClaw* rc = new RoboClaw("/dev/ttyAMA0", (uint32_t) 10);
    printf("Done!\n\n\n");

    // Holder variables
    int add;
    uint8_t add1 = 0x80;
    uint8_t add2 = 0x81;
    int val;
    bool res;

    // Command Loop
    do {
        printf("Enter address (1 or 2): ");
        scanf("%d", &add);
        printf("Enter byteValue (-1 < x < 1): ");
        scanf("%d", &val);
        if (add == 1) {
            res = rc->CombinedForwardBackward(add1, val);
        } else if (add == 2) {
            res = rc->CombinedForwardBackward(add2, val);
        }
	printf("%d\n", res);
    } while (true);

	return 0;
}
