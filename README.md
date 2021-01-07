# ArPiRobot-CoreLib

Experimental C++ library for ArPiRobot robots. Idea is to use language specific bindings for Python, Java, etc and use the same core library to handle the robot scheduler, interfacing with devices, etc.

Ideally, this means only one library to maintain, same features regardless of language, and issues like Python's GIL are avoided (mostly).

## Building

- Install CMake and conan
- Download a [cross compiler](https://github.com/abhiTronix/raspberry-pi-cross-compilers) form the link. Make sure downloaded compiler supports pi zero. Extract to your home directory.
- Run prep_pi_zero
- cd to build directory
- Run `cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake_toolchains/pi_zero.cmake`
