#pragma once

#include <arpirobot/core/action.hpp>

using namespace arpirobot;

class DemoAction : public Action {
protected:
    void begin();
    void process();
    void finish(bool wasInterrupted);
    bool shouldContinue();
};