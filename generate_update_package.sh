#!/bin/bash

DIR=$(realpath $(dirname $0))
cd "$DIR"

echo "Preparing package directory"
rm -rf package
mkdir -p package/
mkdir -p package/python_bindings
mkdir -p package/lib
cp version.txt package/
cp what.txt package/

echo ""
echo "Building C++ Library"
cd cpp_library
rm -rf build
mkdir -p build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../arpirobot-cross.cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
cmake --build .
cp libarpirobot-core.so ../../package/lib/
cp -r ../include ../../package/
cp -r ../deps/asio-*/include/* ../../package/include/
cp -r ../deps/pigpio-*/*.h ../../package/include/
cp -r ../deps/ctpl_stl.h ../../package/include/

cd ../..

echo ""
echo "Adding Python Bindings"
cp -r python_bindings/arpirobot package/python_bindings


echo ""
echo "Generating Package"
cd package
zip -r ../ArPiRobot-CoreLib-VERSION.zip ./* > /dev/null
mv ../ArPiRobot-CoreLib-VERSION.zip ./

echo ""
echo "Done."