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
from arpirobot.core.device import MotorController


## Motor for Adafruit Motor Hat
class AdafruitMotorHatMotor(MotorController):
    DETECT_ADDR = -1
    ADAFRUIT_ADDR = 0x60
    GEEKWORM_ADDR = 0x6F

    ## @param motor_num The number for the motor
    #  @param address The address of the hat for the motor
    #  @param remap_nums If true the motor numbers will be remapped (if needed) to match the 
    #                      physical order of the Adafruit motor hat
    def __init__(self, motor_num: int, hat_address: int = DETECT_ADDR, remap_nums: bool = True):
        super().__init__()
        self._ptr = bridge.arpirobot.AdafruitMotorHatMotor_create(motor_num, hat_address, remap_nums)
    
    def __del__(self):
        bridge.arpirobot.AdafruitMotorHatMotor_destroy(self._ptr)
