import ctypes
import time


@ctypes.CFUNCTYPE(None)
def robot_started():
    print("robot_started")

@ctypes.CFUNCTYPE(None)
def robot_enabled():
    print("robot_enabled")

@ctypes.CFUNCTYPE(None)
def robot_disabled():
    print("robot_disabled")

@ctypes.CFUNCTYPE(None)
def periodic():
    print("periodic")

@ctypes.CFUNCTYPE(None)
def enabled_periodic():
    print("enabled_periodic")

@ctypes.CFUNCTYPE(None)
def disabled_periodic():
    print("disabled_periodic")

arpirobot = ctypes.CDLL('./libarpirobot-core.so')
robot = arpirobot.BaseRobot_create(robot_started, robot_enabled, robot_disabled, enabled_periodic, disabled_periodic, periodic)


try:
    while True:
        arpirobot.BaseRobot_feedWatchdog(robot);
        time.sleep(0.1)
except:
    arpirobot.BaseRobot_destroy(robot)
