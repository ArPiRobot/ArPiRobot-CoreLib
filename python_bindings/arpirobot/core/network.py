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


## Main voltage monitor interface
#  Any sensor (arduino or pi device) can implement this interface to be able to be used as a main vmon
class MainVmon:
    # Just a common base class for valid main vmon devices
    def __init__(self):
        self._ptr = None
    
    def make_main_vmon(self):
        bridge.arpirobot.MainVMon_makeMainVmon(self._ptr)


## Helper class used to manage network table key/value pairs
class NetworkTable:

    ## Sets a given key/value pair. If the key does not exist it will be created. Else the value will be updated.
    #  @param key The key for the pair
    #  @param value The value for the pair
    @staticmethod
    def set(key: str, value: str):
        bridge.arpirobot.NetworkTable_set(key.encode(), value.encode())
    
    ## Get the value for a key/value pair
    #  @param key The key to get the associated value with
    #  @returns The value associated with the given key. If the key does not exist an empty string is returned.
    @staticmethod
    def get(key: str) -> str:
        res = ctypes.c_char_p(bridge.arpirobot.NetworkTable_get(key.encode()))
        retval = res.value.decode()
        bridge.arpirobot.freeString(res)
        return retval
    
    ## Check if a key has a value
    #  @param key The key to check for a value associated with
    #  @returns true if a value exists for the given key, else false
    @staticmethod
    def has(key: str) -> bool:
        return bridge.arpirobot.NetworkTable_has(key.encode())
    
    ## Check if a key's value has changed since last call to get (only due to drive station)
    #  @param key Key to check
    #  @return True If the key has been changed by the drive station since get was last called
    #  @return False If the key has not been changed
    @staticmethod
    def changed(key: str) -> bool:
        return bridge.arpirobot.NetworkTable_changed(key.encode())