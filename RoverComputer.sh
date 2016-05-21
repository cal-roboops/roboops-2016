#!/bin/bash
# Don't forget to run chmod +x RoverComputer.sh

# Setup and login to LogMeIn (now done on startup)
#sudo /etc/init.d/logmein-hamachi start
#sudo hamachi login

# Restart hamachi service if lost connection
#sudo /etc/init.d/logmein-hamachi restart

# Setup the cameras with Motion
sudo motion

# Compile RoverComputer.cpp
#g++ -Wall -Wno-long-long wiringPi/wiringPi/softServo.c Helpers/sharedDefinitions.h GPIO_RaspPi/RoboClaw.cpp GPIO_RaspPi/Encoder.cpp Networking/Server.cpp RoverComputer.cpp -o RoverComputer.exe -lwiringPi

# Start the rover computer application RoverComputer.exe
sudo ./RoverComputer.exe