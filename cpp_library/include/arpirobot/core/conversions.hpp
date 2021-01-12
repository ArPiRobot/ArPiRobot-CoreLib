#pragma once

#include <cstdint>
#include <vector>

namespace arpirobot{

    class Conversions{
    public:
        static void checkBigEndian();
    
        static std::vector<uint8_t> convertInt32ToData(int32_t input, bool littleEndian);

        static int32_t convertDataToInt32(const std::vector<uint8_t> &data, size_t offset, bool littleEndian);

        static std::vector<uint8_t> convertInt16ToData(int16_t input, bool littleEndian);

        static int16_t convertDataToInt16(const std::vector<uint8_t> &data, size_t offset, bool littleEndian);

        static std::vector<uint8_t> convertFloatToData(float input, bool littleEndian);

        static float convertDataToFloat(const std::vector<uint8_t> &data, size_t offset, bool littleEndian);

        static bool isBigEndian;
    };

}