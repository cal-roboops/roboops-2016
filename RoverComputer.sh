#!/bin/bash
# Don't forget to run chmod +x RoverComputer.sh

# Setup the cameras with Motion
sudo motion1
sudo motion2
sudo motion3

# Compile ROverComputer.cpp
g++ -Wall -Wno-long-long wiringPi/wiringPi/softServo.c Helpers/sharedDefinitions.h GPIO_RaspPi/RoboClaw.cpp GPIO_RaspPi/Encoder.cpp Networking/Server.cpp RoverComputer.cpp -o RoverComputer.exe -lwiringPi

# Run RoverComputer.exe
sudo ./RoverComputer.exe