package arpirobot.arduino.device;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;

import com.sun.jna.Pointer;

public abstract class ArduinoDevice extends DestroyableObject {

    /**
     * Get the human-readable name of the device
     * @return the devices human readable name
     */
    public String getDeviceName(){
        verifyNotDestroyed();
        Pointer strPtr = Bridge.arpirobot.ArduinoDevice_getDeviceName(ptr);
        String str = strPtr.getString(0);
        Bridge.arpirobot.freeString(strPtr);
        return str;
    }
}
