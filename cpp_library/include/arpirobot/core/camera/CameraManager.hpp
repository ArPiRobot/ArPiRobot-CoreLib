#pragma once

#include <vector>
#include <memory>
#include <string>
#include <gst/gst.h>
#include <arpirobot/core/camera/Camera.hpp>

namespace arpirobot{

    // TODO: Doc comments

    class CameraManager{
    public:
        static void init();

        static std::vector<Camera> getCameras();

        // TODO: Some way to set controls??? Maybe just call v4l2-ctl or cam?
        // TODO: Is listing controls necessary? Or is that on the user as an advanced feature?

    private:
        static std::vector<std::string> gstCapToVideoModes(GstStructure *cap);

        static void initV4l2();

        static void initLibcamera();


        static bool initialized;
        static bool canConfigure;
        static std::vector<Camera> cameras;
    };

}
