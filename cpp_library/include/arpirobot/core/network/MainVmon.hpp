#pragma once

namespace arpirobot{

    /**
     * \class MainVmon MainVmon.hpp arpirobot/core/network/MainVmon.hpp
     * 
     * Main voltage monitor interface
     * Any sensor (arduino or pi device) can implement this interface to be able to be used as a main vmon
     */
    class MainVmon{
    public:

        /**
         * Make this device the main voltage monitor
         */
        void makeMainVmon();

    protected:
        /**
         * Returns true if this is currently the main vmon
         */
        bool isMainVmon();

        /**
         * Send the main battery voltage to the connected DS (if this is the main vmon)
         */
        void sendMainBatteryVoltage(double voltage);
    };

}