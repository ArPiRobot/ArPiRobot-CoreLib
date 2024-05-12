from setuptools import setup, find_packages
import os

ver = "UNKNOWN"
with open(os.path.join(os.path.dirname(__file__), "..", "version.txt"), "r") as verfile:
    ver = verfile.readline().strip()

setup(
    name='arpirobot-corelib',
    version=ver,
    packages=find_packages(exclude="testrobot-py"),
    url='https://arpirobot.github.io/',
)
