#pragma once

#include <vector>
#include <unordered_map>
#include <thread>
#include <memory>
#include <string>
#include <list>
#include <set>
#include <gst/gst.h>
#include <opencv2/opencv.hpp>
#include <arpirobot/core/camera/Camera.hpp>
#include <arpirobot/core/scheduler.hpp>

namespace arpirobot{

    // TODO: Doc comments

    class CameraManager{
    public:
        static void init();

        static std::vector<Camera> getCameras();

        // TODO: Some way to set controls??? Maybe just call v4l2-ctl or cam?
        // TODO: Is listing controls necessary? Or is that on the user as an advanced feature?

        /**
         * Start a stream encoded using H.264
         * @param streamPort Port number for the stream
         * @param cam Camera to stream from
         * @param mode Frame input mode for the camera
         * @param frameCallback Function to call when frames are read from the camera
         * @param h264profile Profile for h264 encoding (baseline, main, or high, default baseline)
         * @param h264bitrate Bitrate for h264 stream encoding (in kbits/sec, default 2048)
         * @param hwencode If true, will use hardware accelerated encoder when available
         * @param hwdecode If true, will use hardware accelerated decoder when available
         * @param hwconv If true, will use hardware accelerated convert element when available
         */
        static bool startStreamH264(unsigned int streamPort, 
                Camera cam, 
                std::string mode, 
                std::function<void(cv::Mat)> *frameCallback = nullptr,
                std::string h264profile = "baseline",
                std::string h264level = "4",
                unsigned int h264bitrate = 2048,
                bool hwencode = true, bool hwdecode = true, bool hwconv = true);

        // TODO: Start Stream MJPEG

        /**
         * Start a stream using the provided pipeline
         * @param streamPort Port number for the stream
         * @param pipeline Gstreamer pipeline
         * @param frameCallback Function to call when frames are read from the camera
         */
        static bool startStreamFromPipeline(unsigned int streamPort,
                std::string pipeline,
                std::function<void(cv::Mat)> *frameCallback = nullptr);


        static void stopStream(unsigned int streamPort);
        
        static void stopAllStreams();

    private:
        static bool gstHasElement(std::string elementName);
        
        static std::string getCapturePipeline(Camera cam, std::string mode, bool hwaccel);

        static std::string getVideoConvertElement(bool hwaccel);
        static std::string getH264EncodeElement(bool hwaccel, std::string profile, std::string level, std::string bitrate);
        static std::string getH264DecodeElement(bool hwaccel);
        static std::string getJpegDecodeElement(bool hwaccel);

        static std::set<std::string> gstCapToVideoModes(GstStructure *cap);
        static void initV4l2();
        static void initLibcamera();

        static bool initialized;
        static bool canConfigure;
        static std::vector<Camera> cameras;

        class Stream;

        static std::unordered_map<unsigned int, Stream> streams;

        class Stream{
        public:
            bool run;
            cv::VideoCapture cap;
            std::thread thread;
        };
    };

}
