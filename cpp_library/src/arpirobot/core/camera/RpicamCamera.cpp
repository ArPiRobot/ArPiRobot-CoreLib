
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
    return "filesrc location=" + fifoPath + " ! queue ! video/x-raw,format=I420" + 
            ",width=" + capWidth + ",height=" + capHeight + ",framerate=" + capFramerate +
            " ! rawvideoparse use-sink-caps=true";
}

bool RpicamCamera::doStartStreamH264(unsigned int port, unsigned int bitrate, 
        std::string profile, std::string level){
    if(!setupFifo(port))
        return false;

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
    std::string cmd = std::string("rpicam-vid --timeout=0 --verbose=0 --codec=yuv420") + 
            " --camera=" + id + 
            " --width=" + capWidth + 
            " --height=" + capHeight +
            " --framerate=" + framerateToDec() + 
            " --output=" + fifoPath;
    
    if(!setupProc(cmd)){
        teardownProcAndFifo();
        return false;
    }

    bool ret = BaseCamera::doStartStreamH264(port, bitrate, profile, level);
    if(!ret){
        teardownProcAndFifo();
        return false;
    }
    return true;
}

bool RpicamCamera::doStartStreamJpeg(unsigned int port, unsigned int quality){
    if(!setupFifo(port))
        return false;

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
    std::string cmd = std::string("rpicam-vid --timeout=0 --verbose=0 --codec=yuv420") + 
            " --camera=" + id + 
            " --width=" + capWidth + 
            " --height=" + capHeight +
            " --framerate=" + framerateToDec() + 
            " --output=" + fifoPath;
    
    if(!setupProc(cmd)){
        teardownProcAndFifo();
        return false;
    }

    bool ret = BaseCamera::doStartStreamJpeg(port, quality);
    if(!ret){
        teardownProcAndFifo();
        return false;
    }
    return true;
}

void RpicamCamera::doStopStream(){
    BaseCamera::doStopStream();
    teardownProcAndFifo();
}

bool RpicamCamera::setupFifo(unsigned int port){
    fifoPath = "/tmp/stream" + std::to_string(port);
    if(access(fifoPath.c_str(), F_OK) == 0){
        remove(fifoPath.c_str());
    }
    if(mkfifo(fifoPath.c_str(), 0666) != 0){
        Logger::logErrorFrom(getDeviceName(), "Failed to create fifo for stream.");
        return false;
    }
    Logger::logDebugFrom(getDeviceName(), "FiFo path for stream: " + fifoPath);
    chmod(fifoPath.c_str(), 0666);
    return true;
}

bool RpicamCamera::setupProc(std::string cmd){
    Logger::logDebugFrom(getDeviceName(), "Starting process " + cmd);
    proc = popen(cmd.c_str(), "r");
    if(proc == NULL){
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Failed to spawn process.");
        return false;
    }
    return true;
}

void RpicamCamera::teardownProcAndFifo(){
    if(proc != NULL)
        pclose(proc);
    if(fifoPath != "")
        remove(fifoPath.c_str());
    proc = NULL;
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
