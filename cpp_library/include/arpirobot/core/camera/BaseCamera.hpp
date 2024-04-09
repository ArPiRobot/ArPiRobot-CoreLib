#pragma once

#include <string>
#include <thread>
#include <opencv2/opencv.hpp>

// TODO: Some way to stop any running streams when
// robot is stopping. May require something similar to 
// what is done for IoDevices
// Probably just static private map<port, object> in this class
// that will track in use ports and objects with running streams
// objects tracked by pointers? Yes. Handling object scope is not
// the problem here. ctor / dtor will handle add / remove from list
// regardless. So it doesn't actually matter

// TODO: Each child class of BaseCamera could have static method
// listCameras() that enumerates cameras for that backend
// If double calling gst init is fine, this is easy (just)
// never gst deinit
// If it can't be called multiple times, probably need static bool in
// base class to track if gst has been initialized
// For rpicam cameras this would use 

namespace arpirobot{
    class BaseCamera{
    public:
        BaseCamera(std::string id);
        virtual ~BaseCamera();

        /**
         * Get backend-specific device ID for this camera
         */
        std::string getId();

        /**
         * Configure capture settings for this camera
         * @param mode String describing mode information in the following form
         *             [format]:[width]x[height]@[framerate]
         *             format and framerate can be omitted. They will default to
         *             raw and 30 respectively if they are omitted.
         *             Framerate can be specified as either a floating point value
         *             (eg 30 or 7.5) or as a fraction (eg 30/1 or 15/2)
         * @return true if the provided mode string is successfully parsed. Else false
         */
        bool setCaptureMode(std::string mode);

        /**
         * Configure hardware acceleration for streaming with this camera
         * @param hwencode Use hardware encoders if available
         * @param hwdecode Use hardware decoders if available
         * @param hwconvert Use hardware converters if available
         */
        void configHWAccel(bool hwencode, bool hwdecode, bool hwconvert);

        ////////////////////////////////////////////////////////////////////////
        // Backend can modify, but usually won't need to
        ////////////////////////////////////////////////////////////////////////

        virtual bool startStreamH264(unsigned int port, unsigned int bitrate, 
                std::string profile, std::string level);

        virtual bool startStreamJPEG(unsigned int port, unsigned int quality);

        virtual void startStream(unsigned int port, std::string pipeline);

        virtual void stopStream();

        ////////////////////////////////////////////////////////////////////////
    
        ////////////////////////////////////////////////////////////////////////
        // Backend specific
        ////////////////////////////////////////////////////////////////////////

        /**
         * Get name of this camera's backend
         */
        virtual std::string getBackend() = 0;

        /**
         * Get gstreamer pipeline to capture from this camera in the
         * configured mode.
         */
        virtual std::string getCapturePipeline() = 0;

        ////////////////////////////////////////////////////////////////////////

    private:
        // Backend-specific Device ID
        std::string id;

        // These settings are default as they should work for most cameras
        std::string format = "raw";
        std::string width = "640";
        std::string height = "480";
        std::string framerateFrac = "30/1";
        std::string framerateDec = "30";

        // HW accel settings
        bool hwencode = true;
        bool hwdecode = true;
        bool hwconvert = true;

        // Used while streaming
        bool isStreaming = false;
        bool runStream;
        cv::VideoCapture cap;
        std::thread thread;
    };
}