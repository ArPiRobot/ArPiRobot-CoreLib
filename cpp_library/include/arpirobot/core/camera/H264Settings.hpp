#pragma once

#include <string>

namespace arpirobot{
    class H264Settings{
    public:
        std::string profile;
        std::string level;
        unsigned int bitrate;

        static H264Settings Default;
    };
}