import arpirobot.bridge as bridge
import ctypes
from arpirobot.core.device import MotorController


## Motor on DRV8833 module
class DRV8833Motor(MotorController):
    ## @param in1_pin pin for side 1 directional control
    #  @param in2_pin pin for side 2 directional control
    #  @param pwm_pin pin for sleep of module
    def __init__(self, in1_pin: int, in2_pin: int, pwm_pin: int):
        super().__init__()
        self._ptr = bridge.arpirobot.DRV8833Motor_create(in1_pin, in2_pin, pwm_pin)
    
    def __del__(self):
        bridge.arpirobot.DRV8833Motor_destroy(self._ptr)


## DRV8833 Module (two motors)
class DRV8833Module:
    ## @param a_in1_pin In1 pin for motor a of module
    #  @param a_in2_pin In2 pin for motor a of module
    #  @param b_in1_pin In1 pin for motor b of module
    #  @param b_in2_pin In2 pin for motor b of module
    #  @param slp_pin Sleep pin for module
    def __init__(self, a_in1_pin: int, a_in2_pin: int, b_in1_pin: int, b_in2_pin: int, slp_pin: int):
        self.__motorA = DRV8833Motor(a_in1_pin, a_in2_pin, slp_pin)
        self.__motorB = DRV8833Motor(b_in1_pin, b_in2_pin, slp_pin)
    
    ## Get the Motor instance for motor a
    def get_motor_a(self) -> DRV8833Motor:
        return self.__motorA
    
    ## Get the Motor instance for motor b
    def get_motor_b(self) -> DRV8833Motor:
        return self.__motorB
