#pragma once

#include <arpirobot/core/camera/InputFormat.hpp>
#include <string>
#include <vector>

/**
 * A discovered camera device from one of several camera providers
 */
class BaseCameraDevice{
public:
    /**
     * Get the camera identifier (typically integer or path)
     * @return identifier as a string
     */
    virtual std::string getId() = 0;

    /**
     * Get the name of this camera
     * @return name string
     */
    virtual std::string getName() = 0;

    /**
     * Check if this camera supports a specific input format
     * @return true if supported, else false
     */
    virtual bool supportsInputFormat(InputFormat format) = 0;

    /**
     * Get a gstreamer pipeline to capture from this camera
     * @return gstreamer pipeline string
     */
    virtual std::string capturePipeline(InputFormat format) = 0;

    /**
     * Set a camera control value
     * Controls available depend on the camera and provider
     * @param control The name of the control
     * @param value the value to set the control to
     */
    virtual void setControl(std::string control, std::string value) = 0;

    /**
     * Get a camera control value
     * Controls available depend on the camera and provider
     * @param control The name of the control
     * @return Value of the control
     */
    virtual std::string getControl(std::string control) = 0;

    /**
     * Check if this camera has a control value
     * Controls available depend on the camera and provider
     * @param control The name of the control
     * @return true if the camera supports the given control, else false
     */
    virtual bool hasControl(std::string control) = 0;

    /**
     * Get a list of all controls supported by this camera
     * @return a List of controls (names, no values) supported
     */
    virtual std::vector<std::string> supportedControls() = 0;
};
