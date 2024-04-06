#!/bin/bash

DIR=$(realpath $(dirname $0))
cd $DIR
sudo -E LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ./testrobot
