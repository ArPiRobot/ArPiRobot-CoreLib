#!/usr/bin/env python3

import os
import shutil
import glob

# Copytree that works with existing destination directory
# https://stackoverflow.com/questions/1868714/how-do-i-copy-an-entire-directory-of-files-into-an-existing-directory-using-pyth
def copytree(src, dst, symlinks=False, ignore=None):
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copy(s, d)

# Folder this script is in
script_path = os.path.dirname(os.path.realpath(__file__))

# Read version from file
with open(os.path.join(script_path, "version.txt")) as f:
    version = f.readline().strip()

print("Preparing package directory")

# Setup package directory structure
if os.path.exists(os.path.join(script_path, "package")):
    shutil.rmtree(os.path.join(script_path, "package"))
os.mkdir(os.path.join(script_path, "package"))
os.mkdir(os.path.join(script_path, "package", "python_bindings"))
os.mkdir(os.path.join(script_path, "package", "lib"))
os.mkdir(os.path.join(script_path, "package", "lib", "armv6"))
os.mkdir(os.path.join(script_path, "package", "lib", "aarch64"))

# Copy common files
shutil.copy(os.path.join(script_path, "version.txt"), os.path.join(script_path, "package"))
shutil.copy(os.path.join(script_path, "what.txt"), os.path.join(script_path, "package"))
shutil.copy(os.path.join(script_path, "COPYING"), os.path.join(script_path, "package"))
shutil.copy(os.path.join(script_path, "COPYING.LESSER"), os.path.join(script_path, "package"))


print("")
print("Building C++ Library (armv6)")

original_dir = os.getcwd()

# Perform build
os.chdir(os.path.join(script_path, "cpp_library"))
os.system("cmake --preset armv6")
os.system("cmake --build --preset armv6-release")

# Copy library files
for file in glob.glob("build/armv6/Release/*.so"):
    shutil.copy(file, os.path.join("..", "package", "lib", "armv6"))

os.chdir(original_dir)

print("")
print("Building C++ Library (aarch64)")

original_dir = os.getcwd()

# Perform build
os.chdir(os.path.join(script_path, "cpp_library"))
os.system("cmake --preset aarch64")
os.system("cmake --build --preset aarch64-release")

# Copy library files
for file in glob.glob("build/aarch64/Release/*.so"):
    shutil.copy(file, os.path.join("..", "package", "lib", "aarch64"))

os.chdir(original_dir)

print("")
print("Adding includes")
original_dir = os.getcwd()
os.chdir(os.path.join(script_path, "cpp_library"))

# arpirobot includes
shutil.copytree(os.path.join("include"), os.path.join("..", "package", "include"))

# asio includes
for file in glob.glob(os.path.join("deps", "asio-*")):
    copytree(os.path.join(file, "include"), os.path.join("..", "package", "include"))

# libsoc includes
copytree(os.path.join("deps", "libsoc", "include"), os.path.join("..", "package", "include"))

# miniaudio includes
copytree(os.path.join("deps", "miniaudio"), os.path.join("..", "package", "include"))

# pigpio includes
for file in glob.glob(os.path.join("deps", "pigpio-*", "*.h")):
    shutil.copy(file, os.path.join("..", "package", "include"))

# serial includes
copytree(os.path.join("deps", "serial", "include"), os.path.join("..", "package", "include"))

# Loose files
shutil.copy(os.path.join("deps", "ctpl_stl.h"), os.path.join("..", "package", "include"))
os.chdir(original_dir)

print("")
print("Adding python bindings")
shutil.copytree(os.path.join(script_path, "python_bindings", "arpirobot"), os.path.join(script_path, "package", "python_bindings", "arpirobot"))

print("")
print("Generating Package")

original_dir = os.getcwd()
os.chdir(os.path.join(script_path, "package"))

name = shutil.make_archive(os.path.join("..", f"ArPiRobot-CoreLib-{version}"), "zip", ".", ".")
shutil.move(os.path.join("..", f"ArPiRobot-CoreLib-{version}.zip"), os.path.join(".", f"ArPiRobot-CoreLib-{version}.zip"))

os.chdir(original_dir)