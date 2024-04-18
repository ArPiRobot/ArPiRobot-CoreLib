# ArPiRobot-CoreLib

C++ library for ArPiRobot robots. See other folders in this repo for other language bindings.

This method means only one library to maintain, same features regardless of language, and issues like Python's GIL are avoided (mostly).

## Dependencies

*The intent is that these (unless included in source tree) are included on system images and pre-built sysroots.*

- libserialport
- libopencv4
- libgstreamer-1.0
- boost (system, process, and asio)
- libpigpio (OPTIONAL)
- liblgpio (OPTIONAL)
- Included in source tree
    - miniaudio
    - ctpl

Python bindings dependencies:

- opencv
- numpy

*Note: If none of the optional I/O libraries are used, only the fallback libserialport UART-only I/O provider will be available. This is mainly intended for testing on development systems.*

## Building (Core C++ Library)

#### Requirements

- CMake
- Ninja
- LLVM Clang (not apple clang, you need lld!)
- A sysroot (typically installed by deploy tool at `~/.arpirobot/sysroot/[architecture]`). Can be downloaded pre-built from imagescripts repo.


#### Building

```sh
cmake --preset [preset]
cmake --build --preset [preset]-[config]
```

- `[preset]` is `armv6`, `aarch64`, or `native`.
- `[config]` is `debug`, `release`, `relwithdebinfo`, or `minsizerel`
