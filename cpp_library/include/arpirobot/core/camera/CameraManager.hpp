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


        // TODO: Implement these
        // if hwaccel == true it will attempt to use a hw encoder/decoder if available
        // Eg: If v4l2h264enc exists, use it. Else use libx264enc
        // Eg: If v4l2jpegenc exists, use it. Else use jpegenc
        // Eg: If v4l2jpegdec exists, use it. Else use jpegdec

        /**
         * Start a stream encoded using H.264
         * @param streamName Unique name for the stream
         * @param cam Camera to stream from
         * @param mode Frame input mode for the camera
         * @param frameCallback Function to call when frames are read from the camera
         * @param hwaccel If true, will use hardware acceleration when available
         */
        static bool startStreamH264(std::string streamName, 
                Camera cam, 
                std::string mode, 
                std::function<void(cv::Mat)> *frameCallback = nullptr, 
                bool hwaccel = true);
        
        /**
         * Start a stream using the provided pipeline
         * @param streamName Unique name for the stream
         * @param pipeline Gstreamer pipeline string
         * @param frameCallback Function to call when frames are read from the camera
         */
        static bool startStreamFromPipeline(std::string streamName,
                std::string pipeline,
                std::function<void(cv::Mat)> *frameCallback = nullptr);

        // TODO: MJPEG start

        static void stopStream(std::string streamName);
        

    private:
        static std::vector<std::string> gstCapToVideoModes(GstStructure *cap);

        static void initV4l2();

        static void initLibcamera();

        static bool initialized;
        static bool canConfigure;
        static std::vector<Camera> cameras;

        static std::unordered_map<std::string, bool> streamRunFlags;
        static std::unordered_map<std::string, cv::VideoCapture> streamCaptures;
        static std::unordered_map<std::string, std::thread> streamThreads;
    };

}
