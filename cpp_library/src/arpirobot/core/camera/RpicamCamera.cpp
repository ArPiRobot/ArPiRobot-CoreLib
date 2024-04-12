
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
    return ""; // Not used because doStartStream... overriden to not use makeStandardPipeline
}

bool RpicamCamera::doStartStreamH264(unsigned int port, unsigned int bitrate, 
        std::string profile, std::string level){
    if(!setupFifo(port))
        return false;
    std::string cmd = std::string("rpicam-vid -t 0 --codec h264 --verbose=0") + 
            " --camera=" + id + 
            " --width=" + capWidth + 
            " --height=" + capHeight +
            " --framerate=" + framerateToDec() + 
            " --bitrate=" + std::to_string(bitrate) +
            " --profile=" + profile + 
            " --output=" + fifoPath;
    if(level != "")
        cmd += " --level=" + level;
    if(!setupProc(cmd))
        return false;

    std::string convert = getVideoConvertElement();
    std::string decPl = getH264DecodeElement();

    std::string pl = "filesrc location=" + fifoPath + " ! ";
    pl += capFormat + "image/jpeg,width=" + capWidth + ",height=" + capHeight + ",framerate=" + capFramerate;
    // TODO: Other options required for h264 capsfilter
    pl += " ! tee name=enc ";
    pl += "enc. ! queue ! " + decPl + " ! " + convert + " ! appsink drop=true max-buffers=1 ";
    pl += "enc. ! queue ! " + getOutputPipeline(port);

    // Actually start things
    if(!doStartStream(pl)){
        teardownProcAndFifo();
        return false;
    }
    return true;
}

bool RpicamCamera::doStartStreamJpeg(unsigned int port, unsigned int quality){
    if(!setupFifo(port))
        return false;
    std::string cmd = std::string("rpicam-vid -t 0 --codec mjpeg --verbose=0") + 
            " --camera=" + id + 
            " --width=" + capWidth + 
            " --height=" + capHeight +
            " --framerate=" + framerateToDec() + 
            " --quality=" + std::to_string(quality) + 
            " --output=" + fifoPath;
    if(!setupProc(cmd))
        return false;

    std::string convert = getVideoConvertElement();
    std::string decPl = getJpegDecodeElement();
    
    std::string pl = "filesrc location=" + fifoPath + " ! ";
    pl += capFormat + "imag/jpeg,width=" + capWidth + ",height=" + capHeight + ",framerate=" + capFramerate;
    // TODO: Other options required for jpeg capsfilter
    pl += " ! tee name=enc ";
    pl += "enc. ! queue ! " + decPl + " ! " + convert + " ! appsink drop=true max-buffers=1 ";
    pl += "enc. ! queue ! " + getOutputPipeline(port);

    // Actually start things
    if(!doStartStream(pl)){
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
    if(mkfifo(fifoPath.c_str(), 0666) != 0){
        Logger::logErrorFrom(getDeviceName(), "Failed to create fifo for stream.");
        return false;
    }
    chmod(fifoPath.c_str(), 0666);
    return true;
}

bool RpicamCamera::setupProc(std::string cmd){
    Logger::logInfoFrom(getDeviceName(), "Starting process " + cmd);
    proc = popen(cmd.c_str(), "r");
    if(proc == NULL){
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Failed to spawn process.");
        return false;
    }
    return true;
}

void RpicamCamera::teardownProcAndFifo(){
    pclose(proc);
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
