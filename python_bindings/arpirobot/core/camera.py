"""
Copyright 20214Marcus Behel

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


## Common base class for various camera types
#  Provides functionality to run camera streams and receive frames from
#  cameras in OpenCV compatible formats
class BaseCamera:
    def __init__(self):
        self._ptr = None
    
    ## Get backend-specific device ID for this camera
    def get_id(self) -> str:
        res = ctypes.c_char_p(bridge.arpirobot.BaseCamera_getId(self._ptr))
        retval = res.value.decode()
        bridge.arpirobot.freeString(res)
        return retval
    
    ## Configure capture settings for this camera
    #  @param mode String describing mode information in the following form
    #              [format]:[width]x[height]@[framerate]
    #              format and framerate can be omitted. They will default to
    #              raw and 30 respectively if they are omitted.
    #              Framerate can be specified as fraction or whole integer
    #              Format is optional, defaulting to raw if not specified
    #              Framerate is optional, defaulting to 30/1 if not specified
    #              Omit the colon if not specifying format
    #              Omit the at symbol if not specifying framerate
    #  @return true if the provided mode string is successfully applied.
    #          false otherwise. This may mean the mode is invalid
    #          or it may mean that a stream is currently running. The capture
    #          mode cannot be changed while a stream is running
    def set_capture_mode(self, mode: str) -> bool:
        return bridge.arpirobot.BaseCamera_setCaptureMode(self._ptr, mode.encode())

    ## Configure hardware acceleration for streaming with this camera
    #  @param hwencode Use hardware encoders if available
    #  @param hwdecode Use hardware decoders if available
    #  @param hwconvert Use hardware converters if available
    #  @return true on success. Only returns false if a stream is running
    def set_hw_accel(self, hwencode: bool, hwdecode: bool, hwconvert: bool) -> bool:
        return bridge.arpirobot.BaseCamera_setHwAccel(self._ptr, hwencode, hwdecode, hwconvert)

    ## Set an extra option (option names and values are backend and camera specific)
    #  @param name Name of the option to set
    #  @param value Value to set for the option
    #  @return true on success, false on error (which occurs if stream is currently running)
    def set_extra_option(self, name: str, value: str) -> bool:
        return bridge.arpirobot.BaseCamera_setExtraOption(self._ptr, name.encode(), value.encode())

    ## Unset all extra options that were previous set
    #  @return true on success, false on error (which occurs if stream is currently running)
    def clear_extra_options(self) -> bool:
        return bridge.arpirobot.BaseCamera_clearExtraOptions(self._ptr)

    # TODO: set_frame_callback

    ## Start a H264 stream. Note that only one stream may be running on a
    #  camera at a time.
    #  @param key Name of the stream (must be unique)
    #  @param bitrate Target bitrate in kbps (default 2048)
    #  @param profile H.264 profile to use (default baseline)
    #  @param level H.264 level to use (default empty string = auto)
    #  @return true on success, else false (could be pipeline failure, or a stream may already be running)
    def start_stream_h264(self, key: str, bitrate: int = 2048, profile: str = "baseline", level: str = "") -> bool:
        return bridge.arpirobot.BaseCamera_startStreamH264(self._ptr, key.encode(), bitrate, profile.encode(), level.encode())

    ## Start a (M)JPEG stream. Note that only one stream may be running on a
    #  camera at a time.
    #  @param key Name of the stream (must be unique)
    #  @param quality Quality of JPEG images (0-100, default 80). Lower quality requires less bandwidth
    #  @return true on success, else false (could be pipeline failure, or a stream may already be running)
    def start_stream_jpeg(self, key: str, quality: int = 80) -> bool:
        return bridge.arpirobot.BaseCamera_startStreamJpeg(self._ptr, key.encode(), quality)
    
    ## Stop the running stream
    def stop_stream(self):
        bridge.arpirobot.BaseCamera_stopStream(self._ptr)
    
    ## Get name of this camera's backend
    def get_backend(self) -> str:
        res = ctypes.c_char_p(bridge.arpirobot.BaseCamera_getBackend(self._ptr))
        retval = res.value.decode()
        bridge.arpirobot.freeString(res)
        return retval

## Camera object using the v4l2 backend. Can be used for USB webcams. Generally will not work
#  with camera modules (eg RPi Camera modules)
#  
#  When using this camera backend, extra options will be v4l2 controls
class V4l2Camera(BaseCamera):
    ## Create a new camera object using libcamera backend
    #  @param id Path to a camera (as shown by v4l2-ctl --list-devices command).
    #            This must be a /dev/video[num] path not just a number
    def __init__(self, id: str):
        super().__init__()
        self._ptr = bridge.arpirobot.V4l2Camera_create(id.encode())
    
    def __del__(self):
        bridge.arpirobot.V4l2Camera_destroy(self._ptr)

## Camera object using the libcamera backend. Libcamera can be used for USB webcams or 
#  for camera modules (such as RPi camera modules).
#  
#  This camera implementation uses libcamera's gstreamer plugin, which currently has some limitations.
#  As such, using V4l2Camera or RpicamCamera is recommended over LibcameraCamera at this time unless
#  you have no other option (eg camera modules that don't work with RpicamCamera)
#  
#  Limitations:
#  - Framerate settings are ignored
#  - Extra options are not able to be specified
#  
#  Additionally, libcamera's gstreamer plugin seems to be a little unstable causing crashes or 
#  stalls in scenarios that would work fine when using v4l2 with the same camera.
class LibcameraCamera(BaseCamera):
    ## Create a new camera object using libcamera backend
    #  @param id Path to a camera (as shown by cam -l command). IDs (numbers) will not work.
    def __init__(self, id: str):
        super().__init__()
        self._ptr = bridge.arpirobot.LibcameraCamera_create(id.encode())
    
    def __del__(self):
        bridge.arpirobot.LibcameraCamera_destroy(self._ptr)

## Camera object using the rpicam-vid program. Can be used for Raspberry Pi camera modules.
#  It may or may not work with other camera modules. It will not work with USB cameras.
#  
#  This is a more reliable and customizable way to use RPi camera modules than LibcameraCamera.
#  RpicamCamera allows setting extra controls.
#  
#  When using this camera backend, extra options will be command line arguments to rpicam-vid
#  
#  Important notes about rpicam camera
#  - input format is not supported (raw will always be used)
class RpicamCamera(BaseCamera):
    ## Create a new camera object using libcamera backend
    #  @param id ID number of the camera module (as shown by rpicam-vid --list-cameras command).
    #            This is the NUMBER not the path.
    def __init__(self, id: str):
        super().__init__()
        self._ptr = bridge.arpirobot.RpicamCamera_create(id.encode())
    
    def __del__(self):
        bridge.arpirobot.RpicamCamera_destroy(self._ptr)