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
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/log/Logger.hpp>

using namespace arpirobot;


////////////////////////////////////////////////////////////////////////////////
/// AudioManager Static Members
////////////////////////////////////////////////////////////////////////////////
bool AudioManager::initDone = false;
ma_context AudioManager::context;
ma_device_info *AudioManager::playbackDeviceInfos;
ma_uint32 AudioManager::playbackDeviceCount;
ma_device_info *AudioManager::captureDeviceInfos;
ma_uint32 AudioManager::captureDeviceCount;

std::unordered_map<ma_device*, AudioManager::PlaybackSetup*> AudioManager::playbackSetups;
std::unordered_map<int, ma_device*> AudioManager::jobIdMap;

std::mutex AudioManager::lock;


////////////////////////////////////////////////////////////////////////////////
/// AudioManager Functions
////////////////////////////////////////////////////////////////////////////////

std::vector<AudioDeviceInfo> AudioManager::getPlaybackDevices(){
    if(initDone){
        std::vector<AudioDeviceInfo> infos;
        for(ma_uint32 i = 0; i < playbackDeviceCount; ++i){
            AudioDeviceInfo inf;
            inf.id = i; // This id is the index in the playbackDeviceInfos array
            inf.name = std::string(playbackDeviceInfos[i].name);
            inf.isDefault = playbackDeviceInfos[i].isDefault;
            inf.type = AudioDeviceInfo::TYPE_PLAYBACK;
            infos.push_back(inf);
        }
        return infos;
    }
    return {};
}


int AudioManager::playSound(std::string file){
    for(ma_uint32 i = 0; i < playbackDeviceCount; ++i){
        if(playbackDeviceInfos[i].isDefault){
            AudioDeviceInfo info;
            info.id = i;
            info.name = std::string(playbackDeviceInfos[i].name);
            info.isDefault = true;
            info.type = AudioDeviceInfo::TYPE_PLAYBACK;
            return AudioManager::playSound(file, info);
        }
    }
    Logger::logWarningFrom("AudioManager", "Attempted to play sound with default audio device, but none exists.");
    return false;
}

int AudioManager::playSound(std::string file, AudioDeviceInfo info){

    // This function modifies playbackSetups and jobIdMap. Acquire lock
    std::lock_guard<std::mutex> lg(lock);

    PlaybackSetup *setup = new PlaybackSetup();
    setup->jobId = nextJobId();

    // Store this so it can be deleted once playback is done
    playbackSetups[&setup->device] = setup;

    // Init the decoder for the given file
    if(ma_decoder_init_file(file.c_str(), NULL, &setup->decoder) != MA_SUCCESS){
        Logger::logWarningFrom("AudioManager", "Cannot find decoder for file " + file);
        playbackSetups.erase(&setup->device);
        delete setup;
        return -1;
    }

    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.pDeviceID = &playbackDeviceInfos[info.id].id;
    deviceConfig.playback.format = setup->decoder.outputFormat;
    deviceConfig.playback.channels = setup->decoder.outputChannels;
    deviceConfig.sampleRate = setup->decoder.outputSampleRate;
    deviceConfig.dataCallback = &AudioManager::playbackDataCallback;

    ma_result res = ma_device_init(NULL, &deviceConfig, &setup->device);
    if(res != MA_SUCCESS){
        Logger::logWarningFrom("AudioManager", "Unable to init device for playback.");
        Logger::logDebugFrom("AudioManager", "Error code: " + std::to_string(res));
        ma_decoder_uninit(&setup->decoder);
        playbackSetups.erase(&setup->device);
        delete  setup;
        return -1;
    }

    res = ma_device_start(&setup->device);
    if(res != MA_SUCCESS){
        Logger::logWarningFrom("AudioManager", "Unable to start device for playback.");
        Logger::logDebugFrom("AudioManager", "Error code: " + std::to_string(res));
        ma_device_uninit(&setup->device);
        ma_decoder_uninit(&setup->decoder);
        playbackSetups.erase(&setup->device);
        delete setup;
        return -1;
    }

    // Playback was started successfully.
    jobIdMap[setup->jobId] = &setup->device;
    return setup->jobId;
}

void AudioManager::stopJob(int jobId){
    // This function modifies playbackSetups and jobIdMap. Acquire lock
    std::lock_guard<std::mutex> lg(lock);

    if(jobIdMap.find(jobId) != jobIdMap.end()){
        PlaybackSetup *setup = playbackSetups[jobIdMap[jobId]];
        ma_device_stop(&setup->device);
        ma_device_uninit(&setup->device);
        ma_decoder_uninit(&setup->decoder);
        jobIdMap.erase(jobId);
        playbackSetups.erase(&setup->device);
        delete setup;
    }
}

void AudioManager::playbackDataCallback(ma_device *device, void *output, const void *input, ma_uint32 frameCount){
    // Do not continue if no decoder for this device exists. This should never happen...
    if(playbackSetups.find(device) == playbackSetups.end()){
        Logger::logWarningFrom("AudioManager", "Data provided with no decoder. This should never happen...");
        return;
    }

    // Handle data
    ma_uint32 framesRead = ma_decoder_read_pcm_frames(&playbackSetups[device]->decoder, output, frameCount);
    
    // Fewer frames read than requested. Have reached the end.
    if(framesRead < frameCount){
        // Can't call stopJob directly. This would cause a deadlock (miniaudio internal mutex)
        // Instead run this "soon" on a different thread in the robot's scheduler
        int jobId = playbackSetups[device]->jobId;
        BaseRobot::scheduleOneshotFunction([jobId](){
            stopJob(jobId);
        });
    }
}

int AudioManager::nextJobId(){
    for(int i = 0; i < INT_MAX; ++i){
        if(jobIdMap.find(i) == jobIdMap.end())
            return i;
    }
    return -1;
}

void AudioManager::init(){
    if(!initDone){
        // Init library
        ma_result res = ma_context_init(NULL, 0, NULL, &context);
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
    // TODO: Close anything that needs to be closed
}
