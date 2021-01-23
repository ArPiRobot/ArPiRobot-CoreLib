from arpirobot.core.network import MainVmon
import arpirobot.bridge as bridge
import ctypes
from arpirobot.core.device import BaseDevice


## INA260 Power sensor
class INA260PowerSensor(BaseDevice, MainVmon):
    def __init__(self):
        super().__init__()
        self._ptr = bridge.arpirobot.INA260PowerSensor_create()
    
    def __del__(self):
        bridge.arpirobot.INA260PowerSensor_destroy(self._ptr)
    
    ## Get current measurement
    #  @returns Current in mA
    def get_current(self) -> float:
        return bridge.arpirobot.INA260PowerSensor_getCurrent(self._ptr)

    ## Get voltage measurement
    #  @returns Voltage in V
    def get_voltage(self) -> float:
        return bridge.arpirobot.INA260PowerSensor_getVoltage(self._ptr)

    ## Get power measurement
    #  @returns Power in mW
    def get_power(self) -> float:
        return bridge.arpirobot.INA260PowerSensor_getPower(self._ptr)
