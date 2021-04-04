#pragma once

#include <arpirobot/arduino/device/ArduinoDevice.hpp>
#include <arpirobot/arduino/iface/BaseArduinoInterface.hpp>

namespace arpirobot{
    /**
     * \class SingleEncoder sensor.hpp arpirobot/arduino/sensor.hpp
     * 
     * Single-channel encoder
     */
    class SingleEncoder : public ArduinoDevice{
    public:

        /**
         * @param pin The digital pin number this encoder is connected to
         * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        SingleEncoder(int pin, bool useInternalPullup, bool createDevice = true, int deviceId = -1);

        /**
         * @param pin The pin number this encoder is connected to (prefix with letter A for analog pin)
         * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        SingleEncoder(std::string pin, bool useInternalPullup, bool createDevice = true, int deviceId = -1);

        /**
         * Get the position (tick count) for this encoder
         * @return The position in ticks
         */
        int getPosition();

        /**
         * Set the current tick count to the given value
         * @param currentPosition The new tick value to set
         */
        void setPosition(int currentPosition);

        /**
         * Get the human-readable name for this device
         * @return The device name
         */
        std::string getDeviceName();
    
    protected:
        void applyDefaultState();
        std::vector<uint8_t> getCreateData();
        void handleData(const std::vector<uint8_t> &data);
    
    private:
        std::string pin;
        uint8_t useInternalPullup;
        int count = 0;
        int countOffset = 0;
    };

}
