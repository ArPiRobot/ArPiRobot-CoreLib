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


#include <arpirobot/core/io/IoDevice.hpp>
#include <arpirobot/core/io/Io.hpp>

#include <stdexcept>

using namespace arpirobot;


IoDevice::IoDevice() {
    Io::addDevice(this);
}

IoDevice::~IoDevice(){
    // Cannot run close() from here as this would run IoDevice::close not the child class's close
    // Must rely on child class destructors to call close()
    Io::removeDevice(this);
}
