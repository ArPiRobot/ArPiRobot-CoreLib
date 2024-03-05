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
        bool startStreamH264(std::string streamName, Camera cam, std::string mode, bool hwaccel = true);
        bool startStreamMjpeg(std::string streamName, Camera cam, std::string mode, bool hwaccel = true);
        bool stopStream(std::string streamName);

        // TODO: Support for a callback function when frames read from a certain camera (for use with OpenCV)
        // TODO: Anything using OpenCV would need to be language-native allowing access to OpenCV in all langs

    private:
        static std::vector<std::string> gstCapToVideoModes(GstStructure *cap);

        static bool startStreamGemeric(std::string streamName, std::string pipeline);

        static void initV4l2();

        static void initLibcamera();


        static bool initialized;
        static bool canConfigure;
        static std::vector<Camera> cameras;
        static std::unordered_map<std::string, std::unique_ptr<cv::VideoCapture>> streamCaptures;
        static std::unordered_map<std::string, std::thread> streamThreads;
        static std::unordered_map<std::string, bool> streamRunFlags;
    };

}
