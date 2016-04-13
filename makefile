RoverComputer:
	g++ -Wall -Wno-long-long -I/usr/include/python2.7 wiringPi/wiringPi/softServo.c Helpers/sharedDefinitions.h GPIO_RaspPi/Encoder.cpp Networking/Server.cpp RoverComputer.cpp -o RoverComputer.exe -lwiringPi -lpython2.7
	
CommandComputer:
	g++ -Wall -Wno-long-long Networking/Client.cpp CommandComputer.cpp -o CommandComputer.exe -lws2_32 -lwsock32

Client:
	g++ -Wall -Wno-long-long -o Client.exe Client.cpp -lws2_32 -lwsock32

Server:
	g++ -Wall -Wno-long-long -o Server.exe Server.cpp

PiSSH:
	ssh pi@2601:644:102:7600::fa89 (wifi)
	ssh pi@192.168.137.xxx (ethernet - last number changes)

UART:
        RPi 2: UART=>'ttyAMA0'
        RPi 3: UART=>'ttyS0'
