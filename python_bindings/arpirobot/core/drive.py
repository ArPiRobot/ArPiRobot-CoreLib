import arpirobot.bridge as bridge
import ctypes
from typing import Union, List
from arpirobot.core.device import MotorController


## Drive helper for arcade drive method.
# 
#  Drive helpers calculate speeds for each motor based on a set of inputs and update all motors speeds at once.
#  Arcade drive uses two inputs: speed and rotation.
class ArcadeDriveHelper:
    ## @param left_motors A set of left motors (or a single motor instace)
    #  @param right_motors A set of right motors (or a single motor instace)
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
    
    ## Set a new speed value. Speeds will be re-calculated and updated for each motor
    #  @param speed the new speed (-1.0 to 1.0)
    def update_speed(self, speed: float):
        bridge.arpirobot.ArcadeDriveHelper_updateSpeed(self._ptr, speed)

    ## Set a new rotation value. Speeds will be re-calculated and updated for each motor
    #  @param rotation the new rotation (-1.0 to 1.0)
    def update_rotation(self, rotation: float):
        bridge.arpirobot.ArcadeDriveHelper_updateRotation(self._ptr, rotation)

    ## Set a new speed and rotation value at the same time. 
    #  Speeds will be re-calculated and updated for each motor.
    #  If updating both values use this instead of calling updateSpeed and updateRotation
    #  because this will only recalculate motor speeds once.
    #  @param speed the new speed (-1.0 to 1.0)
    #  @param rotation the new rotation (-1.0 to 1.0)
    def update(self, speed: float, rotation: float):
        bridge.arpirobot.ArcadeDriveHelper_update(self._ptr, speed, rotation)


## Drive helper for tank drive method.
#  
#  Drive helpers calculate speeds for each motor based on a set of inputs and update all motors speeds at once.
#  Tank drive uses two inputs: left speed and right speed.
class TankDriveHelper:
    ## @param left_motors A set of left motors (or a single motor instace)
    #  @param right_motors A set of right motors (or a single motor instace)
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
    
    ## Set a new left speed value. Speeds will be re-calculated and updated for each motor
    #  @param new_left_speed the new rotation (-1.0 to 1.0)
    def update_left_speed(self, new_left_speed: float):
        bridge.arpirobot.TankDriveHelper_updateLeftSpeed(self._ptr, new_left_speed)

    ## Set a new right speed value. Speeds will be re-calculated and updated for each motor
    #  @param new_right_speed the new rotation (-1.0 to 1.0)
    def update_right_speed(self, new_right_speed: float):
        bridge.arpirobot.TankDriveHelper_updateRightSpeed(self._ptr, new_right_speed)

    ## Set a new left and right speed value at the same time. 
    #  Speeds will be re-calculated and updated for each motor.
    #  If updating both values use this instead of calling updateLeftSpeed and updateRightSpeed
    #  because this will only recalculate motor speeds once.
    #  @param new_left_speed the new speed (-1.0 to 1.0)
    #  @param new_right_speed the new rotation (-1.0 to 1.0)
    def update(self, new_left_speed: float, new_right_speed: float):
        bridge.arpirobot.TankDriveHelper_update(self._ptr, new_left_speed, new_right_speed)


## Base class for axis transforms. Axis transforms take an analog axis value (-1.0 to 1.0)
#  and apply a fixed calculation to modify the curve of the axis.
#  
#  Axis transforms are most useful with drive control.
class BaseAxisTransform:
    def __init__(self):
        self._ptr = None
    
    ## Axis transforms should override this function to implement specific calculations.
    def apply_transform(self, raw_axis_value: float):
        return bridge.arpirobot.BaseAxisTransform_applyTransform(raw_axis_value)


## Transform that applies a square root function to the magnitude of the input.
#  Sign of the output will match the sign of the input
class SquareRootAxisTransform(BaseAxisTransform):
    def __init__(self):
        super().__init__()
        self._ptr = bridge.arpirobot.SquareRootAxisTransform_create()
    
    def __del__(self):
        bridge.arpirobot.SquareRootAxisTransform_destroy(self._ptr)


## Transform that applies a cubic function to the magnitude of the input.
#  The cubic function is fit to the data set (0, minPower), (0.5, midPower), (1, 1)
#  This gives a wide range of inputs that will yield a value near midPower
#  Sign of the output will match the sign of the input
class CubicAxisTransform(BaseAxisTransform):
    ## @param min_power The minimum output of this cubic function (leave as zero if unsure)
    #  @param mid_power The "middle" power where the "flat" part of the function should be positioned
    def __init__(self, min_power: float, mid_power: float):
        super().__init__()
        self._ptr = bridge.arpirobot.CubicAxisTransform_create(min_power, mid_power)
    
    def __del__(self):
        bridge.arpirobot.CubicAxisTransform_destroy(self._ptr)
