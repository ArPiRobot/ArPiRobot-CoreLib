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
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include <gst/gst.h>
#include <boost/process.hpp>


namespace arpirobot{
    /**
     * \class BaseCamera BaseCamera.hpp arpirobot/core/camera/BaseCamera.hpp
     * 
     * Common base class for various camera types
     * Provides functionality to run camera streams and receive frames from
     * cameras in OpenCV compatible formats
     */
    class BaseCamera{
    public:
        BaseCamera(std::string id);
        virtual ~BaseCamera();

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
        void setFrameCallback(std::function<void(cv::Mat&)> frameCallback);

        /**
         * Start a H264 stream. Note that only one stream may be running on a
         * camera at a time.
         * @param key Name of the stream (must be unique)
         * @param bitrate Target bitrate in kbps (default 2048)
         * @param profile H.264 profile to use (default baseline)
         * @param level H.264 level to use (default empty string = auto)
         * @return true on success, else false (could be pipeline failure, or a stream may already be running)
         */
        bool startStreamH264(std::string key, unsigned int bitrate = 2048, 
                std::string profile = "baseline", std::string level = "");

        /**
         * Start a (M)JPEG stream. Note that only one stream may be running on a
         * camera at a time.
         * @param key Name of the stream (must be unique)
         * @param quality Quality of JPEG images (0-100, default 80). Lower quality requires less bandwidth
         * @return true on success, else false (could be pipeline failure, or a stream may already be running)
         */
        bool startStreamJpeg(std::string key, unsigned int quality = 80);

        /**
         * Stop the running stream
         */
        void stopStream();
    
        /**
         * Get name of this camera's backend
         */
        virtual std::string getBackend() = 0;

    protected:
        // Backend specific
        virtual std::string getDeviceName() = 0;
        virtual std::string getCapturePipeline() = 0;

        virtual std::string getOutputPipeline(std::string key);
        std::string makeStandardPipeline(std::string key, std::string encPl);

        // Can be overriden by subclasses to change how streams actually work
        // instead of using the standard gstreamer method implemented here
        // These are called by the functions named similary without "do"
        // The non-"do" functions handle the mutex
        virtual bool doStartStreamH264(std::string key, unsigned int bitrate, 
                std::string profile, std::string level);
        virtual bool doStartStreamJpeg(std::string key, unsigned int quality);
        virtual bool doStartStream(std::string key, std::string pipeline);
        virtual void doStopStream();

        // Callbacks shared for all gstreamer pipline instances
        static GstFlowReturn frameFromAppsink(GstElement *sink, BaseCamera *instance);
        static GstBusSyncReply messageFromPipeline(GstBus *bus, GstMessage *msg, gpointer userData);

        // Internally used to choose which element to use for a task based on hw... vars
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
        std::function<void(cv::Mat&)> frameCallback = nullptr;

        // HW accel settings
        bool hwencode = true;
        bool hwdecode = true;
        bool hwconvert = true;

        // Stream management must be thread safe
        std::mutex managementMutex;
        bool isStreaming = false;

        // Used for running stream
        std::string ffmpegFifo;
        std::unique_ptr<boost::process::child> ffmpegProc = nullptr;
        GstElement *gstPl = NULL;
        GstElement *gstAppsink = NULL;
        GstBus *gstBus = NULL;
    };
}