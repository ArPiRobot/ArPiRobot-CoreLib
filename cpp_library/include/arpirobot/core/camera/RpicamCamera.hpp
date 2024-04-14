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

#include <arpirobot/core/camera/BaseCamera.hpp>
#include <boost/process.hpp>

namespace arpirobot{
    /**
     * Uses rpicam-vid instead of gstreamer to capture video
     * Allows control over rpicam-vid options using extra parameters (TODO: When implemented)
     * Somewhat more stable than libcamerasrc (used by libcamera camera)
     * 
     * Important notes about rpicam camera
     * - input format is not supported (raw will always be used).
     */
    class RpicamCamera : public BaseCamera{
    public:

        // ID is camera number as shown by rpicam-vid --list-cameras
        RpicamCamera(std::string id);

        std::string getBackend() override;

    protected:
        std::string getDeviceName() override;
        std::string getCapturePipeline() override;

        bool doStartStreamH264(std::string key, unsigned int bitrate, 
                std::string profile, std::string level) override;
        bool doStartStreamJpeg(std::string key, unsigned int quality) override;

        bool extraSetup(std::string key, std::string pipeline) override;
        void extraTeardown(std::string key, std::string pipeline) override;

    private:
        std::string framerateToDec();

        // Process and pipe name vars
        std::string rpicamFifo = "";
        std::string rpicamCommand = "";
        std::unique_ptr<boost::process::child> rpicamProc;
    };
}