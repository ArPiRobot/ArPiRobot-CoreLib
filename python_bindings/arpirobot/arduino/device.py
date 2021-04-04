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

## Common base class for any device that is connected to an arduino (any device that communicates with the Pi via 
#  an arduino or other microprocessor)
class ArduinoDevice:
    def __init__(self):
        self._ptr = None

    ## Get the human-readable name for this device
    #  @returns The human-readable name
    def get_device_name(self) -> str:
        res = ctypes.c_char_p(bridge.arpirobot.ArduinoDevice_getDeviceName(self._ptr))
        retval = res.value.decode()
        bridge.arpirobot.freeString(res)
        return retval
