#include <arpirobot/core/network/ControllerData.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <cmath>
#include <sstream>
#include <iomanip>


using namespace arpirobot;
using namespace std::placeholders;


////////////////////////////////////////////////////////////////////////////////
/// ControllerData
////////////////////////////////////////////////////////////////////////////////

ControllerData::ControllerData(std::vector<uint8_t> &data){
    updateData(data);
}

void ControllerData::updateData(std::vector<uint8_t> &data){
    // Lock used in case of axis, button, dpad count change
    // It is possible to access data between count vars being updated and 
    //
    std::lock_guard<std::mutex> l(lock);
    controllerNumber = data[0];
    axisCount = data[1];
    buttonCount = data[2];
    dpadCount = data[3];

    // Only reallocate if necessary
    if(axes.size() != axisCount){
        axes.clear();
        axes.resize(axisCount);
    }
    if(buttons.size() != buttonCount){
        buttons.clear();
        buttons.resize(buttonCount);
    }
    if(dpads.size() != dpadCount){
        dpads.clear();
        dpads.resize(dpadCount);
    }

    // Get axis array (each axis is a signed 16-bit integer, full range)
    int offset = 4;
    for(int i = 0; i < axisCount; ++i){
        short tmp = data[offset] << 8 | data[offset + 1]; // signed 16-bit int: high byte, low byte.
        if(tmp <= 0)
            axes[i] = tmp / 32768.0f;
        else if(tmp > 0)
            axes[i] = tmp / 32767.0f;
        offset += 2;
    }

    // Get button array
    // ceil(buttonCount / 8.0) = number of bytes necessary to send the number of buttons
    // go from 1 less than that number of bytes down to zero (inclusive) so index calculation matches DS code
    for(int i = std::ceil(buttonCount / 8.0) - 1; i >= 0; --i){
        uint8_t b = data[4 + (2 * axisCount) + i];
        // Must always process 8 bits (8 right shifts) even if some are ignored
        for(int j = 7; j >= 0; --j){
            if(i * 8 + j < buttonCount){
                buttons[i * 8 + j] = ( (b & 0x01) == 1 );
            }
            // Next bit (next button)
            b >>= 1;
        }
    }

    // Get dpad array
    for(int i = std::ceil(dpadCount / 2.0) - 1; i >= 0; --i){
        uint8_t b = data[4 + (2 * axisCount) + std::ceil(buttonCount / 8.0) + i];
        // Always process upper and lower dpad in byte (4 bits per dpad)
        for(int j = 1; j >= 0; --j){
            if((i * 2 + j) < dpadCount){
                dpads[i * 2 + j] = (b & 0x0F);
            }
            b >>= 4;
        }
    }

    lastData = data;
    lastUpdateTime = std::chrono::steady_clock::now();
}
