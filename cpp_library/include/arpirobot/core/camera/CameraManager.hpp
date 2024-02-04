#pragma once

#include <vector>
#include <memory>
#include <arpirobot/core/camera/BaseCameraDevice.hpp>
#include <arpirobot/core/camera/LibcameraCameraDevice.hpp>
#include <arpirobot/core/camera/V4L2CameraDevice.hpp>


class CameraManager{
public:
    static void init();

    static std::vector<std::shared_ptr<LibcameraCameraDevice>> getLibcameraDevices();

    static std::vector<std::shared_ptr<V4L2CameraDevice>> getV4l2Devices();

private:
    static std::vector<std::shared_ptr<LibcameraCameraDevice>> libcameraDevices;
    static std::vector<std::shared_ptr<V4L2CameraDevice>> v4l2Devices;
};
