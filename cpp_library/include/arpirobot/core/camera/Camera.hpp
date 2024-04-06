#pragma once

#include <set>
#include <string>

namespace arpirobot{
    class Camera{
    public:
        std::string api;
        std::string id;
        std::set<std::string> modes; // [format]:[width]x[height]@[framerate]   framerate is optional
    };
}