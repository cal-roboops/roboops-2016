RoverComputer:
	g++ -Wall -Wno-long-long wiringPi/wiringPi/softServo.c Networking/Server.cpp RoverComputer.cpp -o RoverComputer.exe -lwiringPi
	
CommandComputer:
	g++ -Wall -Wno-long-long Networking/Client.cpp CommandComputer.cpp -o CommandComputer.exe -lws2_32 -lwsock32

Client:
	g++ -Wall -Wno-long-long -o Client.exe Client.cpp -lws2_32 -lwsock32

Server:
	g++ -Wall -Wno-long-long -o Server.exe Server.cpp

PiSSH:
	ssh pi@2601:644:102:7600::fa89