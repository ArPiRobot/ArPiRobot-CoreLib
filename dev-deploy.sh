#!/bin/bash

# Make sure zero or one arguments
if [ $# -gt 1 ]; then
    printf "Usage:\n"
    printf "    $0 [ROBOT_ADDRESS]\n\n"
    printf "Arguments:\n"
    printf "    ROBOT_ADDRESS\tThe IP address or hostname of the robot.\n"
    exit 1
fi

# Get robot IP from first argument
if [ -z "$1" ]; then
    # No IP supplied. Use default.
    ROBOT_IP="192.168.10.1"
else
    ROBOT_IP="$1"
fi


# Delete folder & create empty one
printf "**Removing old files form the pi.**\n"
ssh -l pi $ROBOT_IP "sudo mount -o rw,remount /;mkdir -p /home/pi/CPPLibTest/;sudo rm -rf /home/pi/CPPLibTest/*"


# Copy from this PC to Pi
printf "\n**Copying files to the pi.**\n"
scp -r cpp_library/build/libarpirobot-core.so cpp_library/build/testrobot python_bindings/arpirobot/ python_bindings/testrobot-py java_bindings/lib/build/libs/*.jar java_bindings/testrobot/build/libs/robot.jar start-cpp.sh start-py.sh start-java.sh  pi@$ROBOT_IP:/home/pi/CPPLibTest

printf "\n**THE PI IS NOW READ/WRITE!**\n"
