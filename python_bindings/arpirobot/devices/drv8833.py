import arpirobot.bridge as bridge
import ctypes
from arpirobot.core.device import MotorController


class DRV8833Motor(MotorController):
    def __init__(self, in1_pin: int, in2_pin: int, pwm_pin: int):
        super().__init__()
        self._ptr = bridge.arpirobot.DRV8833Motor_create(in1_pin, in2_pin, pwm_pin)
    
    def __del__(self):
        bridge.arpirobot.DRV8833Motor_destroy(self._ptr)


class DRV8833Module:
    def __init__(self, a_in1_pin: int, a_in2_pin:int, b_in1_pin: int, b_in2_pin: int, slp_pin: int):
        self.__motorA = DRV8833Motor(a_in1_pin, a_in2_pin, slp_pin)
        self.__motorB = DRV8833Motor(b_in1_pin, b_in2_pin, slp_pin)
    
    def get_motor_a(self) -> DRV8833Motor:
        return self.__motorA
    
    def get_motor_b(self) -> DRV8833Motor:
        return self.__motorB
