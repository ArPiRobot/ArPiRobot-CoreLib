#pragma once

#include <arpirobot/arduino/device.hpp>
#include <arpirobot/arduino/iface.hpp>

namespace arpirobot{
    class VoltageMonitor : public ArduinoDevice{
    public:
        VoltageMonitor(std::string pin, double vboard, int r1, int r2, bool createDevice = true, int deviceId = -1);
        VoltageMonitor(int pin, double vboard, int r1, int r2, bool createDevice = true, int deviceId = -1);

        void makeMainVmon();

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