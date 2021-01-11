#pragma once

#include <arpirobot/arduino/iface.hpp>

#include <string>
#include <vector>

namespace arpirobot{
    class ArduinoDevice{
    public:
        ArduinoDevice(bool createDevice, int deviceId);
    
        virtual ~ArduinoDevice();

        virtual std::string getDeviceName() = 0;

    protected:
        void setArduino(BaseArduinoInterface *arduino);
        void setDeviceId(int deviceId);

        bool sendData(const std::vector<uint8_t> &data);
        bool sendData(const std::string &data);

        // Device specific
        virtual void applyDefaultState() = 0;
        virtual std::vector<uint8_t> getCreateData() = 0;
        virtual void handleData(const std::vector<uint8_t> &data) = 0;

        bool createDevice;
        int deviceId;
        BaseArduinoInterface *arduino = nullptr;

        friend class BaseArduinoInterface;
    };
}