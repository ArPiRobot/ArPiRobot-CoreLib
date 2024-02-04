
#include <arpirobot/core/camera/CameraManager.hpp>
#include <libcamera/libcamera.h>
#include <arpirobot/core/log/Logger.hpp>
#include <unistd.h>
#include <filesystem>
#include <regex>

using namespace arpirobot;

std::vector<std::shared_ptr<LibcameraCameraDevice>> CameraManager::libcameraDevices;
std::vector<std::shared_ptr<V4L2CameraDevice>> CameraManager::v4l2Devices;
std::unique_ptr<libcamera::CameraManager> CameraManager::libcameraMgr = nullptr;

void CameraManager::discoverCameras(){
    libcameraDevices.clear();
    v4l2Devices.clear();

    Logger::logDebugFrom("CameraManager", "Starting camera discovery.");

    // Discover libcamera devices
    if(libcameraMgr == nullptr)
        libcameraMgr = std::make_unique<libcamera::CameraManager>();
    libcameraMgr->start();
    for(auto const &cam : libcameraMgr->cameras()){
        Logger::logDebugFrom("CameraManager", "Found libcamera device " + cam->id());
        libcameraDevices.push_back(std::make_shared<LibcameraCameraDevice>(cam));
    }

    // Discover v4l2 devices
    for(const auto &item : std::filesystem::directory_iterator("/dev/")){
        auto path = item.path().string();
        if(std::regex_match(path.begin(), path.end(), std::regex("/dev/video[0-9]+"))){
            Logger::logDebugFrom("CameraManager", "Found v4l2 device " + path);
            v4l2Devices.push_back(std::make_shared<V4L2CameraDevice>(path));
        }
    }


    unsigned int count = libcameraDevices.size() + v4l2Devices.size();
    Logger::logInfoFrom("CameraManager", "Discovered " + std::to_string(count) + " devices.");
}
