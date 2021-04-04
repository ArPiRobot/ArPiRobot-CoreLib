#pragma once

#include <arpirobot/arduino/device/ArduinoDevice.hpp>
#include <arpirobot/arduino/iface/BaseArduinoInterface.hpp>
#include <arpirobot/core/network/MainVmon.hpp>

namespace arpirobot{
    /**
     * \class VoltageMonitor sensor.hpp arpirobot/arduino/sensor.hpp
     * 
     * Simple analog input voltage monitor (using voltage divider) connected to arduino analog input.
     */
    class VoltageMonitor : public ArduinoDevice, public MainVmon{
    public:

        /**
         * @param pin The analog input pin this voltage monitor is connected to (can be prefixed with letter A)
         * @param vboard The board voltage for this arduino (usually 5V or 3.3V).
         * @param r1 The top resistor of the voltage divider
         * @param r2 The bottom resistor of the voltage divider (the one voltage is measured across)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        VoltageMonitor(std::string pin, double vboard, int r1, int r2, bool createDevice = true, int deviceId = -1);

        /**
         * @param pin The analog input pin number this voltage monitor is connected
         * @param vboard The board voltage for this arduino (usually 5V or 3.3V).
         * @param r1 The top resistor of the voltage divider
         * @param r2 The bottom resistor of the voltage divider (the one voltage is measured across)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        VoltageMonitor(int pin, double vboard, int r1, int r2, bool createDevice = true, int deviceId = -1);

        /**
         * Get the voltage read by this voltage monitor (in volts)
         * @return The voltage in volts
         */
        double getVoltage();

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
        double vboard;
        int r1;
        int r2;
        double voltage = 0;
    };

}
