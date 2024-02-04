#pragma once

#include <string>
#include <vector>
#include <arpirobot/core/camera/CameraAPI.hpp>
#include <arpirobot/core/camera/CameraVideoMode.hpp>

// TODO: Doc comments

namespace arpirobot{
    
    class CameraDevice{
    public:
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