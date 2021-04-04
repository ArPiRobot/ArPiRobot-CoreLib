#pragma once

#include <arpirobot/arduino/device/ArduinoDevice.hpp>
#include <arpirobot/arduino/iface/BaseArduinoInterface.hpp>

namespace arpirobot{
    /**
     * \class Ultrasonic4Pin sensor.hpp arpirobot/arduino/sensor.hpp
     * 
     * Device for 4-pin ultrasonic sensor.
     */
    class Ultrasonic4Pin : public ArduinoDevice{
    public:
        /**
         * @param triggerPin Digital pin number for the trigger pin
         * @param echoPin Digital pin number for the echo pin
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        Ultrasonic4Pin(int triggerPin, int echoPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param triggerPin Digital pin number for the trigger pin
         * @param echoPin Pin number for the echo pin (prefix with letter A for analog pin)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        Ultrasonic4Pin(int triggerPin, std::string echoPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param triggerPin Pin number for the trigger pin (prefix with letter A for analog pin)
         * @param echoPin Digital pin number for the echo pin
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        Ultrasonic4Pin(std::string triggerPin, int echoPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param triggerPin Pin number for the trigger pin (prefix with letter A for analog pin)
         * @param echoPin Pin number for the echo pin (prefix with letter A for analog pin)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        Ultrasonic4Pin(std::string triggerPin, std::string echoPin, bool createDevice = true, int deviceId = -1);

        /**
         * Get the last distance read by the ultrasonic sensor
         * @return The distance in cm
         */
        int getDistance();

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
        std::string triggerPin;
        std::string echoPin;
        int distance = 0;
    };
    
}