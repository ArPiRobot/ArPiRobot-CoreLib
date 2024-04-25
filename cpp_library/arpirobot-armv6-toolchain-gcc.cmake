if(CMAKE_HOST_WIN32)
    set(HOMEDIR "$ENV{UserProfile}")
    string(REPLACE "\\" "/" HOMEDIR "${HOMEDIR}")
    set(EXTENSION ".exe")
else()
    set(HOMEDIR "$ENV{HOME}")
    set(EXTENSION "")
endif()

set(CMAKE_CROSSCOMPILING TRUE)
SET(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

SET(TARGET arm-linux-gnueabihf)
set(CMAKE_SYSROOT "${HOMEDIR}/.arpirobot/sysroot/armv6")

set(GCC "${HOMEDIR}/.arpirobot/toolchain/armv6/bin/arm-linux-gnueabihf-gcc")
set(GPP "${HOMEDIR}/.arpirobot/toolchain/armv6/bin/arm-linux-gnueabihf-g++")

# Make pkg-config work properly with sysroot
set(ENV{PKG_CONFIG_DIR} "")
set(ENV{PKG_CONFIG_LIBDIR} "${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf/pkgconfig")
set(ENV{PKG_CONFIG_SYSROOT_DIR} ${CMAKE_SYSROOT})

if(NOT EXISTS "${CMAKE_SYSROOT}")
    message(FATAL_ERROR "Sysroot directory is missing.")
endif()

SET(CMAKE_C_COMPILER ${GCC})
SET(CMAKE_C_COMPILER_TARGET arm-linux-gnueabihf)

SET(CMAKE_CXX_COMPILER ${GPP})
SET(CMAKE_CXX_COMPILER_TARGET arm-linux-gnueabihf)

SET(CMAKE_ASM_COMPILER ${GCC})
SET(CMAKE_ASM_COMPILER_TARGET arm-linux-gnueabihf)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

SET(SHARED_FLAGS "-Wno-psabi -Wl,-rpath-link,${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf")
SET(CMAKE_C_FLAGS "${SHARED_FLAGS}" CACHE STRING "C compiler flags")
SET(CMAKE_CXX_FLAGS "${SHARED_FLAGS}" CACHE STRING "C++ compiler flags")
set(LINK_FLAGS "${SHARED_FLAGS}" CACHE STRING "Linker flags")
