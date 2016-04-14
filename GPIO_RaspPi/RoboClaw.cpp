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

// RoboClaw Clear Checksum
void Roboclaw::crc_clear() {
	checksum = 0;
}

// RoboClaw Update Checksum
void Roboclaw::crc_update(uint8_t data) {
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
			write(data);
		}

		// Reset variable arguments
		va_end(marker);
		uint16_t crc = crc_get();
		write(crc >> 8);
		write(crc);

		if(read(timeout) == 0xFF) {
			return true;
		}
	} while(trys--);

	return false;
}

// RoboClaw Read
bool RoboClaw::read_n(uint8_t cmd,uint8_t address,uint8_t cnt,...) {
	uint32_t value=0;
	uint8_t trys=MAXRETRY;
	int16_t data;

	do {
		flush();
		
		data = 0;
		crc_clear();
		write(address);
		crc_update(address);
		write(cmd);
		crc_update(cmd);

		// Send data with crc
		va_list marker;
		// Initialize variable arguments
		va_start(marker, cnt);
		for (uint8_t index=0; index<cnt; index++) {
			uint32_t *ptr = (uint32_t *)va_arg(marker, int);

			if (data != -1) {
				data = read(timeout);
				crc_update(data);
				value = (uint32_t) data << 24;
			} else {
				break;
			}
			
			if (data != -1) {
				data = read(timeout);
				crc_update(data);
				value|=(uint32_t)data<<16;
			} else {
				break;
			}

			if (data != -1) {
				data = read(timeout);
				crc_update(data);
				value|=(uint32_t)data<<8;
			} else {
				break;
			}

			if (data != -1) {
				data = read(timeout);
				crc_update(data);
				value|=(uint32_t)data;
			} else {
				break;
			}

			*ptr = value;
		}

		// Reset variable arguments
		va_end(marker);

		if (data != -1) {
			uint16_t ccrc;
			data = read(timeout);
			if (data != -1) {
				ccrc = data << 8;
				data = read(timeout);
				if (data != -1) {
					ccrc |= data;
					return crc_get()==ccrc;
				}
			}
		}
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

// RoboClaw M2 Forward
bool RoboClaw::ForwardM2(uint8_t address, uint8_t speed) {
	return write_n(3,address,M2FORWARD,speed);
}

// RoboClaw M2 Backward
bool RoboClaw::BackwardM2(uint8_t address, uint8_t speed) {
	return write_n(3,address,M2BACKWARD,speed);
}

// RoboClaw Mixed Forward
bool RoboClaw::ForwardMixed(uint8_t address, uint8_t speed) {
	return write_n(3,address,MIXEDFORWARD,speed);
}

// RoboClaw Mixed Backward
bool RoboClaw::BackwardMixed(uint8_t address, uint8_t speed) {
	return write_n(3,address,MIXEDBACKWARD,speed);
}

// RoboClaw Mixed Forward Backward
bool RoboClaw::ForwardBackwardMixed(uint8_t address, uint8_t speed) {
	return write_n(3,address,MIXEDFB,speed);
}

// RoboClaw Set M1 PID
bool RoboClaw::SetM1VelocityPID(uint8_t address, float kp_fp, float ki_fp, float kd_fp, uint32_t qpps) {
	uint32_t kp = kp_fp*65536;
	uint32_t ki = ki_fp*65536;
	uint32_t kd = kd_fp*65536;
	return write_n(18,address,SETM1PID,SetDWORDval(kd),SetDWORDval(kp),SetDWORDval(ki),SetDWORDval(qpps));
}

// RoboClaw Set M2 PID
bool RoboClaw::SetM2VelocityPID(uint8_t address, float kp_fp, float ki_fp, float kd_fp, uint32_t qpps) {
	uint32_t kp = kp_fp*65536;
	uint32_t ki = ki_fp*65536;
	uint32_t kd = kd_fp*65536;
	return write_n(18,address,SETM2PID,SetDWORDval(kd),SetDWORDval(kp),SetDWORDval(ki),SetDWORDval(qpps));
}

// RoboClaw M1 Duty
bool RoboClaw::DutyM1(uint8_t address, uint16_t duty) {
	return write_n(4,address,M1DUTY,SetWORDval(duty));
}

// RoboClaw M2 Duty
bool RoboClaw::DutyM2(uint8_t address, uint16_t duty) {
	return write_n(4,address,M2DUTY,SetWORDval(duty));
}

// RoboClaw M1 & M2 Duty
bool RoboClaw::DutyM1M2(uint8_t address, uint16_t duty1, uint16_t duty2) {
	return write_n(6,address,MIXEDDUTY,SetWORDval(duty1),SetWORDval(duty2));
}

// Main method for RoboClaw testing
// Sets up initial config and begins outputting to the terminal
// Rename to main if compiling only this file
int main() {
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
            res = rc->ForwardBackwardMixed(add1, val);
        } else if (add == 2) {
            res = rc->ForwardBackwardMixed(add2, val);
        }
	printf("%d\n", res);
    } while (true);

	return 0;
}
