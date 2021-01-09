import arpirobot.bridge as bridge
import ctypes
from typing import Union, List
from arpirobot.core.device import MotorController


class ArcadeDriveHelper:
    def __init__(self, left_motors: Union[MotorController, List[MotorController]], 
            right_motors:  Union[MotorController, List[MotorController]]):

        # Build lists of the internal pointers to each device
        lm = []
        rm = []
        if isinstance(left_motors, List):
            for m in left_motors:
                lm.append(m._ptr)
        else:
            lm.append(left_motors._ptr)

        if isinstance(right_motors, List):
            for m in right_motors:
                rm.append(m._ptr)
        else:
            rm.append(right_motors._ptr)

        # Pass these lists as c style arrays (pointers and a second arg for size)

        lm_type = ctypes.c_void_p * len(lm)
        rm_type = ctypes.c_void_p * len(rm)

        self._ptr = bridge.arpirobot.ArcadeDriveHelper_create(lm_type(*lm), len(lm), rm_type(*rm), len(rm))
    
    def __del__(self):
        bridge.arpirobot.ArcadeDriveHelper_destroy(self._ptr)
    
    def update_speed(self, speed: float):
        bridge.arpirobot.ArcadeDriveHelper_updateSpeed(self._ptr, speed)

    def update_rotation(self, rotation: float):
        bridge.arpirobot.ArcadeDriveHelper_updateRotation(self._ptr, rotation)

    def update(self, speed: float, rotation: float):
        bridge.arpirobot.ArcadeDriveHelper_update(self._ptr, speed, rotation)


class TankDriveHelper:
    def __init__(self, left_motors: Union[MotorController, List[MotorController]], 
            right_motors:  Union[MotorController, List[MotorController]]):

        # Build lists of the internal pointers to each device
        lm = []
        rm = []
        if isinstance(left_motors, List):
            for m in left_motors:
                lm.append(m._ptr)
        else:
            lm.append(left_motors._ptr)

        if isinstance(right_motors, List):
            for m in right_motors:
                rm.append(m._ptr)
        else:
            rm.append(right_motors._ptr)

        # Pass these lists as c style arrays (pointers and a second arg for size)

        lm_type = ctypes.c_void_p * len(lm)
        rm_type = ctypes.c_void_p * len(rm)

        self._ptr = bridge.arpirobot.TankDriveHelper_create(lm_type(*lm), len(lm), rm_type(*rm), len(rm))
    
    def __del__(self):
        bridge.arpirobot.TankDriveHelper_destroy(self._ptr)
    
    def update_left_speed(self, new_left_speed: float):
        bridge.arpirobot.TankDriveHelper_updateLeftSpeed(self._ptr, new_left_speed)

    def update_right_speed(self, new_right_speed: float):
        bridge.arpirobot.TankDriveHelper_updateRightSpeed(self._ptr, new_right_speed)

    def update(self, new_left_speed: float, new_right_speed: float):
        bridge.arpirobot.TankDriveHelper_update(self._ptr, new_left_speed, new_right_speed)