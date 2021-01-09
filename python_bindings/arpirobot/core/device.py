import arpirobot.bridge as bridge
import ctypes


class BaseDevice:
    def __init__(self):
        self._ptr = None
    
    def get_device_name(self) -> str:
        res = ctypes.c_char_p(bridge.arpirobot.BaseDevice_getDeviceName(self._ptr))
        retval = res.value.decode()
        bridge.arpirobot.freeString(res)
        return retval


class MotorController(BaseDevice):
    def is_inverted(self) -> bool:
        return bridge.arpirobot.MotorController_isInverted(self._ptr)

    def set_inverted(self, inverted: bool):
        bridge.arpirobot.MotorController_setInverted(self._ptr, inverted)

    def is_brake_mode(self) -> bool:
        return bridge.arpirobot.MotorController_isBrakeMode(self._ptr)

    def set_brake_mode(self, brake_mode):
        bridge.arpirobot.MotorController_setBrakeMode(self._ptr, brake_mode)

    def set_speed(self, speed: float):
        bridge.arpirobot.MotorController_setSpeed(self._ptr, speed)

    def get_speed(self) -> float:
        return bridge.arpirobot.MotorController_getSpeed(self._ptr)
