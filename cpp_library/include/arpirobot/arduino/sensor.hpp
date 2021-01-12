#pragma once

#include <arpirobot/arduino/device.hpp>
#include <arpirobot/arduino/iface.hpp>

namespace arpirobot{
    class VoltageMonitor : public ArduinoDevice{
    public:
        VoltageMonitor(std::string pin, double vboard, int r1, int r2, bool createDevice = true, int deviceId = -1);
        VoltageMonitor(int pin, double vboard, int r1, int r2, bool createDevice = true, int deviceId = -1);

        double getVoltage();

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

    class Ultrasonic4Pin : public ArduinoDevice{
    public:
        Ultrasonic4Pin(int triggerPin, int echoPin, bool createDevice = true, int deviceId = -1);
        Ultrasonic4Pin(int triggerPin, std::string echoPin, bool createDevice = true, int deviceId = -1);
        Ultrasonic4Pin(std::string triggerPin, int echoPin, bool createDevice = true, int deviceId = -1);
        Ultrasonic4Pin(std::string triggerPin, std::string echoPin, bool createDevice = true, int deviceId = -1);

        int getDistance();

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