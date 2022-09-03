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

from arpirobot.core.network import MainVmon
import arpirobot.bridge as bridge
import ctypes
from arpirobot.core.device import BaseDevice


## INA260 Power sensor
class INA260PowerSensor(BaseDevice, MainVmon):
    def __init__(self, bus = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.INA260PowerSensor_create(bus)
    
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
