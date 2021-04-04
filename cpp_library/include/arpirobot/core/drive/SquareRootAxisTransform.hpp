#pragma once

#include <arpirobot/core/device/MotorController.hpp>
#include <arpirobot/core/drive/BaseAxisTransform.hpp>
#include <vector>
#include <memory>
#include <array>

namespace arpirobot{
    /**
     * \class SquareRootAxisTransform drive.hpp arpirobot/core/drive.hpp
     * 
     * Transform that applies a square root function to the magnitude of the input.
     * Sign of the output will match the sign of the input
     */
    class SquareRootAxisTransform : public BaseAxisTransform {
    public:
        double applyTransform(double rawAxisValue);
    };
}
