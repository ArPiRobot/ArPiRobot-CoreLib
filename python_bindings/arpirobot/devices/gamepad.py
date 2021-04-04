"""
Copyright 2021 Marcus Behel

This file is part of ArPiRobot-CoreLib.

ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ArPiRobot-CoreLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
"""

import arpirobot.bridge as bridge
import ctypes
from arpirobot.core.device import BaseDevice
from arpirobot.core.drive import BaseAxisTransform
from arpirobot.core.action import BaseActionTrigger, Action


## Gamepad receiving data from drive station
class Gamepad(BaseDevice):
    ## @param controller_num The controller number for this controller. Top controller in DS is zero
    def __init__(self, controller_num: int):
        super().__init__()
        self._ptr = bridge.arpirobot.Gamepad_create(ctypes.c_int(controller_num))
        self.__axis_transforms = {}
    
    def __del__(self):
        bridge.arpirobot.Gamepad_destroy(self._ptr)
    
    ## Get the controller number for this gamepad
    #  @returns The controller number
    def get_controller_num(self) -> int:
        return bridge.arpirobot.Gamepad_getControllerNum(self._ptr).value
    
    ## Get an axis for this controller
    #  @param axis_num The axis number
    #  @param deadband A minimum threshold for axis values. Values below this will be returned as zero.
    #  @returns The axis value after applying the deadband and (if required) an axis transform
    def get_axis(self, axis_num: int, deadband: float = 0) -> float:
        return bridge.arpirobot.Gamepad_getAxis(self._ptr, axis_num, deadband)
    
    ## Get a button for the controller
    #  @param button_num The button number
    #  @returns true if pressed, else false
    def get_button(self, button_num: int) -> bool:
        return bridge.arpirobot.Gamepad_getButton(self._ptr, button_num)
    
    ## Get the value of a dpad
    #  @param dpad_num The dpad number
    #  @returns 0 if center, 1 for up through 8 going clockwise
    def get_dpad(self, dpad_num: int) -> int:
        return bridge.arpirobot.Gamepad_getDpad(self._ptr, dpad_num)

    ## Set the axis transform for a given axis
    #  @param axis_num The axis number to apply a transform to
    #  @param transform The transform
    def set_axis_transform(self, axis_num: int, transform: BaseAxisTransform):
        # Keep a reference to the BaseAxisTransform object or it will be deallocated
        self.__axis_transforms[axis_num] = transform
        bridge.arpirobot.Gamepad_setAxisTransform(self._ptr, axis_num, transform._ptr)
    
    ## Remove a transform from an axis
    #  @param axis_num The axis number to clear a transform from
    def clear_axis_transform(self, axis_num: int):
        if axis_num in self.__axis_transforms:
            del self.__axis_transforms[axis_num]
        bridge.arpirobot.Gamepad_clearAxisTransform(self._ptr, axis_num)


## Action trigger for when a button on a gamepad is pressed
class ButtonPressedTrigger(BaseActionTrigger):
    ## @param gamepad The gamepad to check for button presses on
    #  @param button_num The button number
    #  @param target_action The action to run when this occurs
    #  @param do_restart If true this will restart the action if it is running when this occurs
    def __init__(self, gamepad: Gamepad, button_num: int, target_action: Action, do_restart: bool = True):
        super().__init__(target_action)
        self._ptr = bridge.arpirobot.ButtonPressedTrigger_create(gamepad._ptr, button_num, 
            target_action._ptr, do_restart)
    
    def __del__(self):
        bridge.arpirobot.ButtonPressedTrigger_destroy(self._ptr)


## Action trigger for when a button on a gamepad is released
class ButtonReleasedTrigger(BaseActionTrigger):
    ## @param gamepad The gamepad to check for button presses on
    #  @param button_num The button number
    #  @param target_action The action to run when this occurs
    #  @param do_restart If true this will restart the action if it is running when this occurs
    def __init__(self, gamepad: Gamepad, button_num: int, target_action: Action, do_restart: bool = True):
        super().__init__(target_action)
        self._ptr = bridge.arpirobot.ButtonReleasedTrigger_create(gamepad._ptr, button_num, 
            target_action._ptr, do_restart)
    
    def __del__(self):
        bridge.arpirobot.ButtonReleasedTrigger_destroy(self._ptr)
