//
// RoboClaw.h
// CPP Project
//
// Created by Mitchell Oleson on 4/2/2016
//
// Written for Debian Linux
//

#define MAXRETRY 2
#define SetDWORDval(arg) (uint8_t)(arg>>24),(uint8_t)(arg>>16),(uint8_t)(arg>>8),(uint8_t)arg
#define SetWORDval(arg) (uint8_t)(arg>>8),(uint8_t)arg

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <termios.h>

// RoboClaw Class
class RoboClaw {
	private:
		// Private Variables
		int uart;
		uint16_t crc;
		uint32_t timeout;
		const char* deviceName;

		// Private Methods
		// transmit (command=single number defined to function in manual)
		void crc_clear();
		uint16_t crc_get();
		void crc_update(uint8_t data);
		bool write_n(uint8_t cnt, ... );

	public:
		// Public Variables
		// Public Methods
		RoboClaw(const char* dev);
		~RoboClaw();
		bool ForwardM1(uint8_t address, uint8_t speed);
		bool BackwardM1(uint8_t address, uint8_t speed);
		bool SetMinVoltageMainBattery(uint8_t address, uint8_t voltage);
		bool SetMaxVoltageMainBattery(uint8_t address, uint8_t voltage);
		bool ForwardM2(uint8_t address, uint8_t speed);
		bool BackwardM2(uint8_t address, uint8_t speed);
		bool ForwardBackwardM1(uint8_t address, uint8_t speed);
		bool ForwardBackwardM2(uint8_t address, uint8_t speed);
		bool CombinedForward(uint8_t address, uint8_t speed);
		bool CombinedBackward(uint8_t address, uint8_t speed);
		bool CombinedForwardBackward(uint8_t address, uint8_t speed);


		enum {
			M1FORWARD = 0,
			M1BACKWARD = 1,
			SETMINMB = 2,
			SETMAXMB = 3,
			M2FORWARD = 4,
			M2BACKWARD = 5,
			M17BIT = 6,
			M27BIT = 7
		};
};
