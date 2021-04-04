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

#include <arpirobot/core/conversions.hpp>

using namespace arpirobot;

bool Conversions::isBigEndian = false;

void Conversions::checkBigEndian(){
    // i = 0x0001
    //  On big endian systems this is stored 0x00, 0x01
    // c = pointer to leftmost byte (array)
    // c[0] = leftmost byte. 1 on little endian. 0 on big endian
    uint16_t i = 1;
    uint8_t *c = (uint8_t*)&i;
    isBigEndian = !c[0];
}

std::vector<uint8_t> Conversions::convertInt32ToData(int32_t input, bool littleEndian){
    std::vector<uint8_t> data;
    data.reserve(4);
    if(littleEndian){
        data.push_back(input);
        data.push_back(input >> 8);
        data.push_back(input >> 16);
        data.push_back(input >> 24);
    }else{
        data.push_back(input >> 24);
        data.push_back(input >> 16);
        data.push_back(input >> 8);
        data.push_back(input);
    }
    return data;
}

int32_t Conversions::convertDataToInt32(const std::vector<uint8_t> &data, size_t offset, bool littleEndian){
    if(littleEndian){
        return data[offset] | 
               data[offset + 1] << 8 | 
               data[offset + 2] << 16 |
               data[offset + 3] << 24;
    }else{
        return data[offset] << 24| 
               data[offset + 1] << 16 | 
               data[offset + 2] << 8 |
               data[offset + 3];
    }
}

std::vector<uint8_t> Conversions::convertInt16ToData(int16_t input, bool littleEndian){
    std::vector<uint8_t> data;
    data.reserve(2);
    if(littleEndian){
        data.push_back(input);
        data.push_back(input >> 8);
    }else{
        data.push_back(input >> 8);
        data.push_back(input);
    }
    return data;
}

int16_t Conversions::convertDataToInt16(const std::vector<uint8_t> &data, size_t offset, bool littleEndian){
    if(littleEndian){
        return data[offset] | data[offset + 1] << 8;
    }else{
        return data[offset] << 8 | data[offset + 1];
    }
}

std::vector<uint8_t> Conversions::convertFloatToData(float input, bool littleEndian){
    std::vector<uint8_t> data;
    data.reserve(4);

    // Pointer to leftmost byte of input
    uint8_t *ptr = (uint8_t*)&input;
    
    if(isBigEndian == littleEndian){
        // System endianess and desired endianess different, so reverse order
        data.push_back(ptr[3]);
        data.push_back(ptr[2]);
        data.push_back(ptr[1]);
        data.push_back(ptr[0]);
    }else{
        // System endianess and desired endianess match, so no need to reverse order
        data.push_back(ptr[0]);
        data.push_back(ptr[1]);
        data.push_back(ptr[2]);
        data.push_back(ptr[3]);
    }

    return data;
}

float Conversions::convertDataToFloat(const std::vector<uint8_t> &data, size_t offset, bool littleEndian){
    uint8_t dataRaw[4];
    
    if(isBigEndian == littleEndian){
        // System endianess and desired endianess different, so reverse order
        dataRaw[0] = data[offset + 3];
        dataRaw[1] = data[offset + 2];
        dataRaw[2] = data[offset + 1];
        dataRaw[3] = data[offset];
    }else{
        // System endianess and desired endianess match, so no need to reverse order
        dataRaw[0] = data[offset];
        dataRaw[1] = data[offset + 1];
        dataRaw[2] = data[offset + 2];
        dataRaw[3] = data[offset + 3];
    }
    // Dereference pointer to start of dataRaw interpreted as float
    return *((float*)(dataRaw));
}