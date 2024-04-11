#pragma once

#include <string>
#include <thread>
#include <opencv2/opencv.hpp>

// IoDevice is really just a way to make sure close() is called
// when the robot is stopped or the program dies no matter what
// this is the same thing that should be done for cameras
// to ensure that a camera thread / subprocess is closed down
// properly in all cases
// So instead of creating something with a different name, that is essentially the
// same exact thing, just use IoDevice here, even though this isn't actually
// using the Io interface.
#include <arpirobot/core/io/IoDevice.hpp>


// TODO: Each child class of BaseCamera could have static method
// listCameras() that enumerates cameras for that backend
// Should just return list of ids
// Note: make sure implementations check if gst is initialized before gst init call

namespace arpirobot{

    // TODO: Inherit from IoDevice & implement close() function
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

        // TODO: Make sure HWaccel and capture mode can't change once isStreaming
        // is set to true. May actually need to mutex config and stream management
        // because if config settings change during stream start process

        /**
         * Configure hardware acceleration for streaming with this camera
         * @param hwencode Use hardware encoders if available
         * @param hwdecode Use hardware decoders if available
         * @param hwconvert Use hardware converters if available
         */
        void configHWAccel(bool hwencode, bool hwdecode, bool hwconvert);

        /**
         * Function to be called when a frame is read from this camera
         * Note: frames are only read from this camera while it is streaming
         * @param frameCallback function to call when a frame is read
         */
        void setFrameCallback(std::function<void(cv::Mat)> *frameCallback);

        ////////////////////////////////////////////////////////////////////////
        // Backend can modify, but usually won't need to
        ////////////////////////////////////////////////////////////////////////

        virtual bool startStreamH264(unsigned int port, unsigned int bitrate, 
                std::string profile, std::string level);

        virtual bool startStreamJPEG(unsigned int port, unsigned int quality);

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

    protected:

        // TODO: This function will actually start the stream
        // Capture and encode pipeline are passed in
        // If capture mode matches stream mode, the encode pipeline will be empty
        // (that check is handled by startH264 and startJpeg functions)
        // If capture mode is raw, this function won't setup a decode pipeline stage
        // Thus, most of the pipeline construction code is shared among all image formats
        virtual void startStream(unsigned int port, std::string capturePipeline, std::string encodePipeline);

        // TODO: Make sure to init gst if needed in gstHasElement
        static bool gstHasElement(std::string elementName);
        static std::string getVideoConvertElement(bool hwaccel);
        static std::string getH264EncodeElement(bool hwaccel, unsigned int bitrate, std::string profile, std::string level);
        static std::string getJpegEncodeElement(bool hwaccel, unsigned int quality);
        static std::string getH264DecodeElement(bool hwaccel);
        static std::string getJpegDecodeElement(bool hwaccel);

        // Backend-specific Device ID
        std::string id;

        // These settings are default as they should work for most cameras
        std::string format = "raw";
        std::string width = "640";
        std::string height = "480";
        std::string framerateFrac = "30/1";
        std::string framerateDec = "30";

        // Called when frame read from this camera
        std::function<void(cv::Mat)> *frameCallback = nullptr;

        // HW accel settings
        bool hwencode = true;
        bool hwdecode = true;
        bool hwconvert = true;

        // Stream management
        std::mutex streamMutex;     // Used to ensure thread safe config and stream management
        bool isStreaming = false;
        bool runStream;
        cv::VideoCapture cap;
        std::thread thread;
    };
}