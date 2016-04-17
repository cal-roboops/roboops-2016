Cal Robo-Ops 2016 Software Repository

Our two main modules are seperated into the CommandComputer.cpp (which will eventually support a Windows Visual Studio GUI) and RoverComputer.cpp which runs on a Raspberry Pi with Raspbian Lite OS.

The Command Computer is located at home base and administers commands to the rover via a TCP connection and the inputs of either the GUI or a Saitek X52 Joystick. Additionally the GUI hosts a live stream from the rovers onboard cameras and switches between two of the three cameras depending on the current drive mode. The command computer does all the command translations so that the Rover Computer only needs to send the data to the appropriate system (and preform type converstions). This creates an orderly fashion for sending commands and ensures that we don't overburden the Raspberry Pi by doing a lot of rover side preprocessing.

The Rover Computer is responsible for receiving all the commands and forwarding them to the correct controllers. This is done via the wiringPi software library (http://wiringpi.com/) and its accompanying packages as well as rpi-serial-console (https://github.com/lurch/rpi-serial-console) to enable the UART or its now possible to enable the UART using 'raspi-config'. For everything else we have written custom software to support the hardware controllers and feedback systems. To control the drive motors we use two Roboclaws and issue commands via a shared packet serial interface. In the arm we use custom circuits to control the motors and actuators while reading back and displaying the positioning data so we won't run into anything.

Setup Notes:
	1) Windows computers will need to have ION Motion installed if you want to test/debug the RoboClaws before use. Make sure that the most recent firnware is installed on the roboclaw by using the ION Motion software to update it.
	2) Need to have DirectX SDK Installed to compile software
		- If you get error S1023 while installing DirectX you need to uninstall the Microsoft Visual C++ 2010 Redistributables and then install DirectX. After it's done you can reinstall the newest version of the Visual C++ 2010 Redistributable.
	3) Use rpi-serial-console to setup the UART on the Rover's Raspberry Pi then use the wiringPi libraries to control the GPIO's via C++. Note: On Raspberry Pi 2 the UART is mapped to '/dev/ttyAMA0' and on the Pi 3 it is at '/dev/ttyS0' (the bluetooth is now at the old mapping).
		- To use the serial device follow guide at 'http://elinux.org/RPi_Serial_Connection'
	4) Make sure that the roboclaw is in the right mode. It has the tendancy to switch modes if it goes for a hard resert which isn't what we want and will result in erratic behaviour.
	5) Command Computer will need to download the Saitek X52 Drivers from 'www.saitek.com/uk/drivers.php' in order to pickup and run the joystick. Additionally, the Saitek software suite has examples of how to pickup button presses and axis position from the controller. (Also the DirectX SDK has a source library with a generic jostick program and equivalent Visual Studio Code)
	6) We are using motion to setup and control the cameras on the Rasparry Pi. This allows us to easily setup new cameras and broadcast them to a web browser so that we can easily pick them up on the command computer side of things. We used 'http://www.lavrsen.dk/foswiki/bin/view/Motion/FrequentlyAskedQuestions#How_do_I_get_Motion_to_work_with_two_or_more_camera_at_a_time_63' for setup and information about the cameras. Use the config file to enable remote streaming and adjust the framrates/resolution. ('https://www.linux.com/learn/how-operate-linux-spycams-motion' is also helpful)