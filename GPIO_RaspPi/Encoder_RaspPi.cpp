//
// Encoder_RaspPi.cpp
// CPP Project
//
// Created by Mitchell Oleson on 2/26/2016
// 
// Copyright © 2016 Mitchell Oleson. All rights reserved.
//
// Written for Debian Linux
//

#include "Encoder_RaspPi.h"

Encoder_RaspPi::Encoder_RaspPi() {
	;
}

Encoder_RaspPi::~Encoder_RaspPi() {
	;
}

int Encoder_RaspPi::value() {
	curr_value = read();
	return curr_value;
}