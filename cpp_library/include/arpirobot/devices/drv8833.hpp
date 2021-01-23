#pragma once

#include <arpirobot/core/device.hpp>

namespace arpirobot{

    /**
     * \class DRV8833Motor drv8833.hpp arpirobot/devices/drv8833.hpp
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
         * @returns the devices human readable name
         */
        std::string getDeviceName();

    protected:
        void begin();
        
        void run();

    private:
        int in1, in2, slp;
    };

    /**
     * \class DRV8833Module drv8833.hpp arpirobot/devices/drv8833.hpp
     * 
     * DRV8833 Module (two motors)
     */
    class DRV8833Module{
    public:

        /**
         * @param aIn1Pin In1 pin for motor a of module
         * @param aIn2Pin In2 pin for motor a of module
         * @param bIn1Pin In1 pin for motor b of module
         * @param bIn2Pin In2 pin for motor b of module
         * @param slpPin Sleep pin for module
         */
        DRV8833Module(int aIn1Pin, int aIn2Pin, int bIn1Pin, int bIn2Pin, int slpPin);

        /**
         * Get the Motor instance for motor a
         */
        DRV8833Motor &getMotorA();

        /**
         * Get the Motor instance for motor b
         */
        DRV8833Motor &getMotorB();

    private:
        DRV8833Motor motorA, motorB;
    };
    
}
