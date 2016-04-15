#!/bin/bash
# Don't forget to run chmod +x RoverComputer.sh

# Setup Motion Cameras

# Compile ROverComputer.cpp
g++ -Wall -Wno-long-long wiringPi/wiringPi/softServo.c Helpers/sharedDefinitions.h GPIO_RaspPi/RoboClaw.cpp GPIO_RaspPi/Encoder.cpp Networking/Server.cpp RoverComputer.cpp -o RoverComputer.exe -lwiringPi

# Run RoverComputer.exe
sudo ./RoverComputer.exe