#pragma once

#include <string>
#include <vector>
#include <arpirobot/core/camera/CameraVideoMode.hpp>

// TODO: Doc comments

namespace arpirobot{
    
    class CameraDevice{
    public:
        enum class CameraAPI{
            V4L2 = 0,
            LIBCAMERA = 1
        };

        CameraDevice(CameraAPI api, std::string id, std::string name, std::vector<CameraVideoMode> modes);

        CameraAPI getApi();

        std::string getId();

        std::string getName();

        std::vector<CameraVideoMode> getModes();

    private:
        CameraAPI api;
        std::string id;
        std::string name;
        std::vector<CameraVideoMode> modes;

        // TODO Controls list
    };

}