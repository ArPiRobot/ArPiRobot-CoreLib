import arpirobot.bridge as bridge
import ctypes
from arpirobot.core.device import BaseDevice


class Gamepad(BaseDevice):
    def __init__(self, controller_num: int):
        super().__init__()
        self._ptr = bridge.arpirobot.Gamepad_create(ctypes.c_int(controller_num))
    
    def __del__(self):
        bridge.arpirobot.Gamepad_destroy(self._ptr)
    
    def get_controller_num(self) -> int:
        return bridge.arpirobot.Gamepad_getControllerNum(self._ptr).value
    
    def get_axis(self, axis_num: int, deadband: float = 0) -> float:
        return bridge.arpirobot.Gamepad_getAxis(self._ptr, axis_num, deadband)
    
    def get_button(self, button_num: int) -> bool:
        return bridge.arpirobot.Gamepad_getButton(self._ptr, button_num)
    
    def get_dpad(self, dpad_num: int) -> int:
        return bridge.arpirobot.Gamepad_getDpad(self._ptr, dpad_num)
