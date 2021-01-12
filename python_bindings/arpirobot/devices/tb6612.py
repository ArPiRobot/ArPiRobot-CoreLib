import arpirobot.bridge as bridge
import ctypes
from arpirobot.core.device import MotorController


class TB6612Motor(MotorController):
    def __init__(self, in1_pin: int, in2_pin: int, pwm_pin: int):
        super().__init__()
        self._ptr = bridge.arpirobot.TB6612Motor_create(in1_pin, in2_pin, pwm_pin)
    
    def __del__(self):
        bridge.arpirobot.TB6612Motor_destroy(self._ptr)


class TB6612Module:
    def __init__(self, in1_pin: int, in2_pin: int, pwma_pin: int, in3_pin: int, in4_pin: int, pwmb_pin: int):
        self.__motorA = TB6612Motor(in1_pin, in2_pin, pwma_pin)
        self.__motorB = TB6612Motor(in3_pin, in4_pin, pwmb_pin)
    
    def get_motor_a(self) -> TB6612Motor:
        return self.__motorA
    
    def get_motor_b(self) -> TB6612Motor:
        return self.__motorB
