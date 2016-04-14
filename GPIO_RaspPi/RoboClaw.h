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

#include <poll.h>
#include <math.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include "../wiringPi/wiringPi/wiringPi.h"
#include "../wiringPi/wiringPi/wiringSerial.h"

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
		bool read_n(uint8_t cmd,uint8_t address,uint8_t cnt,...);
		
	public:
		// Public Variables
		// Public Methods
		RoboClaw(const char* dev, uint32_t tout);
		~RoboClaw();
		bool ForwardM1(uint8_t address, uint8_t speed);
		bool BackwardM1(uint8_t address, uint8_t speed);
		bool ForwardM2(uint8_t address, uint8_t speed);
		bool BackwardM2(uint8_t address, uint8_t speed);
		bool ForwardMixed(uint8_t address, uint8_t speed);
		bool BackwardMixed(uint8_t address, uint8_t speed);
		bool ForwardBackwardMixed(uint8_t address, uint8_t speed);
		bool SetM1VelocityPID(uint8_t address, float kp_fp, float ki_fp, float kd_fp, uint32_t qpps);
		bool SetM2VelocityPID(uint8_t address, float kp_fp, float ki_fp, float kd_fp, uint32_t qpps);
		bool DutyM1(uint8_t address, uint16_t duty);
		bool DutyM2(uint8_t address, uint16_t duty);
		bool DutyM1M2(uint8_t address, uint16_t duty1, uint16_t duty2);

		enum {
			M1FORWARD = 0,
			M1BACKWARD = 1,
			M2FORWARD = 4,
			M2BACKWARD = 5,
			MIXEDFORWARD = 8,
			MIXEDBACKWARD = 9,
			MIXEDFB = 12,
			SETM1PID = 28,
			SETM2PID = 29,
			M1DUTY = 32,
			M2DUTY = 33,
			MIXEDDUTY = 34
		}
};
