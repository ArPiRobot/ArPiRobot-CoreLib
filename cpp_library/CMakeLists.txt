cmake_minimum_required (VERSION 3.0)
project (ArPiRobot-CoreLib C CXX)

# Build pigpio as static library
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/pigpio-71 EXCLUDE_FROM_ALL)

# ArPiRobot core library

file (GLOB_RECURSE SOURCES
     ${PROJECT_SOURCE_DIR}/src/*.c
     ${PROJECT_SOURCE_DIR}/src/*.cpp
     ${PROJECT_SOURCE_DIR}/include/*.h
     ${PROJECT_SOURCE_DIR}/include/*.hpp
)

add_library(arpirobot-core SHARED ${SOURCES})
add_dependencies(arpirobot-core pigpio)
target_include_directories(arpirobot-core PUBLIC 
     ${PROJECT_SOURCE_DIR}/include 
     ${PROJECT_SOURCE_DIR}/deps 
     ${PROJECT_SOURCE_DIR}/deps/pigpio-71
     ${PROJECT_SOURCE_DIR}/deps/asio-1.18.1/include)
target_link_libraries(arpirobot-core pigpio pthread)
target_compile_options(arpirobot-core PRIVATE -Wno-psabi)


file (GLOB_RECURSE SOURCES_TESTROBOT
     ${PROJECT_SOURCE_DIR}/testrobot/*.cpp
)

add_executable(testrobot ${SOURCES_TESTROBOT})
add_dependencies(testrobot arpirobot-core)
target_include_directories(testrobot PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(testrobot arpirobot-core)
target_compile_options(testrobot PRIVATE -Wno-psabi)
