#pragma once

#include <arpirobot/core/action.hpp>

#include <chrono>

using namespace arpirobot;

class JSDriveAction : public Action {
protected:
    void begin();
    void process();
    void finish(bool wasInterrupted);
    bool shouldContinue();
};


class DriveTimeAction : public Action {
public:
    DriveTimeAction(int durationMs, double speed);

protected:
    void begin();
    void process();
    void finish(bool wasInterrupted);
    bool shouldContinue();

private:
    int durationMs;
    double speed;
    std::chrono::steady_clock::time_point startTime;
};
