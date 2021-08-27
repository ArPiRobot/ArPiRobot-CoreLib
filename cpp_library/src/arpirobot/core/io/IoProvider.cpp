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

#include <arpirobot/core/io/IoProvider.hpp>
#include <arpirobot/core/io/IoDevice.hpp>
#include <arpirobot/core/io/PiGpioIoProvider.hpp>
#include <arpirobot/core/io/DummyIoProvider.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <stdexcept>
#include <algorithm>

using namespace arpirobot;


std::vector<IoDevice*> IoProvider::ioDevices;
std::mutex IoProvider::ioDevicesLock;
IoProvider *IoProvider::instance = nullptr;

const char *IoProvider::PROVIDER_PIGPIO = "pigpio";
const char *IoProvider::PROVIDER_DUMMY = "dummy";


IoProvider::IoProvider(){

}

void IoProvider::init(std::string provider){
    // Choose a default provider based on the current platform if none is specified
    if(provider == ""){
#if defined(__linux__) && defined(__arm__)
        provider = "pigpio";
#else
        provider = "dummy";
#endif
    }

    // Terminate old provider first
    IoProvider::terminate();

    // Instantiate the requested provider
    if(provider == PROVIDER_PIGPIO){
        //IoProvider::instance = new PigpioIoProvider();
    }else if(provider == PROVIDER_DUMMY){
        IoProvider::instance = new DummyIoProvider();
    }else{
        throw std::runtime_error("The IO provider '" + provider + "' is unknown.");
    }
}
        
void IoProvider::terminate(){
    if(IoProvider::instance != nullptr){

        ioDevicesLock.lock();
        for(IoDevice *dev : IoProvider::ioDevices){
            dev->close();
        }
        ioDevices.clear();
        ioDevicesLock.unlock();

        delete IoProvider::instance;
        IoProvider::instance = nullptr;
    }
}

void IoProvider::addDevice(IoDevice *device){
    ioDevicesLock.lock();
    if(std::find(ioDevices.begin(), ioDevices.end(), device) == ioDevices.end()){
        ioDevices.push_back(device);
    }
    ioDevicesLock.unlock();
}

void IoProvider::removeDevice(IoDevice *device){
    ioDevicesLock.lock();
    auto it = std::find(ioDevices.begin(), ioDevices.end(), device);
    if(it != ioDevices.end()){
        ioDevices.erase(it);
    }
    ioDevicesLock.unlock();
}
