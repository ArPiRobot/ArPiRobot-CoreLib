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
from arpirobot.core.device import BaseDevice
from enum import IntEnum


## GPIO Pin
## Use this device to directly control a GPIO pin
## This allows using the pin either as an input or an output
## Supported operations:
## - Digital Write & Read
## - PWM Value (0-255)
## - PWM Frequency (Hz)
class GPIOPin(BaseDevice):

    class Mode(IntEnum):
        Input = 0
        Output = 1
    
    class Level(IntEnum):
        Low = 0
        High = 1

    def __init__(self, pin: int):
        super().__init__()
        self._ptr = bridge.arpirobot.GPIOPin_create(pin)
    
    def __del__(self):
        bridge.arpirobot.GPIOPin_destroy(self._ptr)
    
    def set_mode(self, mode: Mode):
        bridge.arpirobot.GPIOPin_setMode(self._ptr, 1)
    
    def set_level(self, level: Level):
        bridge.arpirobot.GPIOPin_setLevel(self._ptr, int(level))
    
    def get_level(self) -> Level:
        return GPIOPin.Level(bridge.arpirobot.GPIOPin_getLevel(self._ptr))
    
    def set_pwm_value(self, value: int):
        if value > 255:
            value = 255
        elif value < 0:
            value = 0
        bridge.arpirobot.GPIOPin_setPwmValue(self._ptr, value)
    
    def get_pwm_value(self) -> int:
        return bridge.arpirobot.GPIOPin_getPwmValue(self._ptr)
    
    def set_pwm_frequency(self, freq: int):
        if(freq < 0):
            freq = 0
        bridge.arpirobot.GPIOPin_setPwmFrequency(self._ptr, freq)
    
    def get_pwm_frequency(self) -> int:
        return bridge.arpirobot.GPIOPin_getPwmFrequency(self._ptr)


## Status LED
## While robot is enabled the LED is blinking
## While robot is disabled the LED is solid
class StatusLED(BaseDevice):
    def __init__(self, pin: int):
        super().__init__()
        self._ptr = bridge.arpirobot.StatusLED_create(pin)
    
    def __del__(self):
        bridge.arpirobot.StatusLED_destroy(self._ptr)