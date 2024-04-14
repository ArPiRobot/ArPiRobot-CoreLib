cmake_minimum_required (VERSION 3.20)
project (ArPiRobot-CoreLib C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# System dependencies
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED COMPONENTS headers)
pkg_check_modules(GSTREAMER REQUIRED IMPORTED_TARGET gstreamer-1.0)
pkg_check_modules(OPENCV REQUIRED IMPORTED_TARGET opencv4)

# Dependencies built from source

add_subdirectory(${PROJECT_SOURCE_DIR}/deps/pigpio-79 EXCLUDE_FROM_ALL)
target_compile_options(pigpio PRIVATE -fPIC)

add_subdirectory(${PROJECT_SOURCE_DIR}/deps/serial/ EXCLUDE_FROM_ALL)
target_compile_options(serial PRIVATE -fPIC)

add_subdirectory(${PROJECT_SOURCE_DIR}/deps/libsoc EXCLUDE_FROM_ALL)
target_compile_options(soc PRIVATE -fPIC)


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
target_link_libraries(arpirobot-core Threads::Threads PkgConfig::GSTREAMER PkgConfig::OPENCV Boost::headers)
include(GenerateExportHeader)
generate_export_header(arpirobot-core)

add_dependencies(arpirobot-core pigpio)
target_compile_definitions(arpirobot-core PUBLIC HAS_PIGPIO)
target_include_directories(arpirobot-core PUBLIC ${PROJECT_SOURCE_DIR}/deps/pigpio-79)
target_link_libraries(arpirobot-core pigpio)

# Copy pigpio library to build directory so it is included in update packages
add_custom_command(TARGET arpirobot-core POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pigpio> $<OUTPUT_CONFIG:$<CONFIG>>
)

add_dependencies(arpirobot-core serial)
target_compile_definitions(arpirobot-core PUBLIC HAS_SERIAL)
target_include_directories(arpirobot-core PUBLIC ${PROJECT_SOURCE_DIR}/deps/serial/include)
target_link_libraries(arpirobot-core serial)

# Copy serial library to build directory so it is included in update packages
add_custom_command(TARGET arpirobot-core POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:serial> $<OUTPUT_CONFIG:$<CONFIG>>
)

add_dependencies(arpirobot-core soc)
target_compile_definitions(arpirobot-core PUBLIC HAS_LIBSOC)
target_include_directories(arpirobot-core PUBLIC ${PROJECT_SOURCE_DIR}/deps/libsoc/include)

target_link_libraries(arpirobot-core soc)

# Copy libsoc library to build directory so it is included in update packages
add_custom_command(TARGET arpirobot-core POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:soc> $<OUTPUT_CONFIG:$<CONFIG>>
)

# miniaudio requires dl (dlfcn) library on Linux
target_link_libraries(arpirobot-core dl)


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
