package arpirobot.arduino.iface;

import arpirobot.DestroyableObject;
import arpirobot.arduino.device.ArduinoDevice;
import arpirobot.Bridge;

/** 
 * Common base class (communication protocol agnostic) for an arduino (or other microcontroller) sensor coprocessor.
 * This class handles all the high-level data between the arduino and pi, without implementing a specific
 * communication protocol. See the below classes for different communication protocols.
 */
public abstract class BaseArduinoInterface extends DestroyableObject {

    /**
     * Create devices (as needed) and start processing sensor data. After this is called devices can 
     * no longer be added to this arduino using BaseArduinoInterface::addDevice.
     */
    public void begin(){
        Bridge.arpirobot.BaseArduinoInterface_begin(ptr);
    }

    /**
     * Add a device to this arduino interface instance. 
     * You cannot add devices after BaseArduinoInterface::begin is called
     * @param device The ArduinoDevice instance to add
     */
    public void addDevice(ArduinoDevice device){
        Bridge.arpirobot.BaseArduinoInterface_addDevice(ptr, device._ptr());
    }

    /**
     * Returns true when the arduino is ready to process sensor data (after BaseArduinoInterface::begin 
     * is successful)
     * @return true if ready, else false
     */
    public boolean isReady(){
        return Bridge.arpirobot.BaseArduinoInterface_isReady(ptr);
    }
}
