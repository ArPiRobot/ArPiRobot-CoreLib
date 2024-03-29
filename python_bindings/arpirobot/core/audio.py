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
    TYPE_PLAYBACK = 0
    TYPE_CAPTURE = 1

    def __init__(self):
        self.id: int = -1
        self.name: str = ""
        self.is_default: bool = False
        self.type: int = 0


class AudioManager:
    @staticmethod
    def get_playback_devices() -> List[AudioDeviceInfo]:
        count: int = bridge.arpirobot.AudioManager_getPlaybackDevicesCount()
        id = ctypes.c_uint32(0)
        name = ctypes.c_char_p(b"")
        is_default = ctypes.c_bool(False)
        type = ctypes.c_uint8(0)
        lst = []
        for i in range(count):
            bridge.arpirobot.AudioManager_getPlaybackDevice(i, ctypes.byref(id), ctypes.byref(name), ctypes.byref(is_default), ctypes.byref(type))
            info = AudioDeviceInfo()
            info.id = id.value
            info.name = name.value.decode()
            info.is_default = is_default.value
            info.type = type.value
            lst.append(info)
            bridge.arpirobot.freeString(name)
        return lst

    @staticmethod
    def play_sound(filename: str, info: AudioDeviceInfo = None) -> int:
        if info is None:
            return bridge.arpirobot.AudioManager_playSound(filename.encode())
        else:
            return bridge.arpirobot.AudioManager_playSoundWithDevice(filename.encode(), info.id, info.name.encode(), info.is_default, info.type)

    @staticmethod
    def stop_job(job_id: int):
        bridge.arpirobot.AudioManager_stopJob(job_id)