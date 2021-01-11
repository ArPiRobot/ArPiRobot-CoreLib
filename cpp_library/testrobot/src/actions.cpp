#include <actions.hpp>
#include <main.hpp>

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
