cmake_minimum_required (VERSION 3.20)
project (ArPiRobot-CoreLib CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")  


# Io libraries
option(USE_PIGPIO "Use libpigpio" ON)
option(USE_LGPIO "Use liblgpio" ON)
option(USE_SERIALPORT "Use libserialport" ON)


# Find dependencies
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED COMPONENTS headers system)
pkg_check_modules(GSTREAMER REQUIRED IMPORTED_TARGET gstreamer-1.0)
pkg_check_modules(OPENCV REQUIRED IMPORTED_TARGET opencv4)
if(USE_PIGPIO)
     find_package(pigpio REQUIRED)
endif()
if(USE_LGPIO)
     # TODO
endif()
if(USE_SERIALPORT)
     pkg_check_modules(SERIALPORT REQUIRED IMPORTED_TARGET libserialport)
endif()


# ArPiRobot core library
file (GLOB_RECURSE SOURCES
     ${PROJECT_SOURCE_DIR}/src/*.c
     ${PROJECT_SOURCE_DIR}/src/*.cpp
     ${PROJECT_SOURCE_DIR}/include/*.h
     ${PROJECT_SOURCE_DIR}/include/*.hpp
)
add_library(arpirobot-core SHARED ${SOURCES} ${PROJECT_SOURCE_DIR}/deps/miniaudio/miniaudio.h)
target_include_directories(arpirobot-core PUBLIC 
     ${PROJECT_SOURCE_DIR}/include 
     ${PROJECT_SOURCE_DIR}/deps 
     ${PROJECT_SOURCE_DIR}/deps/miniaudio/)
target_link_libraries(arpirobot-core Threads::Threads PkgConfig::GSTREAMER PkgConfig::OPENCV 
          Boost::headers Boost::system dl)
include(GenerateExportHeader)
generate_export_header(arpirobot-core)
if(USE_PIGPIO)
     target_compile_definitions(arpirobot-core PUBLIC HAS_PIGPIO)
     target_link_libraries(arpirobot-core pigpio)
endif()
if(USE_LGPIO)
     target_compile_definitions(arpirobot-core PUBLIC HAS_LGPIO)
     # TODO: Link
endif()
if(USE_SERIALPORT)
     target_compile_definitions(arpirobot-core PUBLIC HAS_SERIALPORT)
     target_link_libraries(arpirobot-core PkgConfig::SERIALPORT)
endif()


# C++ testrobot program
file (GLOB_RECURSE SOURCES_TESTROBOT
     ${PROJECT_SOURCE_DIR}/testrobot/src/*.cpp
     ${PROJECT_SOURCE_DIR}/testrobot/include/*.hpp
)
list(LENGTH SOURCES_TESTROBOT TR_LEN)
if(${TR_LEN} GREATER 0)
     add_executable(testrobot ${SOURCES_TESTROBOT})
     add_dependencies(testrobot arpirobot-core)
     target_include_directories(testrobot PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/testrobot/include)
     target_link_libraries(testrobot arpirobot-core)
     target_compile_options(testrobot PRIVATE)
endif()
