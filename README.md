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

**IMPORTANT NOTE** The Pi 3 has also changed how the UART Serial Pins work. This made it so that our roboclaws would not function as the pins don't have a stable signal anymore (see 'https://blog.adafruit.com/2016/03/07/raspberry-pi-3-uart-speed-workaround/'). Essentially, this stems from the fact that the hardware UART of the chip now talks to the bluetooth (the default '/dev/ttyAMA0') instead of the UART (which was moved to '/dev/ttyS0'). The consequence of this is that the UART pins on the GPIO header now use "mini-UART" which is tied to the CPU speed. This means GPIO UART is not guarenteed a stable baudrate since the CPU speed changes. This lead to a host of problems when we switched from our test bed of Pi 2 (which uses the hardware UART for the GPIO header) to the Pi 3, most notably a complete inability to control the roboclaws and a much greater stutter in the servos. Check out 'http://www.briandorey.com/post/Raspberry-Pi-3-UART-Boot-Overlay-Part-Two' (or the now old post http://www.briandorey.com/post/Raspberry-Pi-3-UART-Overlay-Workaround') for a potential fix (we just decided to use the Pi 2 instead as we were on a tight schedule).

4) Make sure that the roboclaw is in the right mode. It has the tendancy to switch modes if it goes for a hard resert which isn't what we want and will result in erratic behaviour.

5) Command Computer will need to download the Saitek X52 Drivers from 'www.saitek.com/uk/drivers.php' in order to pickup and run the joystick. Additionally, the Saitek software suite has examples of how to pickup button presses and axis position from the controller. (Also the DirectX SDK has a source library with a generic jostick program and equivalent Visual Studio Code)

6) We are using motion to setup and control the cameras on the Rasparry Pi. This allows us to easily setup new cameras and broadcast them to a web browser so that we can easily pick them up on the command computer side of things. We used 'http://www.lavrsen.dk/foswiki/bin/view/Motion/FrequentlyAskedQuestions#How_do_I_get_Motion_to_work_with_two_or_more_camera_at_a_time_63' for setup and information about the cameras. Use the config file to enable remote streaming and adjust the framrates/resolution. ('https://www.linux.com/learn/how-operate-linux-spycams-motion' is also helpful)

**NOTE** This has been switched to just FFSERVER and FFMPEG since these were already incorporated in motion which provided a bad wrapper and inconvienient framework.

7) Make sure that if the controller software is rewritten the functions 'EnumJoysticksCallback' and 'EnumObjectsCallback' remain global (not under the controllers class). If they are put inside the class then an inccorect method to pointer error will be thrown. We used the directX joystick example class and 'https://www.cs.cmu.edu/~jparise/directx/joystick/' as guide for the general joystick class. In addition to this we added the ability to store variables and instantiate the class so that we could bring it in as a part of our larger project.

8) Before plugging in the Saitek Joystick for the first time make sure that you have the right drivers downloaded and DO NOT plug the controller in before the driver installer ASKS you to. If you do plug in before then it windows will automatically search for the drivers and mess up the intial config. Check out these threads 'https://forums.robertsspaceindustries.com/discussion/308307/x52-and-win-10' and 'http://forums.eagle.ru/showthread.php?t=84295' if you run into this problem.

Additions for future competitions:

1) Sanatize control inputs on the rover side to ensure that received value is within acceptable range. This could possible be done via rewriting the comand parsing structure so that it reads and immediately excutes the command. Could lead to simplification of code, faster execution speeds and also allows a variable amount of arguments (i.e. just roboclaw commands or just servo inputs).

2) Add more drive modes and increase capability of the rover arm. Currently the arm is in a "monkey see, monkey do" kind of command structure. We use the visual feedback of the cameras to a sense of how to drive the pieces that had no encoders. This can be expanded to have more fluid/natrual movement or better mapping to the control stick.

3) Be able to fold and unfold the rover at will. Right now there is no way to return the rover back to folded position. Would require that more encoders be added to arm joints and making sure that raising and lowering camera mast is okay. (Might be done later but need to ensure encoders and pins).

4) Add more redundancy and checks that it's harder to error out and crash the program. Also create a failsafe that stops all action on the rover side in the event of a disconnection (or even power loss  of the rover computer but not the roboclaws).
	