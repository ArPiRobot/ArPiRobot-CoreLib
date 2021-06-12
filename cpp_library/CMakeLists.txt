cmake_minimum_required (VERSION 3.0)
project (ArPiRobot-CoreLib C CXX)

# Build pigpio as shared library on supported platforms
if(("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux") AND ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm"))
     set(BUILDING_PIGPIO TRUE)
     add_subdirectory(${PROJECT_SOURCE_DIR}/deps/pigpio-71 EXCLUDE_FROM_ALL)
     target_compile_options(pigpio PRIVATE -fPIC)
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
     ${PROJECT_SOURCE_DIR}/deps/asio-1.18.1/include
     ${PROJECT_SOURCE_DIR}/deps/miniaudio/)
target_link_libraries(arpirobot-core pthread)
target_compile_options(arpirobot-core PRIVATE -Wno-psabi)

if(${BUILDING_PIGPIO})
     add_dependencies(arpirobot-core pigpio)
     target_include_directories(arpirobot-core PUBLIC ${PROJECT_SOURCE_DIR}/deps/pigpio-71)
     target_link_libraries(arpirobot-core pigpio)

     # Copy pigpio library to build directory so it is included in update packages
     add_custom_command(TARGET arpirobot-core POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pigpio> ${CMAKE_CURRENT_BINARY_DIR}
     )
endif()

if(NOT WIN32)
     # miniaudio requires dl (dlfcn) library on Linux
     target_link_libraries(arpirobot-core dl)
endif()

# Enable building on windows for testing purposes
if(WIN32)
     target_compile_definitions(arpirobot-core PUBLIC _WIN32_WINNT=0x0501)
     target_link_libraries(arpirobot-core ws2_32 wsock32)
endif()


file (GLOB_RECURSE SOURCES_TESTROBOT
     ${PROJECT_SOURCE_DIR}/testrobot/src/*.cpp
     ${PROJECT_SOURCE_DIR}/testrobot/include/*.hpp
)

add_executable(testrobot ${SOURCES_TESTROBOT})
add_dependencies(testrobot arpirobot-core)
target_include_directories(testrobot PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/testrobot/include)
target_link_libraries(testrobot arpirobot-core)
target_compile_options(testrobot PRIVATE -Wno-psabi)
