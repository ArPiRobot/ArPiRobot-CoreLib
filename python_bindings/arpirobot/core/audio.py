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


from typing import List
import arpirobot.bridge as bridge
import ctypes


class AudioDeviceInfo:
    id: int = -1
    name: str = ""
    is_default: bool = False


class AudioManager:
    @staticmethod
    def get_playback_devices() -> List[AudioDeviceInfo]:
        count: int = bridge.arpirobot.AudioManager_getPlaybackDevicesCount()
        id = ctypes.c_uint32(0)
        name = ctypes.c_char_p(b"")
        is_default = ctypes.c_bool(False)
        lst = []
        for i in range(count):
            bridge.arpirobot.AudioManager_getPlaybackDevice(i, ctypes.byref(id), ctypes.byref(name), ctypes.byref(is_default))
            info = AudioDeviceInfo()
            info.id = id.value
            info.name = name.value
            info.is_default = is_default.value
            lst.append(info)
            bridge.arpirobot.freeString(name)
        return lst

    @staticmethod
    def get_capture_devices() -> List[AudioDeviceInfo]:
        count: int = bridge.arpirobot.AudioManager_getCaptureDevicesCount()
        id = ctypes.c_uint32(0)
        name = ctypes.c_char_p(b"")
        is_default = ctypes.c_bool(False)
        lst = []
        for i in range(count):
            bridge.arpirobot.AudioManager_getCaptureDevice(i, ctypes.byref(id), ctypes.byref(name), ctypes.byref(is_default))
            info = AudioDeviceInfo()
            info.id = id.value
            info.name = name.value.decode()
            info.is_default = is_default.value
            lst.append(info)
            bridge.arpirobot.freeString(name)
        return lst

    @staticmethod
    def play_sound(filename: str, info: AudioDeviceInfo = None) -> bool:
        if info is None:
            return bridge.arpirobot.AudioManager_playSound(filename)
        else:
            return bridge.arpirobot.AudioManager_playSoundWithDevice(filename, info.id, info.name.encode(), info.is_default)
