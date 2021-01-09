# ArPiRobot-CoreLib

C++ library for ArPiRobot robots. See other folders in this repo for other language bindings.

This method means only one library to maintain, same features regardless of language, and issues like Python's GIL are avoided (mostly).

## Libraries
- ASIO:
    - In deps folder. Header only.
- CTPL
    - Header only, in deps folder
- PiGPIO
    - Built using CMake build system from pigpio project (in deps folder)
    - Statically linked into libarpirobot-core


## Building (Core C++ Library)

#### Requirements
- [CMake](https://cmake.org/)
- [conan](https://conan.io/)
- Make (if on windows will need to download this somewhere)
- Raspberry Pi Cross Compiler (GCC 8.3). Make sure to download one supporting the Pi Zero.
    - [Pre-built Toolchains for Linux Host](https://github.com/abhiTronix/raspberry-pi-cross-compilers)
        - Extract to `~/.arpirobot/toolchain`
    - [Pre-built toolchains for Windows Host](https://gnutoolchains.com/raspberry/)
        - Install to `C:\Users\USERNAME\.arpirobot\toolchain` changing USERNAME to your username


#### Building
Run the following commands in the cpp_lib directory
```
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../arpirobot-cross.cmake -G "Unix Makefiles" ..
cmake --build .
```