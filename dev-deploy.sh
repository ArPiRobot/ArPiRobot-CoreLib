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
ssh -l pi $ROBOT_IP "sudo mount -o rw,remount /;mkdir -p /home/pi/CoreLib-Test/;sudo rm -rf /home/pi/CoreLib-Test/*"


# Copy from this PC to Pi
printf "\n**Copying files to the pi.**\n"
scp -r cpp_library/build/*.so cpp_library/build/testrobot python_bindings/arpirobot/ python_bindings/testrobot-py start-cpp.sh start-py.sh pi@$ROBOT_IP:/home/pi/CoreLib-Test

printf "\n**THE PI IS NOW READ/WRITE!**\n"
