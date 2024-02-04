#pragma once

#include <arpirobot/core/camera/BaseCameraDevice.hpp>
#include <string>
#include <memory>
#include <libcamera/libcamera.h>

/**
 * Implementation of BaseCameraDevice using libcamera
 */
class LibcameraCameraDevice : public BaseCameraDevice{
public:
    LibcameraCameraDevice(std::shared_ptr<libcamera::Camera> cam);

    std::string getId() override;

    std::string getName() override;

    bool supportsInputFormat(InputFormat format) override;

    std::string capturePipeline(InputFormat format) override;

    void setControl(std::string control, std::string value) override;

    std::string getControl(std::string control) override;

    bool hasControl(std::string control) override;

    std::vector<std::string> supportedControls() override;

private:
    std::shared_ptr<libcamera::Camera> cam;
};
