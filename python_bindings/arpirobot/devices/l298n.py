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


## Motor on L298N module
class L298NMotor(MotorController):
    ## @param in1_pin pin for side 1 directional control
    #  @param in2_pin pin for side 2 directional control
    #  @param pwm_pin pin for pwm of motor
    def __init__(self, in1_pin: int, in2_pin: int, pwm_pin: int):
        super().__init__()
        self._ptr = bridge.arpirobot.L298NMotor_create(in1_pin, in2_pin, pwm_pin)
    
    def __del__(self):
        bridge.arpirobot.L298NMotor_destroy(self._ptr)


## L298N Module (two motors)
class L298NModule:
    ## @param in1_pin In1 pin for module (motor a)
    #  @param in2_pin In2 pin for module (motor a)
    #  @param ena_pin PWM pin for motor a of module
    #  @param in3_pin In3 pin for module (motor b)
    #  @param in4_pin In4 pin for module (motor b)
    #  @param enb_pin PWM pin for motor b of module
    def __init__(self, in1_pin: int, in2_pin: int, ena_pin: int, in3_pin: int, in4_pin: int, enb_pin: int):
        self.__motorA = L298NMotor(in1_pin, in2_pin, ena_pin)
        self.__motorB = L298NMotor(in3_pin, in4_pin, enb_pin)
    
    ## Get the Motor instance for motor a
    def get_motor_a(self) -> L298NMotor:
        return self.__motorA
    
    ## Get the Motor instance for motor b
    def get_motor_b(self) -> L298NMotor:
        return self.__motorB
