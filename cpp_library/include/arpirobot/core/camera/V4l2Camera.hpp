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
     * \class V4l2Camera V4l2Camera.hpp arpirobot/core/camera/V4l2Camera.hpp
     * 
     * Camera object using the v4l2 backend. Can be used for USB webcams. Generally will not work
     * with camera modules (eg RPi Camera modules)
     * 
     * When using this camera backend, extra options will be v4l2 controls
     */
    class V4l2Camera : public BaseCamera{
    public:
        /**
         * Create a new camera object using libcamera backend
         * @param id Path to a camera (as shown by v4l2-ctl --list-devices command).
         *           This must be a /dev/video[num] path not just a number
         */
        V4l2Camera(std::string id);

        std::string getBackend() override;

    protected:
        std::string getDeviceName() override;
        std::string getCapturePipeline() override;
    };
}