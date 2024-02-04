#pragma once

#include <arpirobot/core/camera/BaseCameraDevice.hpp>
#include <string>

/**
 * Implementation of BaseCameraDevice using v4l2
 */
class V4L2CameraDevice : public BaseCameraDevice{
public:
    V4L2CameraDevice(std::string id);

    std::string getId() override;

    std::string getName() override;

    bool supportsInputFormat(InputFormat format) override;

    std::string capturePipeline(InputFormat format) override;

    void setControl(std::string control, std::string value) override;

    std::string getControl(std::string control) override;

    bool hasControl(std::string control) override;

    std::vector<std::string> supportedControls() override;

private:
    std::string id;
};
