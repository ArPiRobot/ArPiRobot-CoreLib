
#include <arpirobot/core/camera/BaseCamera.hpp>

using namespace arpirobot;


BaseCamera::BaseCamera(std::string id) : id(id){
    // TODO: ANything else needed in initializer list?
}

BaseCamera::~BaseCamera(){
    //TODO: close();
}

// TODO: close function

std::string BaseCamera::getId(){
    return id;
}

bool BaseCamera::setCaptureMode(std::string mode){
    // Configuration and stream management must be mutually exclusive
    // If this config changes part way through stream start, stream will
    // be started incorrectly
    std::lock_guard<std::mutex> l(mutex);

    if(isStreaming)
        return false;
    
    // TODO: Parse, validate, and apply capture mode

    return true; // Should be false if parse / validate fails
}

bool BaseCamera::configHWAccel(bool hwencode, bool hwdecode, bool hwconvert){
    /// If this config changes part way through stream start, stream will
    // be started incorrectly
    std::lock_guard<std::mutex> l(mutex);

    if(isStreaming)
        return false;
    
    this->hwencode = hwencode;
    this->hwdecode = hwdecode;
    this->hwconvert = hwconvert;
}

void BaseCamera::setFrameCallback(std::function<void(cv::Mat)> *frameCallback){
    // No need to mutex this. This doesn't impact stream startup
    // process. It can also change during a stream since the pointer in the object
    // is accessed.
    // TODO: When framecallback is run in stream thread
    // the stream thread should perform a single read. Otherwise check then use
    // could have thread safety problems
    this->frameCallback = frameCallback;
}

bool BaseCamera::startStreamH264(unsigned int port, unsigned int bitrate, 
        std::string profile, std::string level){
    std::lock_guard<std::mutex> l(mutex);
    if(isStreaming)
        return false;

    // Note: Even if input is already h264, we still want to
    // re-encode because encoder settings may not be correct
    std::string encodePipeline = ""; // TODO
    return doStartStream(port, encodePipeline);
}

bool BaseCamera::startStreamJPEG(unsigned int port, unsigned int quality){
    std::lock_guard<std::mutex> l(mutex);
    if(isStreaming)
        return false;

    // Note: Even if input is already jpeg, we still want to
    // re-encode because encoder settings may not be correct
    std::string encodePipeline = ""; // TODO
    return doStartStream(port, encodePipeline);
}

void BaseCamera::stopStream(){
    std::lock_guard<std::mutex> l(mutex);
    if(isStreaming)
        doStopStream();
}

// TODO: This is entirely re-implemented for RpicamCamera
// rpicam-vid would provide frames in a "ready to stream"
// state. We then use decode pipeline to get into appsrc
bool BaseCamera::doStartStream(unsigned int port, std::string encodePipeline){
    // Note: not taking mutex here. This will be called from another function
    // that is still holding it

    // Capture pipeline includes correct capsfilter
    std::string capturePipeline = getCapturePipeline();
    
    // TODO: Construct this based on frame input mode. We always need
    // have to decode because OpenCV needs raw frames for application to access
    std::string decodePipeline = "";

    // Construct full pipeline
    // Note: after input, there is a SW videoconvert because HW converter often have
    // limitations that cause pipelines to break with some cameras. videoconvert can usually
    // fix such issues (normally just colorimetry)
    std::string pipeline = 
            capturePipeline + " ! videoconvert ! ";
            // TODO: Finish pipeline construction
    
    // TODO: Actually run stream
}

void BaseCamera::doStopStream(){
    // TODO
}

// TODO: Implement all these
// TODO: Make sure to init gst if needed in gstHasElement
bool gstHasElement(std::string elementName);
std::string getVideoConvertElement(bool hwaccel);
std::string getH264EncodeElement(bool hwaccel, unsigned int bitrate, std::string profile, std::string level);
std::string getJpegEncodeElement(bool hwaccel, unsigned int quality);
std::string getH264DecodeElement(bool hwaccel);
std::string getJpegDecodeElement(bool hwaccel);
