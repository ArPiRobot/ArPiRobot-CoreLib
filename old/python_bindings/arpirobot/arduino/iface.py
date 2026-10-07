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
from arpirobot.arduino.device import ArduinoDevice


## Common base class (communication protocol agnostic) for an arduino (or other microcontroller) sensor coprocessor.
#  This class handles all the high-level data between the arduino and pi, without implementing a specific
#  communication protocol. See the below classes for different communication protocols.
class BaseArduinoInterface:
    def __init__(self):
        self._ptr = None
    
    ## Create devices (as needed) and start processing sensor data. After this is called devices can 
    #  no longer be added to this arduino using BaseArduinoInterface::addDevice.
    def begin(self):
        bridge.arpirobot.BaseArduinoInterface_begin(self._ptr)
    
    ## Add a device to this arduino interface instance. 
    #  You cannot add devices after BaseArduinoInterface::begin is called
    #  @param device The ArduinoDevice instance to add
    def add_device(self, device: ArduinoDevice):
        bridge.arpirobot.BaseArduinoInterface_addDevice(self._ptr, device._ptr)
    
    ## Returns true when the arduino is ready to process sensor data (after BaseArduinoInterface::begin 
    #  is successful)
    def is_ready(self) -> bool:
        return bridge.arpirobot.BaseArduinoInterface_isReady(self._ptr)


## Arduino interface implementation using UART to communicate with the arduino.
class ArduinoUartInterface(BaseArduinoInterface):
    ## @param tty The UART port for the arduino (/dev/tty...)
    #  @param baud The baud rate for UART communication
    def __init__(self, tty: str, baud: int):
        super().__init__()
        self._ptr = bridge.arpirobot.ArduinoUartInterface_create(tty.encode(), baud)
    
    def __del__(self):
        bridge.arpirobot.ArduinoUartInterface_destroy(self._ptr)
    
