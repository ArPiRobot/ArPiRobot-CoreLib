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

#include <arpirobot/core/camera/LibcameraCamera.hpp>

#include <string>
#include <regex>

using namespace arpirobot;

LibcameraCamera::LibcameraCamera(std::string id) : BaseCamera(id){

}

std::string LibcameraCamera::getBackend(){
    return "libcamera";
}

std::string LibcameraCamera::getDeviceName(){
    return "LibcameraCamera(" + id + ")";
}

std::string LibcameraCamera::getCapturePipeline(){
    std::string idEscape = id;
    idEscape = std::regex_replace(idEscape, std::regex("\\\\"), "\\\\");
    std::string pl = "libcamerasrc camera-name=\"" + idEscape + "\" ! ";
    if(capFormat == "raw"){
        pl += "video/x-raw";
    }else if(capFormat == "jpeg"){
        pl += "image/jpeg";
    }else if(capFormat == "h264"){
        pl += "video/x-h264";
    }else{
        return "";
    }
    pl += ",width=" + capWidth + ",height=" + capHeight;
    // framerate setting not used - not supported by libcamerasrc
    return pl;
}