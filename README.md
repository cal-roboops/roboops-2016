Cal Robo-Ops 2016 Software Team

Our two main modules are seperated into the CommandComputer.cpp (which will eventually support a Windows Visual Studio GUI) and RoverComputer.cpp which runs on a Raspberry Pi with Raspbian Lite OS.

The Command Computer is located at home base and administers commands to the rover via a TCP connection and the inputs of either the GUI or a Saitek X52 Joystick. Additionally the GUI hosts a live stream from the rovers onboard cameras and switches between two of the three cameras depending on the current drive mode. The command computer does all the command translations so that the Rover Computer only needs to send the data to the appropriate system (and preform type converstions). This creates an orderly fashion for sending commands and ensures that we don't overburden the Raspberry Pi by doing a lot of rover side preprocessing.

The Rover Computer is responsible for receiving all the commands and forwarding them to the correct controllers. This is done via the wiringPi software library (http://wiringpi.com/) and its accompanying packages as well as rpi-serial-console (https://github.com/lurch/rpi-serial-console) to enable the UART. For everything else we have written custom software to support the hardware controllers and feedback systems. To control the drive motors we use two Roboclaws and issue commands via a shared packet serial interface. In the arm we use custom circuits to control the motors and actuators while reading back and displaying the positioning data so we won't run into anything.

Setup Notes:
	1) Windows computers will need to have ION Motion installed if you want to test/debug the RoboClaws before use.
	2) Need to have DirectX SDK Installed to compile software
		- If you get error S1023 while installing DirectX you need to uninstall the Microsoft Visual C++ 2010 Redistributables and then install DirectX. After it's done you can reinstall the newest version of the Visual C++ 2010 Redistributable.
	3) Use rpi-serial-console to setup the UART on the Rover's Raspberry Pi then use the wiringPi libraries to control the GPIO's via C++. Note: On Raspberry Pi 2 the UART is mapped to '/dev/ttyAMA0' and on the Pi 3 it is at '/dev/ttyS0' (the bluetooth is now at the old mapping). Alternatively, you can just use the Raspberry Pi's built in config editor by running 'sudo rasppi-config'.