cmake_minimum_required (VERSION 3.0)
project (ArPiRobot-CoreLib C CXX)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup(TARGETS)
else()
     message(FATAL_ERROR "Run conan first")
endif()



include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/deps)

file (GLOB_RECURSE SOURCES
     ${PROJECT_SOURCE_DIR}/src/*.c
     ${PROJECT_SOURCE_DIR}/src/*.cpp
     ${PROJECT_SOURCE_DIR}/include/*.h
     ${PROJECT_SOURCE_DIR}/include/*.hpp
)

add_library(arpirobot-core SHARED ${SOURCES})
