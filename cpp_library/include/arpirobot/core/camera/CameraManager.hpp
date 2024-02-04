#pragma once

#include <vector>
#include <memory>
#include <arpirobot/core/camera/CameraDevice.hpp>
#include <arpirobot/core/camera/CameraVideoMode.hpp>
#include <gst/gst.h>

namespace arpirobot{

    // TODO: Doc comments

    class CameraManager{
    public:    
        static void init();

        static std::vector<CameraDevice> getDevices();

    private:

        static bool gstCapToVideoMode(CameraVideoMode &mode, GstStructure *cap);

        static void initV4l2();

        static void initLibcamera();


        // Notes on states:
        // If gstraemer is using a camera, can't configure it using libcamera (and maybe v4l2 also)
        // So essentially
        // init()
        //      Phase 1 uses gstreamer to enumerate devices & collect format info
        //      Phase 2 uses either v4l2 or libcamera APIs to collect controls and other info
        //      After init, neither gstreamer or native APIs are using the device
        // config() function
        //      Use either v4l2 or libcamera APIs to set controls
        // startStream()
        //      Uses gstramer (via opencv) to stream camera to rtsp server and provide app access
        //      While any stream is running, configuration of ANY device is disallowed
        //      This is because libcamera doesn't support multiple CameraManager's
        //      Which means gstreamer must have a CameraManager, thus this can't
        //      V4L2 devices could technically be configured (unless they are actively streaming)
        //      but easiest to just avoid the issue entirely by having 3 distinct phases.
        enum class State{
            UNINITIALIZED,      // Hasn't called init yet
            IDLE,               // Called init, but not using cameras. Can configure.
            RUNNING             // Using cameras. Can't configure.
        };

        static State state;
        static std::vector<CameraDevice> devices;
    };

}
