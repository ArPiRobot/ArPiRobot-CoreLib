package arpirobot.core.device;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;
import com.sun.jna.Pointer;

/**
 * Base class for any device connected to the raspberry pi.
 */
public abstract class BaseDevice extends DestroyableObject {
    /**
     * Get the human-readable name of the device
     * @returns the devices human readable name
     */
    public String getDeviceName(){
        Pointer strPtr = Bridge.arpirobot.BaseDevice_getDeviceName(ptr);
        String str = strPtr.getString(0);
        Bridge.arpirobot.freeString(strPtr);
        return str;
    }
}
