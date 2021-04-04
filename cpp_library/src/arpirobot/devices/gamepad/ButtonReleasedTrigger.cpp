
#include <arpirobot/devices/gamepad/ButtonReleasedTrigger.hpp>

using namespace arpirobot;

ButtonReleasedTrigger::ButtonReleasedTrigger(Gamepad *gamepad, int buttonNum, Action *targetAction, 
        bool doRestart) : BaseActionTrigger(targetAction, doRestart), gamepad(gamepad), buttonNum(buttonNum){

}

bool ButtonReleasedTrigger::shouldRun(){
    bool res = false;
    bool value = gamepad->getButton(buttonNum);
    if(!value && lastValue){
        res = true;
    }
    lastValue = value;
    return res;
}