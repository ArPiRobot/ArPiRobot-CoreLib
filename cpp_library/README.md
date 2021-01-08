# ArPiRobot-CoreLib

Experimental C++ library for ArPiRobot robots. Idea is to use language specific bindings for Python, Java, etc and use the same core library to handle the robot scheduler, interfacing with devices, etc.

Ideally, this means only one library to maintain, same features regardless of language, and issues like Python's GIL are avoided (mostly).

## Libraries
- Boost:
    - Installed via conan
    - Statically linked into libarpirobot-core.so
- CTPL
    - Header only, in deps folder
- PiGPIO
    - Installed using CMake build system from pigpio project (in deps folder)
    - Link to shared library b/c want to use whatever version is installed on the pi (as using pigpiod)
        - Using shared library b/c need same version of library and pigpiod (use what is installed on pi itself)
        - This library is installed on the pi from RPiOS repos when building the custom image
    - Version in deps folder should match version on newest Pi image
    - On pi run pigpiod -v to get version

## Building (Linux Only)

- Install CMake and conan
- Download a [cross compiler](https://github.com/abhiTronix/raspberry-pi-cross-compilers) from the link below. Make sure downloaded compiler supports pi zero. Extract to your home directory.
- Run prep_pi_zero
- cd to build directory
- Run `cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake_toolchains/pi_zero.cmake`
