#pragma once

#include <arpirobot/core/device/MotorController.hpp>
#include <arpirobot/core/drive/BaseAxisTransform.hpp>
#include <vector>
#include <memory>
#include <array>

namespace arpirobot{
    /**
     * \class CubicAxisTransform CubicAxisTransform.hpp arpirobot/core/drive/CubicAxisTransform.hpp
     * 
     * Transform that applies a cubic function to the magnitude of the input.
     * The cubic function is fit to the data set (0, minPower), (0.5, midPower), (1, 1)
     * This gives a wide range of inputs that will yield a value near midPower
     * Sign of the output will match the sign of the input
     */
    class CubicAxisTransform : public BaseAxisTransform {
    public:
        /**
         * @param minPower The minimum output of this cubic function (leave as zero if unsure)
         * @param midPower The "middle" power where the "flat" part of the function should be positioned
         */
        CubicAxisTransform(double minPower, double midPower);

        double applyTransform(double rawAxisValue);

    private:
        // a*(x^3) + b*(x^2) + c*x + d
        double a, b, c, d;

        static std::vector<double> polyfit(std::vector<double> x, std::vector<double> y, int order);
    };
}
