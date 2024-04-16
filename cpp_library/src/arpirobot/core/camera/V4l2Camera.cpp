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

#include <arpirobot/core/camera/V4l2Camera.hpp>

using namespace arpirobot;

V4l2Camera::V4l2Camera(std::string id) : BaseCamera(id){

}

std::string V4l2Camera::getBackend(){
    return "v4l2";
}

std::string V4l2Camera::getDeviceName(){
    return "V4l2Camera(" + id + ")";
}

std::string V4l2Camera::getCapturePipeline(){
    std::string pl = "v4l2src io-mode=dmabuf device=" + id;

    std::string optsStr = " extra-controls=controls";
    if(extraOpts.size() != 0){
        for(auto opt : extraOpts){
            optsStr += "," + opt.first + "=" + opt.second;
        }
        pl += optsStr;
    }
    pl += " ! ";
    if(capFormat == "raw"){
        pl += "video/x-raw";
    }else if(capFormat == "jpeg"){
        pl += "image/jpeg";
    }else if(capFormat == "h264"){
        pl += "video/x-h264";
    }else{
        return "";
    }
    pl += ",width=" + capWidth + ",height=" + capHeight + ",framerate=" + capFramerate;
    return pl;
}