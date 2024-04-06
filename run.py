#!/usr/bin/env python3

import os
import platform
import argparse

if platform.system() != "Linux":
    print("Can only run on linux system!")

script_dir = os.path.dirname(os.path.realpath(__file__))

parser = argparse.ArgumentParser(description="Run testrobot program on this PC. Notice: you must build the cpp library for this pc before running the testrobot programs.")
parser.add_argument("program", help="Which testrobot program to run.", type=str, choices=["cpp", "python"])
parser.add_argument("config", help="Which cpp configuration to use", type=str, choices=["Debug", "Release", "MinSizeRel", "RelWithDebInfo"])
args = parser.parse_args()

try:
    if args.program == "cpp":
        if "LD_LIBRARY_PATH" not in os.environ:
            os.environ["LD_LIBRARY_PATH"] = ""
        os.environ["LD_LIBRARY_PATH"] += os.pathsep + os.path.join(script_dir, "cpp_library", "build", "native", args.config)
        os.system(os.path.join("cpp_library", "build", "native", args.config, "testrobot.exe" if platform.system() == "Windows" else "testrobot"))
    elif args.program == "python":
        os.environ["PYTHONPATH"] = os.path.join(script_dir, "python_bindings")
        if "LD_LIBRARY_PATH" not in os.environ:
            os.environ["LD_LIBRARY_PATH"] = ""
        os.environ["LD_LIBRARY_PATH"] += os.pathsep + os.path.join(script_dir, "cpp_library", "build", "native", args.config)
        os.system("python3 " + os.path.join("python_bindings", "testrobot-py", "main.py"))
except KeyboardInterrupt:
    pass