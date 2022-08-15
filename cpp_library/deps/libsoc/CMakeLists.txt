cmake_minimum_required(VERSION 3.0)
project(soc)
file (GLOB_RECURSE SOURCES
     ${PROJECT_SOURCE_DIR}/src/*.c
     ${PROJECT_SOURCE_DIR}/include/*.h
)
add_library(soc SHARED ${SOURCES})
target_compile_definitions(soc PUBLIC DATA_DIR="/usr/local/share/libsoc")
target_compile_definitions(soc PUBLIC LIBSOC_CONF="/usr/local/etc/libsoc.conf")
include_directories(include)
