# ArPiRobot-CoreLib

Core library for ArPiRobot robots containing user-facing API, device "driver" code, etc.


## [cpp_library](cpp_library/)

C++ Library. This is where everything is implemented. This library is loaded by bindings to other languages, or used to program the robot in C++.


## [python_bindings](python_bindings/)

Python bindings for the ArPiRobot core library (C++). The python library matches the object oriented API of the C++ library.


## Opening in VSCode

Open either `cpp_library` or `python_bindings` in vscode. It is recommended to install the C/C++, CMake Tools, and Python extensions.

## Dependencies

*Note: If cross compiling for armv6 or aarch64 the sysroot provided from the Image scripts repo will include the dependencies of the corelib.*

- gstreamer
- opencv
- boost (system, asio, process)
- libserialport
- pigpio (optional)
- lgpio (optional)

For python, you need the python opencv module.

## Required Tools for Build

- LLVM Clang (Apple Clang does not work, you need llvm clang with lld linker, use brew to install on mac)
- CMake
- Ninja
- pkg-config (should be included, except on windows - install using scoop)

- You will need a sysroot for the system you are compiling for (armv6 or aarch64 sysroot from imagescripts repo) installed in `~/.arpirobot/sysroot/ARCH`.

## Building

```sh
# Setup test robot project (assuming you want to run anything after building)
# Note: If the test robot project already exists this will delete it and replace it with the base version
python3 setup-testrobot.py

# Enter cpp lib directory
cd cpp_library

# Configure for one of the supported architectures
# armv6, aarch64, or native
# native builds for a native Linux system. You can only use this if you are building on Linux
# It is used if running the robot program for testing on your development PC (again, Linux only)
cmake --preset [PRESET]

# Build
# CONFIG is Debug, Release, MinSizeRel, or RelWithDebInfo
# Must use a preset you previously configured for
cmake --build [PRESET]-[CONFIG]
```

## Running on Robot

1. Build the CPP library for the correct architecture

2. Run `python3 dev-deploy.py PRESET CONFIG` to deploy the built library and test robot programs via ssh

3. Login to the robot via ssh

4. Stop the arpirobot-program service on the robot `sudo systemctl stop arpirobot-program.service`

5. On the robot, in the `/home/arpirobot/CoreLib-Test` folder run either `start-cpp.sh` or `start-py.sh` to run the cpp or python test robot programs

## Running on Dev PC

The corelib can be built for a native linux PC. You'll need to install the dependencies for the corelib on your development pc. When running on a native PC, only the libserialport IO provider will be available meaning only UART devices will work. Most devices (GPIO, I2C, PWM, SPI, etc) will not work.

This can still be useful to test Arduino Firmware or during development of Arduino Firmware. Additionally, can be useful when testing features that don't require use of any IO interfaces.

To run, after building use `python3 run.py [cpp/python] [CONFIG]`