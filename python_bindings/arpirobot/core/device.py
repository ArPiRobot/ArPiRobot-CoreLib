import arpirobot.bridge as bridge
import ctypes


## Base class for any device connected to the raspberry pi.
class BaseDevice:
    def __init__(self):
        self._ptr = None
    
    ## Get the human-readable name of the device
    #  @returns the devices human readable name
    def get_device_name(self) -> str:
        res = ctypes.c_char_p(bridge.arpirobot.BaseDevice_getDeviceName(self._ptr))
        retval = res.value.decode()
        bridge.arpirobot.freeString(res)
        return retval


## Base class for motor controllers.
class MotorController(BaseDevice):

    ## Check if the motor direction is inverted (positive and negative speed switched)
    #  @returns true if inverted, else false
    def is_inverted(self) -> bool:
        return bridge.arpirobot.MotorController_isInverted(self._ptr)

    ## Set the motor direction to inverted or non-inverted (positive and negative speed switched)
    #  @param inverted True for inverted, false for non-inverted
    def set_inverted(self, inverted: bool):
        bridge.arpirobot.MotorController_setInverted(self._ptr, inverted)

    ## Check if brake mode is enabled. When enabled motors with a speed of zero will resist motion.
    # @returns true if enabled, else false
    def is_brake_mode(self) -> bool:
        return bridge.arpirobot.MotorController_isBrakeMode(self._ptr)

    ## Set if brake mode is enabled
    #  @param brake_mode true for enabled, false for disabled
    def set_brake_mode(self, brake_mode):
        bridge.arpirobot.MotorController_setBrakeMode(self._ptr, brake_mode)

    ## Set the current speed of the motor (no effect if motor is disabled)
    #  @param speed The motor's speed (between -1.0 and 1.0)
    def set_speed(self, speed: float):
        bridge.arpirobot.MotorController_setSpeed(self._ptr, speed)

    ## Get the current speed of the motor
    #  @returns the motor's speed
    def get_speed(self) -> float:
        return bridge.arpirobot.MotorController_getSpeed(self._ptr)
