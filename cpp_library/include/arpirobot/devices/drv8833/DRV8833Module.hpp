#pragma once

#include <arpirobot/devices/drv8833/DRV8833Motor.hpp>

namespace arpirobot{
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