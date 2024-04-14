/*
 * Copyright 2024 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

#pragma once

#include <string>
#include <thread>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <arpirobot/core/io/IoDevice.hpp>
#include <boost/process.hpp>


// TODO: Each child class of BaseCamera could have static method
// listCameras() that enumerates cameras for that backend
// Should just return list of ids
// Note: make sure implementations check if gst is initialized before gst init call

namespace arpirobot{
    // TOOD: Doc comments
    class BaseCamera : public IoDevice{
    public:
        BaseCamera(std::string id);
        virtual ~BaseCamera();

        void close() override;

        /**
         * Get backend-specific device ID for this camera
         */
        std::string getId();

        /**
         * Configure capture settings for this camera
         * @param mode String describing mode information in the following form
         *             [format]:[width]x[height]@[framerate]
         *             format and framerate can be omitted. They will default to
         *             raw and 30 respectively if they are omitted.
         *             Framerate can be specified as fraction or whole integer
         *             Format is optional, defaulting to raw if not specified
         *             Framerate is optional, defaulting to 30/1 if not specified
         *             Omit the colon if not specifying format
         *             Omit the at symbol if not specifying framerate
         * @return true if the provided mode string is successfully applied.
         *         false otherwise. This may mean the mode is invalid
         *         or it may mean that a stream is currently running. The capture
         *         mode cannot be changed while a stream is running
         */
        bool setCaptureMode(std::string mode);

        /**
         * Configure hardware acceleration for streaming with this camera
         * @param hwencode Use hardware encoders if available
         * @param hwdecode Use hardware decoders if available
         * @param hwconvert Use hardware converters if available
         * @return true on success. Only returns false if a stream is running
         */
        bool setHwAccel(bool hwencode, bool hwdecode, bool hwconvert);

        // TODO: setExtaControls
        // Will not work for all implementations. but will work for some. advanced feature.

        /**
         * Function to be called when a frame is read from this camera
         * Note: frames are only read from this camera while it is streaming
         * @param frameCallback function to call when a frame is read
         */
        void setFrameCallback(std::function<void(cv::Mat)> *frameCallback);

        /**
         * Start a H264 stream
         * TODO: params
         * @return true on success, else false (could be pipeline failure, or a stream may already be running)
         */
        bool startStreamH264(unsigned int port, unsigned int bitrate = 2048, 
                std::string profile = "baseline", std::string level = "");

        /**
         * Start a JPEG stream
         * TODO: params
         * @return true on success, else false (could be pipeline failure, or a stream may already be running)
         */
        bool startStreamJpeg(unsigned int port, unsigned int quality = 80);

        /**
         * Stop the running stream
         */
        void stopStream();
    
        // Backend specific
        /**
         * Get name of this camera's backend
         */
        virtual std::string getBackend() = 0;

    protected:
        // Backend specific
        virtual std::string getDeviceName() = 0;
        virtual std::string getCapturePipeline() = 0;

        virtual std::string getOutputPipeline(unsigned int port);

        // Can be overriden by subclasses to change how streams actually work
        // instead of using the standard gstreamer method implemented here
        // These are called by the functions named similary without "do"
        // The non-"do" functions handle the mutex
        virtual bool doStartStreamH264(unsigned int port, unsigned int bitrate, 
                std::string profile, std::string level);
        virtual bool doStartStreamJpeg(unsigned int port, unsigned int quality);
        virtual bool doStartStream(unsigned int port, std::string pipeline);
        virtual void doStopStream();

        // Used in base class doStartStream... implementations
        // since pipeline construction only varies by encoder
        std::string makeStandardPipeline(unsigned int port, std::string encPl);

        bool gstHasElement(std::string elementName);
        std::string getVideoConvertElement();
        std::string getH264EncodeElement(unsigned int bitrate, std::string profile, std::string level);
        std::string getJpegEncodeElement(unsigned int quality);
        std::string getH264DecodeElement();
        std::string getJpegDecodeElement();

        // Backend-specific Device ID
        std::string id;

        // These settings are default as they should work for most cameras
        std::string capFormat = "raw";
        std::string capWidth = "640";
        std::string capHeight = "480";
        std::string capFramerate = "30/1";

        // Called when frame read from this camera
        std::function<void(cv::Mat)> *frameCallback = nullptr;

        // HW accel settings
        bool hwencode = true;
        bool hwdecode = true;
        bool hwconvert = true;

        // Stream management
        std::mutex mutex;
        bool isStreaming = false;
        bool runStream;
        std::unique_ptr<cv::VideoCapture> cap;
        std::unique_ptr<std::thread> thread;

        // This exists because v4l2m2m encoders seem to just not work with gstreamer's rtspclientsink
        // No idea why, but it seems to just result in a second or two of running pipeline before
        // something hangs. ffmpeg can publish just fine though, so using ffmpeg to publish as a
        // messy workaround. I don't like this, but it works...
        // See: https://forums.raspberrypi.com/viewtopic.php?p=2201264
        // In my testing I have replicated this with various pipelines. Sometimes it is "fixed" by
        // inserting v4l2convert, rawvideoparse, etc in the right spot, but it doesn't seem to be 
        // consistent or reliable across different cameras or input backend.
        //
        // Alternative would be to use tcpserversink instead. This was experimented with, but
        // allowing multiple clients to play the same stream is somewhat important. Also using
        // stream names instead of port numbers is a little more friendly for users
        //
        // Since gstreamer pipeline is run through opencv, we can't just use stdout/stdin to pipe
        // from gstreamer to ffmpeg. Thus a named pipe is used
        std::unique_ptr<boost::process::child> ffmpegProc;
        std::string ffmpegFifo;
    };
}