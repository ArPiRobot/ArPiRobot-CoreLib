#!/bin/bash

# cd to scripts directory
DIR=$(realpath $(dirname $0))
cd $DIR

rm -rf build
mkdir -p build
cd build
conan install .. -pr ../conan_profiles/pi_zero --build missing
