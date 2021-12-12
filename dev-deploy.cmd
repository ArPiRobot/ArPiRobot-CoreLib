@Echo off

set ROBOT_IP="192.168.10.1"

@Rem Delete folder & create empty one
echo **Removing old files form the pi.**
ssh -l pi %ROBOT_IP% "sudo mount -o rw,remount /;mkdir -p /home/pi/CoreLib-Test/;sudo rm -rf /home/pi/CoreLib-Test/*"


@Rem Copy from this PC to Pi
echo.
echo **Copying files to the pi.**
scp -r cpp_library/build/*.so cpp_library/build/testrobot python_bindings/arpirobot/ python_bindings/testrobot-py start-cpp.sh start-py.sh pi@%ROBOT_IP%:/home/pi/CoreLib-Test

echo.
echo **THE PI IS NOW READ/WRITE!**
