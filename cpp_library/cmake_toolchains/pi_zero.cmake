set(target_host arm-linux-gnueabihf)
set(standalone_toolchain $ENV{HOME}/cross-pi-gcc-10.2.0-0)
set(cc_compiler gcc)
set(cxx_compiler g++)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER ${standalone_toolchain}/bin/${target_host}-${cc_compiler} CACHE PATH "C Compiler")
set(CMAKE_CXX_COMPILER ${standalone_toolchain}/bin/${target_host}-${cxx_compiler} CACHE PATH "C++ Compiler")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)