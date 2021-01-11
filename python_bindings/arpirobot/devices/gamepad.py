import arpirobot.bridge as bridge
import ctypes
from arpirobot.core.device import BaseDevice
from arpirobot.core.drive import BaseAxisTransform
from arpirobot.core.action import BaseActionTrigger, Action


class Gamepad(BaseDevice):
    def __init__(self, controller_num: int):
        super().__init__()
        self._ptr = bridge.arpirobot.Gamepad_create(ctypes.c_int(controller_num))
        self.__axis_transforms = {}
    
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

    def set_axis_transform(self, axis_num: int, transform: BaseAxisTransform):
        # Keep a reference to the BaseAxisTransform object or it will be deallocated
        self.__axis_transforms[axis_num] = transform
        bridge.arpirobot.Gamepad_setAxisTransform(self._ptr, axis_num, transform._ptr)
    
    def clear_axis_transform(self, axis_num: int):
        if axis_num in self.__axis_transforms:
            del self.__axis_transforms[axis_num]
        bridge.arpirobot.Gamepad_clearAxisTransform(self._ptr, axis_num)


class ButtonPressedTrigger(BaseActionTrigger):
    def __init__(self, gamepad: Gamepad, button_num: int, target_action: Action, do_restart: bool = True):
        super().__init__()
        self._ptr = bridge.arpirobot.ButtonPressedTrigger_create(gamepad._ptr, button_num, 
            target_action._ptr, do_restart)
    
    def __del__(self):
        bridge.arpirobot.ButtonPressedTrigger_destroy(self._ptr)


class ButtonReleasedTrigger(BaseActionTrigger):
    def __init__(self, gamepad: Gamepad, button_num: int, target_action: Action, do_restart: bool = True):
        super().__init__()
        self._ptr = bridge.arpirobot.ButtonReleasedTrigger_create(gamepad._ptr, button_num, 
            target_action._ptr, do_restart)
    
    def __del__(self):
        bridge.arpirobot.ButtonReleasedTrigger_destroy(self._ptr)
