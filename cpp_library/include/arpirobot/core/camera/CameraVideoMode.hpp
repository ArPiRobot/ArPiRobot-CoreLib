#pragma once

#include <string>

namespace arpirobot{
    class CameraVideoMode{
    public:
        enum class Format { MJPEG = 0, YUY2 = 1 };
        class Framerate { 
        public:
            unsigned int numerator; 
            unsigned int denominator;

            std::string to_string(){ return std::to_string(numerator) + "/" + std::to_string(denominator); }
        };

        unsigned int width;
        unsigned int height;
        std::vector<Framerate> framerates;
        Format format;
    };
}