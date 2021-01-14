#!/bin/bash

DIR=$(realpath $(dirname $0))
cd $DIR

sudo LD_LIBRARY_PATH=. ./testrobot
