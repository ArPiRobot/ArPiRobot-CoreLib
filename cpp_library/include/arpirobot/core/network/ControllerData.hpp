#pragma once

#include <mutex>
#include <vector>

namespace arpirobot{

    ////////////////////////////////////////////////////////////////////////////
    /// ControllerData
    ////////////////////////////////////////////////////////////////////////////

    /**
     * \class ControllerData ControllerData.hpp arpirobot/core/network/ControllerData.hpp
     * 
     * Class to hold and data for a single controller and parse data received from network
     */
    class ControllerData{
    public:

        /**
         * @param data The raw controller data from network
         */
        ControllerData(std::vector<uint8_t> &data);

        /**
         * Update the controller data from raw network data
         * @param data Raw controller data from network
         */
        void updateData(std::vector<uint8_t> &data);

        int controllerNumber = -1;
        int axisCount = -1;
        int buttonCount = -1;
        int dpadCount = -1;
        std::vector<float> axes;
        std::vector<bool> buttons;
        std::vector<int> dpads;
        std::vector<uint8_t> lastData;
        std::chrono::steady_clock::time_point lastUpdateTime;
        std::mutex lock;
    };

}