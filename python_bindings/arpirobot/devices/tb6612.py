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


## Motor on TB6612 module
class TB6612Motor(MotorController):
    ## @param in1_pin pin for side 1 directional control
    #  @param in2_pin pin for side 2 directional control
    #  @param pwm_pin pin for pwm of motor
    def __init__(self, in1_pin: int, in2_pin: int, pwm_pin: int):
        super().__init__()
        self._ptr = bridge.arpirobot.TB6612Motor_create(in1_pin, in2_pin, pwm_pin)
    
    def __del__(self):
        bridge.arpirobot.TB6612Motor_destroy(self._ptr)


## TB6612 Module (two motors)
class TB6612Module:
    ## @param a_in1_pin In1 pin for motor a of module
    #  @param a_in2_pin In2 pin for motor a of module
    #  @param pwma_pin PWM pin for motor a of module
    #  @param b_in1_pin In1 pin for motor b of module
    #  @param b_in2_pin In2 pin for motor b of module
    #  @param pwmb_pin PWM pin for motor b of module
    def __init__(self, a_in1_pin: int, a_in2_pin: int, pwma_pin: int, b_in1_pin: int, b_in2_pin: int, pwmb_pin: int):
        self.__motorA = TB6612Motor(a_in1_pin, a_in2_pin, pwma_pin)
        self.__motorB = TB6612Motor(b_in1_pin, b_in2_pin, pwmb_pin)
    
    ## Get the Motor instance for motor a
    def get_motor_a(self) -> TB6612Motor:
        return self.__motorA
    
    ## Get the Motor instance for motor b
    def get_motor_b(self) -> TB6612Motor:
        return self.__motorB
