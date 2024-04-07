#pragma once

#include <string>

namespace arpirobot{
    class JpegSettings{
    public:
        unsigned int quality;

        static JpegSettings Default;
    };
}