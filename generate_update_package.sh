#!/bin/bash

DIR=$(realpath $(dirname $0))
cd "$DIR"

rm -rf package
mkdir -p package/
mkdir -p package/python_bindings
cp version.txt package/

echo "Building C++ Library"
cd cpp_library
rm -rf build
mkdir -p build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../arpirobot-cross.cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
cmake --build .
cp libarpirobot-core.so ../../package/
cp -r ../include ../../package/

cd ../..

echo ""
echo "Adding Python Bindings"
cp -r python_bindings/arpirobot package/python_bindings


echo ""
echo "Generating Package"
cd package
zip -r ../ArPiRobot-CoreLib-VERSION.zip ./*
mv ../ArPiRobot-CoreLib-VERSION.zip ./

echo ""
echo "Done."
