#!/bin/bash

DIR=$(realpath $(dirname $0))
cd $DIR

chmod +x ./testrobot
sudo LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ./testrobot
