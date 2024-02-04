#pragma once

namespace arpirobot{
    class CameraVideoMode{
    public:
        enum class Format { MJPEG = 0, YUY2 = 1 };

        unsigned int width;
        unsigned int height;
        unsigned int framerate;
        Format format;
    };
}