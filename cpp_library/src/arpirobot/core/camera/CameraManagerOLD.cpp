
#include <arpirobot/core/camera/CameraManager.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>

#include <gst/gst.h>
#include <glib.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <algorithm>

using namespace arpirobot;

bool CameraManager::initialized = false;
std::vector<Camera> CameraManager::cameras;

std::unordered_map<unsigned int, CameraManager::Stream> CameraManager::streams;

void CameraManager::init(){
    if(initialized){
        Logger::logWarningFrom("CameraManager", "Camera manager was already initialized.");
        return;
    }
    gst_init(NULL, NULL);
    initV4l2();
    initLibcamera();
    initialized = true; 
    Logger::logInfoFrom("CameraManager", "Discovered " + std::to_string(cameras.size()) + " cameras.");
}

std::vector<Camera> CameraManager::getCameras(){
    return cameras;
}

bool CameraManager::startStreamH264(unsigned int streamPort, 
                Camera cam, 
                std::string mode, 
                std::function<void(cv::Mat)> *frameCallback,
                H264Settings h264settings,
                bool hwencode, bool hwdecode, bool hwconv){    

    // Split mode string into components
    int colonPos = mode.find(":", 0);
    int xPos = mode.find("x", colonPos);
    int atPos = mode.find("@", xPos);
    if(colonPos == std::string::npos || xPos == std::string::npos){
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Invalid mode string.");
        return false;
    }
    if(atPos == std::string::npos)
        atPos = mode.length();
    std::string format = mode.substr(0, colonPos);
    std::string width = mode.substr(colonPos + 1, xPos - colonPos - 1);
    std::string height = mode.substr(xPos + 1, atPos - xPos - 1);
    std::string framerate = "";
    if(atPos != mode.length()){
        framerate = mode.substr(atPos + 1, mode.length() - atPos - 1);
        int slashPos = framerate.find("/");
        if(slashPos == std::string::npos){
            framerate += "/1";
        }
    }

    // Element to open camera
    std::string input = getSource(cam);

    // Caps for input
    std::string inputCaps;
    if(format == "raw"){
        inputCaps += "video/x-raw";
        if(cam.api == "rpicam"){}
            // Must specify all caps if using rpicam-vid with raw frames
            inputCaps += ",format=I420";
    }else if(format == "jpeg"){
        inputCaps += "image/jpeg";
    }else if(format == "h264"){
        inputCaps += "video/x-h264";
    }else{
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Unknown input format in mode.");
        return false;
    }
    inputCaps += ",width=" + width + ",height=" + height;
    if(framerate != ""){
        inputCaps += ",framerate=" + framerate;
    }else if(cam.api == "rpicam"){
        // Must specify all caps if using rpicam-vid with raw frames
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Framerate must be specified if using rpicam with raw frames");
        return false;
    }

    // Decoder (as needed)
    std::string decoder;
    if(format == "jpeg"){
        decoder = getJpegDecodeElement(hwdecode);
    }else if(format == "h264"){
        decoder = getH264DecodeElement(hwdecode);
    }

    // Encoder (as needed)
    std::string encoder;
    if(format != "h264"){
        encoder = getH264EncodeElement(hwencode, h264settings);
    }

    // Convert element
    std::string convert = getVideoConvertElement(hwconv);

    // Stream output
    // buffers-soft-max=2 recover-policy=latest on tcpserversink do two things to help with network drops / latency
    //   1. If a client looses frames, they are not re-sent. Instead the latest frame is sent
    //   2. If packets are dropped, the number of buffers used is limited to avoid excessive memory usage
    std::string streamOut = std::string("h264parse config-interval=-1 ! video/x-h264,stream-format=byte-stream,alignment=au ! ") +
            "tcpserversink buffers-soft-max=2 recover-policy=latest host=0.0.0.0 port=" + std::to_string(streamPort);

    // Construct pipeline
    // Note: drop=true max-buffers=1 properties on appsink ensure it doesn't queue frames if application reading
    // the frames is too slow. Queueing frames could stall pipeline or use lots of memory as well as adding latency
    // for the application to receive the frames
    std::string pipeline = "";
    if(format == "raw"){
        // Scenario 1: Raw input (only need encode)
        // Note: need videoconvert on input side because converters down the line may be hw accelerated
        // and hw accelerated converters usually have colorometry limitations that SW videoconvert does not
        pipeline += input + " ! " + inputCaps + " ! videoconvert ! tee name=raw ";
        pipeline += "raw. ! queue ! " + convert + " ! appsink drop=true max-buffers=1 ";
        pipeline += "raw. ! queue ! " + convert + " ! " + encoder + " ! " + streamOut;
    }else if(format == "h264"){
        // Scenario 2: Input format matches stream format (only need decode)
        pipeline += input + " ! " + inputCaps + " ! tee name=in ";
        pipeline += "in. ! queue ! " + decoder + " ! " + convert + " ! appsink drop=true max-buffers=1 ";
        pipeline += "in. ! queue ! " + streamOut;
    }else{
        // Scenario 3: Input format not raw, and does not match stream format (need decode then encode)
        pipeline += input + " ! " + inputCaps + " ! " + decoder + " ! tee name=raw ";
        pipeline += "raw. ! queue ! " + convert + " ! appsink drop=true max-buffers=1 ";
        pipeline += "raw. ! queue ! " + convert + " ! " + encoder + " ! " + streamOut;
    }

    std::string fifo = getFifo(cam);
    std::string procCmd = getProcCmd(fifo, cam, format, width, height, framerate);
    return startStreamFromPipeline(streamPort, pipeline, frameCallback, fifo, procCmd);
}

bool CameraManager::startStreamJpeg(unsigned int streamPort, 
                Camera cam, 
                std::string mode, 
                std::function<void(cv::Mat)> *frameCallback,
                JpegSettings jpegSettings,
                bool hwencode, bool hwdecode, bool hwconv){    

    // Split mode string into components
    int colonPos = mode.find(":", 0);
    int xPos = mode.find("x", colonPos);
    int atPos = mode.find("@", xPos);
    if(colonPos == std::string::npos || xPos == std::string::npos){
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Invalid mode string.");
        return false;
    }
    if(atPos == std::string::npos)
        atPos = mode.length();
    std::string format = mode.substr(0, colonPos);
    std::string width = mode.substr(colonPos + 1, xPos - colonPos - 1);
    std::string height = mode.substr(xPos + 1, atPos - xPos - 1);
    std::string framerate = "";
    if(atPos != mode.length()){
        framerate = mode.substr(atPos + 1, mode.length() - atPos - 1);
        int slashPos = framerate.find("/");
        if(slashPos == std::string::npos){
            framerate += "/1";
        }
    }

    // Element to open camera
    std::string input = getSource(cam);

    // Caps for input
    std::string inputCaps;
    if(format == "raw"){
        inputCaps += "video/x-raw";
        if(cam.api == "rpicam"){}
            // Must specify all caps if using rpicam-vid with raw frames
            inputCaps += ",format=I420";
    }else if(format == "jpeg"){
        inputCaps += "image/jpeg";
    }else if(format == "h264"){
        inputCaps += "video/x-h264";
    }else{
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Unknown input format in mode.");
        return false;
    }
    inputCaps += ",width=" + width + ",height=" + height;
    if(framerate != ""){
        inputCaps += ",framerate=" + framerate;
    }else if(cam.api == "rpicam"){
        // Must specify all caps if using rpicam-vid with raw frames
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Framerate must be specified if using rpicam with raw frames");
        return false;
    }

    // Decoder (as needed)
    std::string decoder;
    if(format == "jpeg"){
        decoder = getJpegDecodeElement(hwdecode);
    }else if(format == "h264"){
        decoder = getH264DecodeElement(hwdecode);
    }

    // Encoder (as needed)
    std::string encoder;
    if(format != "jpeg"){
        encoder = getJpegEncodeElement(hwencode, jpegSettings);
    }

    // Convert element
    std::string convert = getVideoConvertElement(hwconv);

    // Stream output
    // buffers-soft-max=2 recover-policy=latest on tcpserversink do two things to help with network drops / latency
    //   1. If a client looses frames, they are not re-sent. Instead the latest frame is sent
    //   2. If packets are dropped, the number of buffers used is limited to avoid excessive memory usage
    std::string streamOut = "tcpserversink buffers-soft-max=2 recover-policy=latest host=0.0.0.0 port=" + 
            std::to_string(streamPort);

    // Construct pipeline
    // Note: drop=true max-buffers=1 properties on appsink ensure it doesn't queue frames if application reading
    // the frames is too slow. Queueing frames could stall pipeline or use lots of memory as well as adding latency
    // for the application to receive the frames
    std::string pipeline = "";
    if(format == "raw"){
        // Scenario 1: Raw input (only need encode)
        // Note: need videoconvert on input side because converters down the line may be hw accelerated
        // and hw accelerated converters usually have colorometry limitations that SW videoconvert does not
        pipeline += input + " ! " + inputCaps + " ! videoconvert ! tee name=raw ";
        pipeline += "raw. ! queue ! " + convert + " ! appsink drop=true max-buffers=1 ";
        pipeline += "raw. ! queue ! " + convert + " ! " + encoder + " ! " + streamOut;
    }else if(format == "jpeg"){
        // Scenario 2: Input format matches stream format (only need decode)
        pipeline += input + " ! " + inputCaps + " ! tee name=in ";
        pipeline += "in. ! queue ! " + decoder + " ! " + convert + " ! appsink drop=true max-buffers=1 ";
        pipeline += "in. ! queue ! " + streamOut;
    }else{
        // Scenario 3: Input format not raw, and does not match stream format (need decode then encode)
        pipeline += input + " ! " + inputCaps + " ! " + decoder + " ! tee name=raw ";
        pipeline += "raw. ! queue ! " + convert + " ! appsink drop=true max-buffers=1 ";
        pipeline += "raw. ! queue ! " + convert + " ! " + encoder + " ! " + streamOut;
    }

    std::string fifo = getFifo(cam);
    std::string procCmd = getProcCmd(fifo, cam, format, width, height, framerate);
    return startStreamFromPipeline(streamPort, pipeline, frameCallback, fifo, procCmd);
}

bool CameraManager::startStreamFromPipeline(unsigned int streamPort,
                std::string pipeline,
                std::function<void(cv::Mat)> *frameCallback,
                std::string fifo,
                std::string procCmd){
    if(streams.find(streamPort) != streams.end()){
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Duplicate stream name.");
        return false;
    }

    Logger::logInfoFrom("CameraManager", "Starting stream " + std::to_string(streamPort));
    Logger::logDebugFrom("CameraManager", "Stream " + std::to_string(streamPort) + " pipeline: " + pipeline);

    streams[streamPort] = Stream();
    if(fifo != "" && procCmd != ""){
        Logger::logDebugFrom("CameraManager", "Fifo path: " + fifo);
        if(access(fifo.c_str(), F_OK) == 0){
            remove(fifo.c_str());
        }
        if(mkfifo(fifo.c_str(), 0666) != 0){
            streams.erase(streamPort);
            Logger::logErrorFrom("CameraManager", "Failed to create fifo for stream.");
            return false;
        }
        chmod(fifo.c_str(), 0666);
        Logger::logInfoFrom("CameraManager", "Starting process for stream " + procCmd);
        streams[streamPort].proc = popen(procCmd.c_str(), "r");
        if(streams[streamPort].proc == NULL){
            streams.erase(streamPort);
            Logger::logErrorFrom("CameraManager", "Failed to start stream. Failed to spawn process.");
            return false;
        }
    }
    streams[streamPort].cap = cv::VideoCapture();
    if(!streams[streamPort].cap.open(pipeline, cv::CAP_GSTREAMER)){
        streams.erase(streamPort);
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Failed to open capture pipeline.");
        return false;
    }
    streams[streamPort].run = true;
    streams[streamPort].thread = std::thread([streamPort, frameCallback](){
        cv::Mat frame;
        while(streams[streamPort].run){
            streams[streamPort].cap.read(frame);
            if(frame.empty())
                continue; // Stream is probably shutting down. If so, run flag will be false
            if(frameCallback != nullptr)
                (*frameCallback)(frame);
        }
    });
    return true;
}

void CameraManager::stopStream(unsigned int streamPort){
    if(streams.find(streamPort) == streams.end())
        return; // No such stream
    
    Logger::logInfoFrom("CameraManager", "Stopping stream " + std::to_string(streamPort));

    if(streams[streamPort].proc != NULL){
        pclose(streams[streamPort].proc);
        remove(streams[streamPort].fifo.c_str());
    }

    streams[streamPort].run = false;
    streams[streamPort].cap.release();
    streams[streamPort].thread.join();

    streams.erase(streamPort);
}



std::string CameraManager::getSource(Camera cam){
    if(cam.api == "libcamera"){
        // Note: libcamerasrc doesn't support setting controls (yet)
        // If controls are needed use v4l2 or rpicam
        return "libcamerasrc camera-name=" + cam.id;
    }else if(cam.api == "v4l2"){
        // TODO: Controls
        return "v4l2src device=" + cam.id;
    }else if(cam.api == "rpicam"){
        return "filesrc location=" + getFifo(cam);
    }else{
        return "";
    }
}

std::string CameraManager::getFifo(Camera cam){
    if(cam.api == "rpicam"){
        std::string pipeName = "stream" + cam.id;
        std::replace(pipeName.begin(), pipeName.end(), '/', '_');
        pipeName.erase(std::remove(pipeName.begin(), pipeName.end(), '@'));
        return "/tmp/" + pipeName;
    }
    return "";
}

// TODO: Log errors in this function
std::string CameraManager::getProcCmd(std::string fifo, Camera cam, 
        std::string format, std::string width, std::string height, std::string framerate){

    if(cam.api == "rpicam"){
        // Convert camera id (path) into index for rpicam-vid
        int camIndex = -1;
        std::string getIdCmd = "/usr/bin/env bash -c \"rpicam-vid --list-cameras | grep '" + cam.id + "' | cut -d: -f1\"";
        FILE *p = popen(getIdCmd.c_str(), "r");
        if(p == NULL)
            return "";
        char buf[32];
        if(fgets(buf, sizeof(buf), p) != NULL){
            camIndex = atoi(buf);
        }
        pclose(p);
        if(camIndex < 0)
            return "";

        if(framerate != ""){
            int slashPos = framerate.find("/");
            std::string numstr = framerate.substr(0, slashPos);
            std::string denstr = framerate.substr(slashPos + 1, framerate.length() - slashPos);
            if(denstr == "1"){
                framerate = numstr;
            }else{
                try{
                    double frdouble = std::stod(numstr) / std::stod(denstr);
                    framerate = std::to_string(frdouble);
                }catch(const std::exception &e){
                    return "";
                }
            }
        }

        // TODO: Controls
        std::string codec;
        if(format == "jpeg")
            codec = "mjpeg";
        else if(format == "raw")
            codec = "yuv420";
        else if(format == "h264")
            codec = "h264";
        else
            return "";
        std::string cmd = std::string("/usr/bin/rpicam-vid -t 0 -v 0 --flush=1") + 
                " --camera=" + std::to_string(camIndex) + 
                " --codec=" + codec +
                " --width=" + width + 
                " --height=" + height;
        if(framerate != "")
            cmd += " --framerate=" + framerate;
        cmd += " -o " + fifo;
        return cmd;
    }

    return "";
}






void CameraManager::initV4l2(){
    Logger::logDebugFrom("CameraManager", "Enumerating v4l2 devices.");
    auto provider = gst_device_provider_factory_get_by_name("v4l2deviceprovider");
    if(provider == NULL){
        Logger::logWarningFrom("CameraManager", "Unable to find v4l2 provider! v4l2 devices will be unavailable.");
    }else{
        Logger::logDebugFrom("CameraManager", "Found v4l2 provider. Searching for devices.");
        auto devices = gst_device_provider_get_devices(provider);
        for(auto element = devices; element; element = element->next){
            auto dev = (GstDevice*)element->data;
            if(gst_device_has_classes(dev, "Video/Source")){
                // This device is a camera
                auto props = gst_device_get_properties(dev);
                if(props){
                    if(gst_structure_has_field(props, "device.path")){
                        // Camera has a /dev/video device path
                        std::string id = std::string(gst_structure_get_string(props, "device.path"));
                        Logger::logDebugFrom("CameraManager", "Discovered v4l2 device '" + id + "'");

                        // Have all required info. Add this device
                        cameras.push_back(Camera{
                            .api = "v4l2",
                            .id = id
                        });
                    }
                }
                gst_structure_free(props);
            }
        }
        g_list_free(devices);
    }
}

void CameraManager::initLibcamera(){
    // Suppress INFO messages from libcamera
    setenv("LIBCAMERA_LOG_LEVELS", "*:WARNING", true);

    Logger::logDebugFrom("CameraManager", "Enumerating libcamera devices.");
    auto provider = gst_device_provider_factory_get_by_name("libcameraprovider");
    if(provider == NULL){
        Logger::logWarningFrom("CameraManager", "Unable to find libcamera provider! libcamera devices will be unavailable.");
    }else{
        Logger::logDebugFrom("CameraManager", "Found libcamera provider. Searching for devices.");
        auto devices = gst_device_provider_get_devices(provider);
        for(auto element = devices; element; element = element->next){
            auto dev = (GstDevice*)element->data;
            if(gst_device_has_classes(dev, "Video/Source")){
                // This device is a camera
                auto idchars = gst_device_get_display_name(dev);
                if(idchars){
                    std::string id = std::string(idchars);
                    Logger::logDebugFrom("CameraManager", "Discovered libcamera device '" + id + "'");

                    // Have all required info. Add this device
                    cameras.push_back(Camera{
                        .api = "libcamera",
                        .id = id,
                    });

                    // rpicam-apps supports non-usb cameras
                    // This is the method used in rpicam-apps source code to identify devices
                    if(id.find("/usb") == std::string::npos){
                        cameras.push_back(Camera{
                            .api = "rpicam",
                            .id = id,
                        });
                    }
                }
            }
        }
        g_list_free(devices);
    }
}
