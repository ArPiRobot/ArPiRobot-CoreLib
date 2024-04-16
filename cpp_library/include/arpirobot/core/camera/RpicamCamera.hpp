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
     * \class RpicamCamera RpicamCamera.hpp arpirobot/core/camera/RpicamCamera.hpp
     * 
     * Camera object using the rpicam-vid program. Can be used for Raspberry Pi camera modules.
     * It may or may not work with other camera modules. It will not work with USB cameras.
     * 
     * This is a more reliable and customizable way to use RPi camera modules than LibcameraCamera.
     * RpicamCamera allows setting extra controls.
     * 
     * When using this camera backend, extra options will be command line arguments to rpicam-vid
     * 
     * Important notes about rpicam camera
     * - input format is not supported (raw will always be used)
     */
    class RpicamCamera : public BaseCamera{
    public:
        /**
         * Create a new camera object using libcamera backend
         * @param id ID number of the camera module (as shown by rpicam-vid --list-cameras command).
         *           This is the NUMBER not the path.
         */
        RpicamCamera(std::string id);

        std::string getBackend() override;

    protected:
        std::string getDeviceName() override;
        std::string getCapturePipeline() override;

        bool doStartStreamH264(std::string key, unsigned int bitrate, 
                std::string profile, std::string level) override;
        bool doStartStreamJpeg(std::string key, unsigned int quality) override;
        bool doStartStream(std::string key, std::string pipeline) override;
        void doStopStream() override;

    private:
        std::string framerateToDec();

        // Process and pipe name vars
        std::string rpicamFifo = "";
        std::string rpicamCommand = "";
        std::unique_ptr<boost::process::child> rpicamProc;
    };
}