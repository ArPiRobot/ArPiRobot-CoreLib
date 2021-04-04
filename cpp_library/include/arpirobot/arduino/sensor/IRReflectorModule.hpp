#pragma once

#include <arpirobot/arduino/device/ArduinoDevice.hpp>
#include <arpirobot/arduino/iface/BaseArduinoInterface.hpp>

namespace arpirobot{
    /**
     * \class IRReflectorModule sensor.hpp arpirobot/arduino/sensor.hpp
     * 
     * Infrared reflection detector module
     */
    class IRReflectorModule : public ArduinoDevice{
    public:
        /**
         * @param digitalPin The digital output pin for this sensor
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        IRReflectorModule(int digitalPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param digitalPin The digital output pin for this sensor (prefix with letter A for analog pin)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        IRReflectorModule(std::string digitalPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param digitalPin The digital output pin for this sensor
         * @param analogPin The analog output pin for this sensor (can be prefixed with letter A, but not required)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        IRReflectorModule(int digitalPin, std::string analogPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param digitalPin The digital output pin for this sensor (prefix with letter A for analog pin)
         * @param analogPin The analog output pin for this sensor (can be prefixed with letter A, but not required)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        IRReflectorModule(std::string digitalPin, std::string analogPin, bool createDevice = true, int deviceId = -1);

        /**
         * Get the digital value read from the sensor
         * @return true or false. One cooresponds to reflection detected. The other is no reflection detected.
         */
        bool getDigitalValue();

        /**
         * Get the analog reflection signal strength (if this device was setup to use an analog input)
         * @return The read analog value (will be zero if not configured for analog data)
         */
        int getAnalogValue();

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
        std::string digitalPin;
        std::string analogPin;
        bool digitalValue = false;
        int analogValue = 0;
    };
}