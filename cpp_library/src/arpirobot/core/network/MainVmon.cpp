#include <arpirobot/core/network/MainVmon.hpp>
#include <arpirobot/core/network/NetworkManager.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <cmath>
#include <sstream>
#include <iomanip>


using namespace arpirobot;
using namespace std::placeholders;


void MainVmon::makeMainVmon(){
    NetworkManager::setMainVmon(this);
}

bool MainVmon::isMainVmon(){
    return NetworkManager::isMainVmon(this);
}

void MainVmon::sendMainBatteryVoltage(double voltage){
    if(isMainVmon())
        NetworkManager::sendMainBatteryVoltage(voltage);
}