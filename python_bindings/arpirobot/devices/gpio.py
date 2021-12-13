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


## Status LED
## While robot is enabled the LED is blinking
## While robot is disabled the LED is solid
class StatusLED(BaseDevice):
    def __init__(self, pin: int):
        super().__init__()
        self._ptr = bridge.arpirobot.StatusLED_create(pin)
    
    def __del__(self):
        bridge.arpirobot.StatusLED_destroy(self._ptr)