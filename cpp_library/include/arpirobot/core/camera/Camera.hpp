#pragma once

#include <vector>
#include <string>

namespace arpirobot{
    class Camera{
    public:
        std::string api;
        std::string id;
        std::vector<std::string> modes; // [format]:[width]x[height]@[framerate]   framerate is optional
    };
}