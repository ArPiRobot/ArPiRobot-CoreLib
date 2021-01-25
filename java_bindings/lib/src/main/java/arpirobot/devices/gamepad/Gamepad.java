package arpirobot.devices.gamepad;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;
import arpirobot.core.drive.BaseAxisTransform;

public class Gamepad extends DestroyableObject {

    /**
     * @param controllerNum The controller number for this controller. Top controller in DS is zero
     */
    public Gamepad(int controllerNum){
        ptr = Bridge.arpirobot.Gamepad_create(controllerNum);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.Gamepad_destroy(ptr);
    }

    /**
     * Get the controller number for this gamepad
     * @returns The controller number
     */
    public int getControllerNum(){
        return Bridge.arpirobot.Gamepad_getControllerNum(ptr);
    }

    /**
     * Get an axis for this controller
     * @param axisNum The axis number
     * @returns The axis value after applying the deadband and (if required) an axis transform
     */
    public double getAxis(int axisNum){
        return getAxis(axisNum, 0);
    }

    /**
     * Get an axis for this controller
     * @param axisNum The axis number
     * @param deadband A minimum threshold for axis values. Values below this will be returned as zero.
     * @returns The axis value after applying the deadband and (if required) an axis transform
     */
    public double getAxis(int axisNum, double deadband){
        return Bridge.arpirobot.Gamepad_getAxis(ptr, axisNum, deadband);
    }

    /**
     * Get a button for the controller
     * @param buttonNum The button number
     * @returns true if pressed, else false
     */
    public boolean getButton(int buttonNum){
        return Bridge.arpirobot.Gamepad_getButton(ptr, buttonNum);
    }

    /**
     * Get the value of a dpad
     * @param dpadNum The dpad number
     * @returns 0 if center, 1 for up through 8 going clockwise
     */
    public int getDpad(int dpadNum){
        return Bridge.arpirobot.Gamepad_getDpad(ptr, dpadNum);
    }

    public void setAxisTransform(int axisNum, BaseAxisTransform transform){
        Bridge.arpirobot.Gamepad_setAxisTransform(ptr, axisNum, transform._ptr());
    }

    public void clearAxisTransform(int axisNum){
        Bridge.arpirobot.Gamepad_clearAxisTransform(ptr, axisNum);
    }
}
