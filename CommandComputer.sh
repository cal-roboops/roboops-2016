#!/bin/bash
# Don't forget to run chmod +x CommandComputer.sh

# Compile CommandComputer.cpp (will be commented out in final since visual studio compiles it)
#g++ -Wall -Wno-long-long Networking/Client.cpp CommandComputer.cpp -o CommandComputer.exe -lws2_32 -lwsock32

# Start the video screen application
./Video/MJPEG/MjpegProcessorTestWinForms/bin/Release/MjpegProcessorTestWinForms.exe &

# Start the CommandComputer.exe
./CommandComputer/x64/Debug/CommandComputer.exe &
