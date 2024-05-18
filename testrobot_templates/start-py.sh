#!/bin/bash

if [ $# -gt 2 ]; then
    echo "Usage: start-cpp.sh [--debug]"
    exit 1
fi
DEBUG="false"
if [ $# -eq 1 ]; then
    DEBUG="true"
fi

DIR=$(realpath $(dirname $0))
cd $DIR
if [ "$DEBUG" = "true" ]; then
    sudo -E LD_LIBRARY_PATH=. PYTHONPATH=. python3 -m debugpy --listen 0.0.0.0:2000 --wait-for-client testrobot-py/main.py
else
    sudo -E LD_LIBRARY_PATH=. PYTHONPATH=. python3 testrobot-py/main.py
fi
