RoverComputer:
	g++ -Wall -Wno-long-long wiringPi/wiringPi/softServo.c GPIO_RaspPi/RoboClaw.cpp GPIO_RaspPi/Encoder.cpp Networking/Server.cpp RoverComputer.cpp -o RoverComputer.exe -lwiringPi
	
CommandComputer:
	g++ -Wall -Wno-long-long Networking/Client.cpp CommandComputer.cpp -o CommandComputer.exe -lws2_32 -lwsock32

FFSERVER:
	ffserver &

FFMPEG:
	ffmpeg -f video4linux2 -i /dev/video0 http://localhost:8090/webcam1.ffm

Client:
	g++ -Wall -Wno-long-long -o Client.exe Client.cpp -lws2_32 -lwsock32

Server:
	g++ -Wall -Wno-long-long -o Server.exe Server.cpp

PiSSH:
	ssh pi@2601:644:102:7600::xxxx (wifi)
	ssh pi@192.168.137.xxx (ethernet - last number changes find with "ping raspberrypi.local")
	ssh pi@ (ethernet - last number changes find with "ping raspberrypi.local")

UART:
        RPi 2: UART=>'ttyAMA0'
        RPi 3: UART=>'ttyS0'
