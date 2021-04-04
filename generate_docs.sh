#!/bin/bash

DIR=$(realpath $(dirname $0))
cd $DIR

mkdir -p doxygen-docs
cd cpp_library
doxygen Doxyfile
cd ../python_bindings
doxygen Doxyfile
cd ../java_bindings
doxygen Doxyfile

