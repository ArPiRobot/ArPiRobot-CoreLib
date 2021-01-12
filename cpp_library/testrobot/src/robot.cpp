#include <robot.hpp>
#include <arpirobot/core/action.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/devices/gamepad.hpp>
#include <actions.hpp>

void Robot::robotStarted(){
    flmotor.setInverted(true);
    frmotor.setInverted(true);

    arduino.addDevice(&vmon);
    arduino.addDevice(&imu);
    arduino.begin();

    vmon.makeMainVmon();

    // Runs when X pressed on gamepad
    ActionManager::addTrigger(new ButtonPressedTrigger(&gp0, 0, new ActionSeries(
        {new DriveTimeAction(1000, 0.5), new DriveTimeAction(1000, -0.5)},
        new JSDriveAction()
    )));

    // Runs until interrupted
    ActionManager::startAction(new JSDriveAction());
}

void Robot::robotEnabled(){

}

void Robot::robotDisabled(){

}

void Robot::enabledPeriodic(){

}

void Robot::disabledPeriodic(){

}

void Robot::periodic(){
    Logger::logInfo("Orientation = " + std::to_string(imu.getGyroZ()));
    if(gp0.getButton(0)){
        imu.setGyroZ(0);
    }
    feedWatchdog();
}
