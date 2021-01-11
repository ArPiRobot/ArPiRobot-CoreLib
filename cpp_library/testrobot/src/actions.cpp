#include <actions.hpp>
#include <main.hpp>

#include <arpirobot/core/log.hpp>

void JSDriveAction::begin(){
    lockDevices({
        &Main::robot->flmotor,
        &Main::robot->frmotor,
        &Main::robot->rlmotor,
        &Main::robot->rrmotor
    });
}

void JSDriveAction::process(){
    double speed = Main::robot->gp0.getAxis(1, 0.1) * -1;
    double rotation = Main::robot->gp0.getAxis(2, 0.1);
    Main::robot->driveHelper.update(speed, rotation);
}

void JSDriveAction::finish(bool wasInterrupted){
    Main::robot->driveHelper.update(0, 0);
}

bool JSDriveAction::shouldContinue(){
    return true;
}


DriveTimeAction::DriveTimeAction(int durationMs, double speed) : durationMs(durationMs), speed(speed){

}


void DriveTimeAction::begin(){
    lockDevices({
        &Main::robot->flmotor,
        &Main::robot->frmotor,
        &Main::robot->rlmotor,
        &Main::robot->rrmotor
    });

    startTime = std::chrono::steady_clock::now();

    Main::robot->driveHelper.update(speed, 0);
}

void DriveTimeAction::process(){

}

void DriveTimeAction::finish(bool wasInterrupted){
    Main::robot->driveHelper.update(0, 0);
}

bool DriveTimeAction::shouldContinue(){
    int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - startTime).count();
    return elapsed <= durationMs;
}
