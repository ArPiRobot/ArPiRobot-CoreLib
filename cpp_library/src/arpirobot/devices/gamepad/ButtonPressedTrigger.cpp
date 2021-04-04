
#include <arpirobot/devices/gamepad/ButtonPressedTrigger.hpp>

using namespace arpirobot;

ButtonPressedTrigger::ButtonPressedTrigger(Gamepad *gamepad, int buttonNum, Action *targetAction, 
        bool doRestart) : BaseActionTrigger(targetAction, doRestart), gamepad(gamepad), buttonNum(buttonNum){

}

bool ButtonPressedTrigger::shouldRun(){
    bool res = false;
    bool value = gamepad->getButton(buttonNum);
    if(value && !lastValue){
        res = true;
    }
    lastValue = value;
    return res;
}