#!/bin/bash

DIR=$(realpath $(dirname $0))
cd $DIR
sudo -E LD_LIBRARY_PATH=. PYTHONPATH=. python3 testrobot-py/main.py
