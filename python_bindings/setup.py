#!/usr/bin/env python

from distutils.core import setup
import os

ver = "UNKNOWN"
with open(os.path.join(os.path.dirname(__file__), "..", "version.txt"), "r") as verfile:
    ver = verfile.readline().strip()

setup(name='arpirobot-corelib',
    version=ver,
    url='https://arpirobot.github.io/latest/',
    packages=['arpirobot'],
)