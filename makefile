RoverComputer:
	g++ -Wall -Wno-long-long wiringPi/wiringPi/softServo.c Networking/Server.cpp RoverComputer.cpp -o RoverComputer.exe -lwiringPi
        g++ -Wall -Wno-long-long wiringPi/wiringPi/softServo.c Helpers/roverHelper.cpp GPIO_RaspPi/RoboClaw.cpp GPIO_RaspPi/Encoder.cpp Networking/Server.cpp Helpers/sharedDefinitions.h RoverComputer.cpp -o RoverComputer.exe -lwiringPi
	
CommandComputer:
	g++ -Wall -Wno-long-long Networking/Client.cpp CommandComputer.cpp -o CommandComputer.exe -lws2_32 -lwsock32

Client:
	g++ -Wall -Wno-long-long -o Client.exe Client.cpp -lws2_32 -lwsock32

Server:
	g++ -Wall -Wno-long-long -o Server.exe Server.cpp

PiSSH:
	ssh pi@2601:644:102:7600::fa89 (wifi)
	ssh pi@192.168.137.204 (ethernet)
