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
    /**
     * \class AudioManager AudioManager.hpp arpirobot/core/audio/AudioManager.hpp
     * 
     * Helper class to play audio files via audio devices on the robot
     */
    class AudioManager{
    public:

        /**
         * Get all available playback devices on this system
         * @return vector of AudioDeviceInfo objects with information about each available device
         */
        static std::vector<AudioDeviceInfo> getPlaybackDevices();

        /**
         * Play a sound file on the default audio device
         * @param file Full path to the audio file to play
         * @return A job ID for the sound being played
         */
        static int playSound(std::string file);

        /**
         * Play a sound file on a specific audio device
         * @param file Full path to the audio file to play
         * @param info AudioDeviceInfo object for the device to play the file on (as obtained from getPlaybackDevices())
         * @return A job ID for the sound being played
         */
        static int playSound(std::string file, AudioDeviceInfo info);

        /**
         * Stop playing a certain sound using it's job ID (as obtained from playSound())
         * If the job has already finished, this function will do nothing
         * @param jobId job ID for the sound to stop as obtained from playSound()
         */
        static void stopJob(int jobId);
    
    private:
        class PlaybackSetup{
        public:
            int jobId;
            ma_device device;
            ma_decoder decoder;
            ma_device_config deviceConfig;
        };

        static void playbackDataCallback(ma_device *device, void *output, const void *input, ma_uint32 frameCount);
        static int nextJobId();

        static void init();
        static void finish();

        static bool initDone;
        static ma_context context;
        static ma_device_info *playbackDeviceInfos;
        static ma_uint32 playbackDeviceCount;
        static ma_device_info *captureDeviceInfos;
        static ma_uint32 captureDeviceCount;

        static std::unordered_map<ma_device*, PlaybackSetup*> playbackSetups;
        static std::unordered_map<int, ma_device*> jobIdMap;

        static std::mutex lock;

        static AudioDeviceInfo defaultPlaybackDeviceInfo;
        
        friend class BaseRobot;
    };
}