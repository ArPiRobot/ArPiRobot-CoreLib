/*
 * Copyright 2021 Marcus Behel
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

#include <miniaudio.h>

#include <vector>
#include <unordered_map>
#include <mutex>

#include <arpirobot/core/audio/AudioDeviceInfo.hpp>

namespace arpirobot{
    class AudioManager{
    public:
        static std::vector<AudioDeviceInfo> getPlaybackDevices();
        static std::vector<AudioDeviceInfo> getCaptureDevices();

        static bool playSound(std::string file);
        static bool playSound(std::string file, AudioDeviceInfo info);
    
    private:

        class Device{
        public:
            ma_decoder decoder;
            ma_device_config deviceConfig;
            ma_device device;
        };

        static void playbackDataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount);
        static void playbackStopCallback(ma_device *device);

        static void init();
        static void finish();

        static bool initDone;
        static ma_context context;
        static ma_device_info *playbackDeviceInfos;
        static ma_uint32 playbackDeviceCount;
        static ma_device_info *captureDeviceInfos;
        static ma_uint32 captureDeviceCount;
        static std::mutex amMutex;
        
        // Map IDs to devices that have been inited.
        // Since IDs are related to indices in playbackDeviceInfos and captureDeviceInfos, 
        //      but are in the same map here, they need to be unique.
        // To handle this, capture device IDs start after playback IDs (capture device at 
        //      index 0 has id playbackDeviceCount)
        static std::unordered_map<int, Device> usedDevices;

        friend class BaseRobot;
    };
}