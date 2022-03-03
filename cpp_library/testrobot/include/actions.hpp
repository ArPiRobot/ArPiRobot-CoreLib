#pragma once

#include <arpirobot/core/action/Action.hpp>

#include <chrono>

using namespace arpirobot;


class TestAction : public Action {
protected:
    void begin() override;
    void process() override;
    void finish(bool wasInterrupted) override;
    bool shouldContinue() override;
};
