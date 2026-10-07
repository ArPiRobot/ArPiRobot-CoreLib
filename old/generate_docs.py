#!/usr/bin/env python3

import os

script_dir = os.path.dirname(os.path.realpath(__file__))

original_dir = os.getcwd()

os.chdir(script_dir)

if not os.path.exists("doxygen-docs"):
    os.mkdir("doxygen-docs")

os.chdir("cpp_library")
os.system("doxygen Doxyfile")

os.chdir(os.path.join("..", "python_bindings"))
os.system("doxygen Doxyfile")

os.chdir(original_dir)
