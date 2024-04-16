/*
 * Copyright 2024 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

#pragma once

#include <arpirobot/core/camera/BaseCamera.hpp>

namespace arpirobot{
    /**
     * \class LibcameraCamera LibcameraCamera.hpp arpirobot/core/camera/LibcameraCamera.hpp
     * 
     * Camera object using the libcamera backend. Libcamera can be used for USB webcams or 
     * for camera modules (such as RPi camera modules).
     * 
     * This camera implementation uses libcamera's gstreamer plugin, which currently has some limitations.
     * As such, using V4l2Camera or RpicamCamera is recommended over LibcameraCamera at this time unless
     * you have no other option (eg camera modules that don't work with RpicamCamera)
     * 
     * Limitations:
     * - Framerate settings are ignored
     * - Extra options are not able to be specified
     * 
     * Additionally, libcamera's gstreamer plugin seems to be a little unstable causing crashes or 
     * stalls in scenarios that would work fine when using v4l2 with the same camera.
     */
    class LibcameraCamera : public BaseCamera{
    public:
        /**
         * Create a new camera object using libcamera backend
         * @param id Path to a camera (as shown by cam -l command). IDs (numbers) will not work.
         */
        LibcameraCamera(std::string id);

        std::string getBackend() override;

    protected:
        std::string getDeviceName() override;
        std::string getCapturePipeline() override;
    };
}