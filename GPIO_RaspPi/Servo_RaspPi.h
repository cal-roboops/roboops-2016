//
// Servo_RaspPi.h
// CPP Project
//
// Created by Mitchell Oleson on 2/26/2016
//
// Written for Debian Linux
//

#include <softPwm.h>
#include <stdio.h>
#include <unistd.h>

// Servo_RaspPi Class
class Servo_RaspPi {
	private:
		// Private Variables
		int servo_pin;
		int curr_spin;
		int servo_range;

		// Private Methods
		spin();

	public:
		// Public Variables
		
		// Public Methods
		Servo_RaspPi::Servo_RaspPi(int pin, int range);
		Servo_RaspPi::~Servo_RaspPi();
		void set(int x);
		void update(int x);
};