#pragma once

#include <vector>
#include <memory>
#include <arpirobot/core/camera/BaseCameraDevice.hpp>
#include <arpirobot/core/camera/LibcameraCameraDevice.hpp>
#include <arpirobot/core/camera/V4L2CameraDevice.hpp>


class CameraManager{
public:
    static void discoverCameras();

    static std::vector<std::shared_ptr<BaseCameraDevice>> getLibcameraDevices();

    static std::vector<std::shared_ptr<BaseCameraDevice>> getV4l2Devices();

private:
    static std::vector<std::shared_ptr<LibcameraCameraDevice>> libcameraDevices;
    static std::vector<std::shared_ptr<V4L2CameraDevice>> v4l2Devices;
    static std::unique_ptr<libcamera::CameraManager> libcameraMgr;
};
