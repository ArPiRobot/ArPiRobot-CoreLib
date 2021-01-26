package arpirobot.devices.gamepad;

import arpirobot.Bridge;
import arpirobot.core.action.Action;
import arpirobot.core.action.BaseActionTrigger;

public class ButtonReleasedTrigger extends BaseActionTrigger {
    /**
     * @param gamepad The gamepad to check for button releases on
     * @param buttonNum The button number
     * @param targetAction The action to run when this occurs
     */
    public ButtonReleasedTrigger(Gamepad gamepad, int buttonNum, Action targetAction){
        this(gamepad, buttonNum, targetAction, true);
    }

    /**
     * @param gamepad The gamepad to check for button releases on
     * @param buttonNum The button number
     * @param targetAction The action to run when this occurs
     * @param doRestart If true this will restart the action if it is running when this occurs
     */
    public ButtonReleasedTrigger(Gamepad gamepad, int buttonNum, Action targetAction, boolean doRestart){
        ptr = Bridge.arpirobot.ButtonReleasedTrigger_create(gamepad._ptr(), buttonNum, 
                targetAction._ptr(), doRestart);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.ButtonReleasedTrigger_destroy(ptr);
    }
}
