cmake_minimum_required (VERSION 3.20)
project (ArPiRobot-CoreLib C CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Build pigpio as shared library on supported platforms
if(("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux") AND (("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm") OR ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")))
     set(BUILDING_PIGPIO TRUE)
     add_subdirectory(${PROJECT_SOURCE_DIR}/deps/pigpio-79 EXCLUDE_FROM_ALL)
     target_compile_options(pigpio PRIVATE -fPIC)     
endif()

# Build serial as shared library on windows, linux, or macos
if(("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux") OR ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows") OR ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin"))
     set(BUILDING_SERIAL TRUE)
     add_subdirectory(${PROJECT_SOURCE_DIR}/deps/serial/ EXCLUDE_FROM_ALL)
     target_compile_options(serial PRIVATE -fPIC)
endif()

# Build libsoc as shared library on linux systems
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
     set(BUILDING_LIBSOC TRUE)
     add_subdirectory(${PROJECT_SOURCE_DIR}/deps/libsoc EXCLUDE_FROM_ALL)
     target_compile_options(soc PRIVATE -fPIC)
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
     target_compile_definitions(arpirobot-core PUBLIC HAS_PIGPIO)
     target_include_directories(arpirobot-core PUBLIC ${PROJECT_SOURCE_DIR}/deps/pigpio-79)
     target_link_libraries(arpirobot-core pigpio)

     # Copy pigpio library to build directory so it is included in update packages
     add_custom_command(TARGET arpirobot-core POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pigpio> ${CMAKE_CURRENT_BINARY_DIR}
     )
endif()

if(${BUILDING_SERIAL})
     add_dependencies(arpirobot-core serial)
     target_compile_definitions(arpirobot-core PUBLIC HAS_SERIAL)
     target_include_directories(arpirobot-core PUBLIC ${PROJECT_SOURCE_DIR}/deps/serial/include)
     target_link_libraries(arpirobot-core serial)

     # Copy serial library to build directory so it is included in update packages
     add_custom_command(TARGET arpirobot-core POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:serial> ${CMAKE_CURRENT_BINARY_DIR}
     )
endif()

if(${BUILDING_LIBSOC})
     add_dependencies(arpirobot-core soc)
     target_compile_definitions(arpirobot-core PUBLIC HAS_LIBSOC)
     target_include_directories(arpirobot-core PUBLIC ${PROJECT_SOURCE_DIR}/deps/libsoc/include)

     target_link_libraries(arpirobot-core soc)

     # Copy libsoc library to build directory so it is included in update packages
     add_custom_command(TARGET arpirobot-core POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:soc> ${CMAKE_CURRENT_BINARY_DIR}
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

list(LENGTH SOURCES_TESTROBOT TR_LEN)

if(${TR_LEN} GREATER 0)
     add_executable(testrobot ${SOURCES_TESTROBOT})
     add_dependencies(testrobot arpirobot-core)
     target_include_directories(testrobot PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/testrobot/include)
     target_link_libraries(testrobot arpirobot-core)
     target_compile_options(testrobot PRIVATE -Wno-psabi)
endif()
