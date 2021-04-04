#pragma once

#include <arpirobot/core/device/MotorController.hpp>
#include <vector>
#include <memory>
#include <array>

namespace arpirobot{
    /**
     * \class BaseAxisTransform BaseAxisTransform.hpp arpirobot/core/drive/BaseAxisTransform.hpp
     * 
     * Base class for axis transforms. Axis transforms take an analog axis value (-1.0 to 1.0)
     * and apply a fixed calculation to modify the curve of the axis.
     * 
     * Axis transforms are most useful with drive control.
     */
    class BaseAxisTransform{
    public:
        virtual ~BaseAxisTransform(){}

        /**
         * Axis transforms should override this function to implement specific calculations.
         */
        virtual double applyTransform(double rawAxisValue) = 0;
    };
}
