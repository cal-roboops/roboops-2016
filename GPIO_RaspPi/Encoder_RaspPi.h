//
// Encoder_RaspPi.h
// CPP Project
//
// Created by Mitchell Oleson on 2/26/2016
// 
// Copyright © 2016 Mitchell Oleson. All rights reserved.
//
// Written for Debian Linux
//

// Encoder_RaspPi class
class Encoder_RaspPi {
	private:
		// Private Variables
		int curr_value;

		// Private Methods

	public:
		// Public Variables
		
		// Public Methods
		Encoder_RaspPi::Encoder_RaspPi();
		Encoder_RaspPi::~Encoder_RaspPi();
		int value();
};