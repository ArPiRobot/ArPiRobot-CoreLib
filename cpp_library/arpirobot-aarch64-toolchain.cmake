if(CMAKE_HOST_WIN32)
    set(HOMEDIR "$ENV{UserProfile}")
    set(EXTENSION ".exe")
else()
    set(HOMEDIR "$ENV{HOME}")
    set(EXTENSION "")
endif()

set(CMAKE_CROSSCOMPILING TRUE)
SET(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

SET(TARGET aarch64-linux-gnu)
set(CMAKE_SYSROOT "${HOMEDIR}/.arpirobot/sysroot/aarch64")

SET(CMAKE_C_COMPILER clang)
SET(CMAKE_C_COMPILER_TARGET aarch64-linux-gnu)

SET(CMAKE_CXX_COMPILER clang++)
SET(CMAKE_CXX_COMPILER_TARGET aarch64-linux-gnu)

SET(CMAKE_ASM_COMPILER clang)
SET(CMAKE_ASM_COMPILER_TARGET aarch64-linux-gnu)

# Note: --sysroot automatically passed if CMAKE_SYSROOT is set
# Note: -target is automatically passed by cmake as set above
# Note: Linking with lld since it is cross linker natively
#       Avoids needing gcc for target system just to link
#       Thus, using clang and lld, no cross GNU toolchain is needed. Only sysroot.
SET(SHARED_FLAGS "-fuse-ld=lld -Qunused-arguments")
SET(CMAKE_C_FLAGS "${SHARED_FLAGS}" CACHE STRING "C compiler flags")
SET(CMAKE_CXX_FLAGS "${SHARED_FLAGS}" CACHE STRING "C++ compiler flags")
set(LINK_FLAGS "${SHARED_FLAGS} -fuse-ld=lld" CACHE STRING "Linker flags")
