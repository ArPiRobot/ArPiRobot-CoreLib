# ArPiRobot-CoreLib

Experimental C++ library for ArPiRobot robots. Idea is to use language specific bindings for Python, Java, etc and use the same core library to handle the robot scheduler, interfacing with devices, etc.

Ideally, this means only one library to maintain, same features regardless of language, and issues like Python's GIL are avoided (mostly).

## Libraries
- Boost.ASIO:
    - In deps folder. Header only.
    - Depends on Boost.System (also header only in deps folder)
    - Only includes specific (required) files form boost source
- CTPL
    - Header only, in deps folder
- PiGPIO
    - Built using CMake build system from pigpio project (in deps folder)
    - Statically linked into libarpirobot-core


## Building (Core C++ Library)

#### Requirements
- [CMake](https://cmake.org/)
- [conan](https://conan.io/)
- Raspberry Pi Cross Compiler (GCC 8.3). Make sure to download one supporting the Pi Zero.
    - [Pre-built Toolchains for Linux Host](https://github.com/abhiTronix/raspberry-pi-cross-compilers)
        - Extract to `~/.arpirobot/toolchain`
    - [Pre-built toolchains for Windows Host](https://gnutoolchains.com/raspberry/)
        - Install to `C:\Users\USERNAME\.arpirobot\toolchain` changing USERNAME to your username


#### Building
Run the following commands in the cpp_lib directory
```
./pre_build.sh
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake_toolchains/pi_cross_compile.cmake ..
cmake --build .
```


## Python Bindings

TODO