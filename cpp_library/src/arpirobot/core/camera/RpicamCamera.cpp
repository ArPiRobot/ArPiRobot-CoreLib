
#include <arpirobot/core/camera/RpicamCamera.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <sstream>

using namespace arpirobot;


RpicamCamera::RpicamCamera(std::string id) : BaseCamera(id){

}

std::string RpicamCamera::getBackend(){
    return "rpicam";
}

std::string RpicamCamera::getDeviceName(){
    return "RpicamCamera(" + id + ")";
}

std::string RpicamCamera::getCapturePipeline(){
    return "filesrc location=" + rpicamFifo + " ! queue ! video/x-raw,format=I420" + 
            ",width=" + capWidth + ",height=" + capHeight + ",framerate=" + capFramerate +
            " ! rawvideoparse use-sink-caps=true";
}

bool RpicamCamera::doStartStreamH264(unsigned int port, unsigned int bitrate, 
        std::string profile, std::string level){
    // We always read frames in raw mode from rpicam-vid. Why?
    // 1. If we read jpeg / h264 we'd just end up decoding for appsink. The pi camera doesn't 
    //    natively support jpeg / h264. It just uses the HW encoders. So that's just a waste
    // 2. v4l2 encoders require full caps for a video read from filesrc. This requires guessing
    //    as to the colorimetry (color space) that rpicam-vid ended up using for jpeg (and maybe h264?)
    //    In raw mode, all caps are known and explicitly controlled via rpicam-vid's command line
    // Essentially, RpicamCamera just uses rpicam-vid as an input source instead of libcamerasrc
    // because it is often more stable and allows passing custom controls to the camera via
    // rpicam-vid's command line options (libcamerasrc currently doesn't support setting controls)
    if(capFormat != "raw"){
        Logger::logWarningFrom(getDeviceName(), "Only raw capture format is supported, but " + capFormat + " was specified.");
        capFormat = "raw"; // Must set this correctly or BaseCamera::makeStandardPipeline will do the wrong thing
    }
    rpicamFifo = "/tmp/rpicamvid_" + std::to_string(port);
    rpicamCommand = std::string("rpicam-vid --timeout=0 --verbose=0 --codec=yuv420") + 
            " --camera=" + id + 
            " --width=" + capWidth + 
            " --height=" + capHeight +
            " --framerate=" + framerateToDec() + 
            " --output=" + rpicamFifo;

    return BaseCamera::doStartStreamH264(port, bitrate, profile, level);
}

bool RpicamCamera::doStartStreamJpeg(unsigned int port, unsigned int quality){
    // We always read frames in raw mode from rpicam-vid. Why?
    // 1. If we read jpeg / h264 we'd just end up decoding for appsink. The pi camera doesn't 
    //    natively support jpeg / h264. It just uses the HW encoders. So that's just a waste
    // 2. v4l2 encoders require full caps for a video read from filesrc. This requires guessing
    //    as to the colorimetry (color space) that rpicam-vid ended up using for jpeg (and maybe h264?)
    //    In raw mode, all caps are known and explicitly controlled via rpicam-vid's command line
    // Essentially, RpicamCamera just uses rpicam-vid as an input source instead of libcamerasrc
    // because it is often more stable and allows passing custom controls to the camera via
    // rpicam-vid's command line options (libcamerasrc currently doesn't support setting controls)
    if(capFormat != "raw"){
        Logger::logWarningFrom(getDeviceName(), "Only raw capture format is supported, but " + capFormat + " was specified.");
        capFormat = "raw"; // Must set this correctly or BaseCamera::makeStandardPipeline will do the wrong thing
    }
    rpicamFifo = "/tmp/rpicamvid_" + std::to_string(port);
    rpicamCommand = std::string("rpicam-vid --timeout=0 --verbose=0 --codec=yuv420") + 
            " --camera=" + id + 
            " --width=" + capWidth + 
            " --height=" + capHeight +
            " --framerate=" + framerateToDec() + 
            " --output=" + rpicamFifo;
    
    return BaseCamera::doStartStreamJpeg(port, quality);
}

bool RpicamCamera::doStartStream(unsigned int port, std::string pipeline){
    if(access(rpicamFifo.c_str(), F_OK) == 0){
        remove(rpicamFifo.c_str());
    }
    if(mkfifo(rpicamFifo.c_str(), 0666) != 0){
        Logger::logErrorFrom(getDeviceName(), "Failed to create fifo for rpicam-vid.");
        return false;
    }
    chmod(rpicamFifo.c_str(), 0666);
    Logger::logDebugFrom(getDeviceName(), "rpicam-vid command: " + rpicamCommand);
    rpicamProc = popen(rpicamCommand.c_str(), "r");
    if(rpicamProc == NULL){
        Logger::logErrorFrom("CameraManager", "Failed to start rpicam-vid process.");
        remove(rpicamFifo.c_str());
        return false;
    }
    bool ret = BaseCamera::doStartStream(port, pipeline);
    if(!ret){
        pclose(rpicamProc);
        remove(rpicamFifo.c_str());
        return false;
    }
    return true;
}

void RpicamCamera::doStopStream(){
    BaseCamera::doStopStream();
    pclose(rpicamProc);
    remove(rpicamFifo.c_str());
}

std::string RpicamCamera::framerateToDec(){
    int slashPos = capFramerate.find("/");
    if(slashPos == std::string::npos)
        return "30.0"; // Safe default. String is unparsable
    std::string numStr = capFramerate.substr(0, slashPos);
    std::string decStr = capFramerate.substr(slashPos + 1, capFramerate.length() - slashPos - 1);
    if(decStr == "1")
        return numStr; // No need to divide if denominator is 1
    double num = std::stod(numStr);
    double dec = std::stod(decStr);
    double fr = num / dec;
    std::stringstream s;
    s << std::fixed << std::setprecision(2) << fr;
    return s.str();
}
