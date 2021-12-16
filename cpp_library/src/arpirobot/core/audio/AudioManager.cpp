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

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <arpirobot/core/audio/AudioManager.hpp>
#include <arpirobot/core/log/Logger.hpp>

using namespace arpirobot;

// Static members
bool AudioManager::initDone = false;
ma_context AudioManager::context;
ma_device_info *AudioManager::playbackDeviceInfos = nullptr;
ma_uint32 AudioManager::playbackDeviceCount = 0;
ma_device_info *AudioManager::captureDeviceInfos = nullptr;
ma_uint32 AudioManager::captureDeviceCount = 0;
std::unordered_map<int, AudioManager::Device> AudioManager::usedDevices;
std::mutex AudioManager::amMutex;


std::vector<AudioDeviceInfo> AudioManager::getPlaybackDevices(){
    std::lock_guard<std::mutex> lock(amMutex);
    if(initDone){
        std::vector<AudioDeviceInfo> infos;
        for(ma_uint32 i = 0; i < playbackDeviceCount; ++i){
            AudioDeviceInfo inf;
            inf.id = i; // Playback device IDs start at zero
            inf.name = std::string(playbackDeviceInfos[i].name);
            inf.isDefault = playbackDeviceInfos[i].isDefault;
            infos.push_back(inf);
        }
        return infos;
    }
    return {};
}

std::vector<AudioDeviceInfo> AudioManager::getCaptureDevices(){
    std::lock_guard<std::mutex> lock(amMutex);
    if(initDone){
        std::vector<AudioDeviceInfo> infos;
        for(ma_uint32 i = 0; i < captureDeviceCount; ++i){
            AudioDeviceInfo inf;
            inf.id = i + playbackDeviceCount; // Capture device IDs start after playback
            inf.name = std::string(captureDeviceInfos[i].name);
            inf.isDefault = captureDeviceInfos[i].isDefault;
            infos.push_back(inf);
        }
        return infos;
    }
    return {};
}

bool AudioManager::playSound(std::string file){
    AudioDeviceInfo info;
    {
        std::lock_guard<std::mutex> lock(amMutex);
        ma_int64 idx = -1;
        for(ma_uint32 i = 0; i < playbackDeviceCount; ++i){
            if(playbackDeviceInfos[i].isDefault){
                idx = i;
                break;
            }
        }
        if(idx == -1){
            Logger::logWarningFrom("AudioManager", "Attempted to play sound with default audio device, but none exists.");
            return false;
        }
        info.id = idx; // ID for playback devices matches index
        info.name = playbackDeviceInfos[idx].name;
        info.isDefault = true;
    }
    return playSound(file, info);
}

bool AudioManager::playSound(std::string file, AudioDeviceInfo info){
    {
        std::lock_guard<std::mutex> lock(amMutex);
        if(usedDevices.find(info.id) == usedDevices.end()){
            // Add a "Device" object cooresponding to the id of the AudioDeviceInfo object
            usedDevices[info.id] = Device();
        }
        
        // This device object is a container for a ma_device, ma_decoder, and ma_
        Device &dev = usedDevices[info.id];

        Logger::logDebugFrom("playSound2", "Point A");
        Logger::logDebugFrom("playSound2", "dev: " + std::to_string((long int)(&dev)));
        Logger::logDebugFrom("playSound2", "dev.device: " + std::to_string((long int)(&dev.device)));

        // Ensure device is stopped
        // THIS LINE CAUSES FREEZE!!!
        ma_device_stop(&dev.device);

        Logger::logDebugFrom("playSound2", "Point B");

        // Ensure device is not inited so the device can be re-inited with a new config
        ma_device_uninit(&dev.device);

        // Ensure proper cleanup of decoder
        ma_decoder_uninit(&dev.decoder);

        // Create decoder for the given file and init device
        ma_result res = ma_decoder_init_file(file.c_str(), NULL, &dev.decoder);
        if(res != MA_SUCCESS){
            Logger::logWarningFrom("AudioManager", "Failed to find decoder for audio file " + file);
            return false;
        }

        dev.deviceConfig = ma_device_config_init(ma_device_type_playback);
        dev.deviceConfig.playback.pDeviceID = 
                &playbackDeviceInfos[info.id].id; // ID for playback device matches index in infos array
        dev.deviceConfig.playback.format = dev.decoder.outputFormat;
        dev.deviceConfig.playback.channels = dev.decoder.outputChannels;
        dev.deviceConfig.sampleRate = dev.decoder.outputSampleRate;
        dev.deviceConfig.dataCallback = &AudioManager::playbackDataCallback;
        dev.deviceConfig.stopCallback = &AudioManager::playbackStopCallback;
        dev.deviceConfig.pUserData = &dev.decoder;

        if(ma_device_init(&context, &dev.deviceConfig, &dev.device) != MA_SUCCESS){
            Logger::logWarningFrom("AudioManager", "Failed to init device while playing audio.");
            ma_decoder_uninit(&dev.decoder);
            return false;
        }

        if(ma_device_start(&dev.device) != MA_SUCCESS){
            Logger::logWarningFrom("AudioManager", "Failed to start playback device.");
            ma_device_uninit(&dev.device);
            ma_decoder_uninit(&dev.decoder);
            return false;
        }
        return true;
    }
}

void AudioManager::playbackDataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount){
    // Do not use ActionManager mutex here. This is mutexed in the audio libary.
    ma_decoder* decoder = (ma_decoder*)device->pUserData;
    if (decoder == NULL) {
        return;
    }
    ma_decoder_read_pcm_frames(decoder, output, frameCount);
}

void AudioManager::playbackStopCallback(ma_device *device){
    // TODO: Use this to have a way to inform user-code that audio has finished playing
}

void AudioManager::init(){
    std::lock_guard<std::mutex> lock(amMutex);
    if(!initDone){
        ma_result res;

        // Init library
        res = ma_context_init(NULL, 0, NULL, &context);
        if(res != MA_SUCCESS){
            Logger::logWarningFrom("AudioManager", "Failed to initialize audio library. Error code " + std::to_string(res));
            return;
        }

        // Enumerate devices (also ensures program has permission to access devices)
        res = ma_context_get_devices(&context, &playbackDeviceInfos, &playbackDeviceCount, 
                &captureDeviceInfos, &captureDeviceCount);
        if(res != MA_SUCCESS){
            Logger::logWarningFrom("AudioManager", "Failed to enumerate devices. Error code " + std::to_string(res));
        }
    }
    initDone = true;
}

void AudioManager::finish(){
    std::lock_guard<std::mutex> lock(amMutex);
    if(initDone){
        for(auto &it : usedDevices){
            ma_device_uninit(&it.second.device);
        }
        ma_context_uninit(&context);
    }
    initDone = false;
}
