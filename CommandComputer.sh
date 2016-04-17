#!/bin/bash
# Don't forget to run chmod +x CommandComputer.sh

# Compile CommandComputer.cpp
g++ -Wall -Wno-long-long Networking/Client.cpp CommandComputer.cpp -o CommandComputer.exe -lws2_32 -lwsock32

# Run CommandComputer.exe
./CommandComputer.exe 