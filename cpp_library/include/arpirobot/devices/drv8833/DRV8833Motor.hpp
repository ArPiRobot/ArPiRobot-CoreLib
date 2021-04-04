#pragma once

#include <arpirobot/core/device/MotorController.hpp>

namespace arpirobot{

    /**
     * \class DRV8833Motor DRV8833Motor.hpp arpirobot/devices/drv8833/DRV8833Motor.hpp
     * 
     * Motor on DRV8833 module
     */
    class DRV8833Motor : public MotorController{
    public:

        /**
         * @param in1Pin pin for side 1 directional control
         * @param in2Pin pin for side 2 directional control
         * @param slpPin pin for sleep of module
         */
        DRV8833Motor(int in1Pin, int in2Pin, int slpPin);

        ~DRV8833Motor();

        /**
         * Get the human-readable name of the device
         * @return the devices human readable name
         */
        std::string getDeviceName();

    protected:
        void begin();
        
        void run();

    private:
        int in1, in2, slp;
    };
}
