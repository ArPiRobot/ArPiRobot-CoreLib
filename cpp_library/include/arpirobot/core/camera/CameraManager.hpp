#pragma once

#include <vector>
#include <unordered_map>
#include <thread>
#include <memory>
#include <string>
#include <list>
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
         * @param streamName Unique name for the stream
         * @param cam Camera to stream from
         * @param mode Frame input mode for the camera
         * @param frameCallback Function to call when frames are read from the camera
         * @param hwaccel If true, will use hardware acceleration when available
         * @param h264profile Profile for h264 encoding (baseline, main, or high, default baseline)
         * @param h264bitrate Bitrate for h264 stream encoding (in bits/sec, default 2048000)
         */
        static bool startStreamH264(std::string streamName, 
                Camera cam, 
                std::string mode, 
                std::function<void(cv::Mat)> *frameCallback = nullptr, 
                bool hwaccel = true,
                std::string h264profile = "baseline",
                unsigned int h264bitrate = 2048000);
        
        /**
         * Start a stream encoded using MJPEG
         * @param streamName Unique name for the stream
         * @param cam Camera to stream from
         * @param mode Frame input mode for the camera
         * @param frameCallback Function to call when frames are read from the camera
         * @param hwaccel If true, will use hardware acceleration when available
         * @param quality JPEG image quality 0-100 (default 80)
         */
        static bool startStreamMjpeg(std::string streamName,
                Camera cam,
                std::string mode,
                std::function<void(cv::Mat)> *frameCallback = nullptr,
                bool hwaccel = true,
                unsigned int quality = 80);

        /**
         * Start a stream using the provided pipeline
         * @param streamName Unique name for the stream
         * @param capturePipeline Gstreamer pipeline string for capture
         * @param publishPipeline Gstreamer pipeline string for publish
         * @param frameCallback Function to call when frames are read from the camera
         */
        static bool startStreamFromPipelines(std::string streamName,
                std::string capturePipeline,
                std::string publishPipeline,
                std::function<void(cv::Mat)> *frameCallback = nullptr);


        static void stopStream(std::string streamName);
        
        static void stopAllStreams();

    private:
        static bool gstHasElement(std::string elementName);
        static std::vector<std::string> gstCapToVideoModes(GstStructure *cap);
        static std::string getCapturePipeline(Camera cam, std::string mode, bool hwaccel);

        static std::string getVideoConvertElement(bool hwaccel);
        static std::string getH264EncodeElement(bool hwaccel, std::string profile, std::string bitrate);
        static std::string getJpegEncodeElement(bool hwaccel, std::string quality);
        static std::string getJpegDecodeElement(bool hwaccel);

        static void initV4l2();

        static void initLibcamera();

        static bool initialized;
        static bool canConfigure;
        static std::vector<Camera> cameras;

        class Stream;

        static std::unordered_map<std::string, Stream> streams;

        class Stream{
        public:
            bool run;
            cv::VideoCapture cap;
            cv::VideoWriter pub;
            std::thread thread;
        };
    };

}
