#pragma once

#include <arpirobot/core/camera/CameraVideoMode.hpp>
#include <arpirobot/core/camera/CameraDevice.hpp>

namespace arpirobot{

    class CameraStreamSettings{
    public:
        enum class OutputFormat{
            MJPEG = 0,
            H264 = 1
        };

        enum class H264Encoder{
            LIBV4L2 = 0,            // HW accelerated on some SBCs (including RPi)
            LIBX264 = 1             // SW encoding
        };

        CameraDevice camera;
        CameraVideoMode inputMode;
        std::string streamName;
        OutputFormat outputFormat;
        H264Encoder H264Encoder;
    };

}