package arpirobot.core.network;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;

/**
 * Main voltage monitor interface
 * Any sensor (arduino or pi device) can implement this interface to be able to be used as a main vmon
 */
public abstract class MainVmon extends DestroyableObject{
    /**
     * Make this device the main voltage monitor
     */
    public void makeMainVmon(){
        Bridge.arpirobot.MainVMon_makeMainVmon(ptr);
    }
}
