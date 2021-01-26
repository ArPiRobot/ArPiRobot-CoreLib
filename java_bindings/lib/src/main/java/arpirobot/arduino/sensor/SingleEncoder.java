package arpirobot.arduino.sensor;

import arpirobot.Bridge;
import arpirobot.arduino.device.ArduinoDevice;

/**
 * Single-channel encoder
 */
public class SingleEncoder extends ArduinoDevice {

    /**
     * @param pin The digital pin number this encoder is connected to
     * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
     */
    public SingleEncoder(int pin, boolean useInternalPullup){
        this("" + pin, useInternalPullup);
    }

    /**
     * @param pin The pin number this encoder is connected to (prefix with letter A for analog pin)
     * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
     */
    public SingleEncoder(String pin, boolean useInternalPullup){
        this(pin, useInternalPullup, true, -1);
    }

    /**
     * @param pin The digital pin number this encoder is connected to
     * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public SingleEncoder(int pin, boolean useInternalPullup, boolean createDevice, int deviceId){
        this("" + pin, useInternalPullup, createDevice, deviceId);
    }

    /**
     * @param pin The pin number this encoder is connected to (prefix with letter A for analog pin)
     * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public SingleEncoder(String pin, boolean useInternalPullup, boolean createDevice, int deviceId){
        ptr = Bridge.arpirobot.SingleEncoder_create(pin, useInternalPullup, createDevice, deviceId);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.SingleEncoder_destroy(ptr);
    }

    /**
     * Get the position (tick count) for this encoder
     * @return The position in ticks
     */
    public int getPosition(){
        verifyNotDestroyed();
        return Bridge.arpirobot.SingleEncoder_getPosition(ptr);
    }

    /**
     * Set the current tick count to the given value
     * @param currentPosition The new tick value to set
     */
    public void setPosition(int newPosition){
        verifyNotDestroyed();
        Bridge.arpirobot.SingleEncoder_setPosition(ptr, newPosition);
    }

}
