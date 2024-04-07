
#include <arpirobot/core/camera/H264Settings.hpp>

using namespace arpirobot;

H264Settings H264Settings::Default = {
    .profile = "baseline",
    .level = "4",
    .bitrate = 2048
};
