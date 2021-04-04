
#include <arpirobot/core/drive/SquareRootAxisTransform.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <cmath>

using namespace arpirobot;


double SquareRootAxisTransform::applyTransform(double rawAxisValue){
    double y = std::abs(rawAxisValue);
    if(rawAxisValue < 0)
        return -1 * y;
    return y;
}