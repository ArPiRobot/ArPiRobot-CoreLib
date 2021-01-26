package arpirobot.arduino.sensor;

import arpirobot.Bridge;
import arpirobot.arduino.device.ArduinoDevice;

/**
 * Simple analog input voltage monitor (using voltage divider) connected to arduino analog input.
 */
public class VoltageMonitor extends ArduinoDevice {

    /**
     * @param pin The analog input pin number this voltage monitor is connected
     * @param vboard The board voltage for this arduino (usually 5V or 3.3V).
     * @param r1 The top resistor of the voltage divider
     * @param r2 The bottom resistor of the voltage divider (the one voltage is measured across)
     */
    public VoltageMonitor(int pin, double vboard, int r1, int r2){
        this("" + pin, vboard, r1, r2);
    }

    /**
     * @param pin The analog input pin this voltage monitor is connected to (can be prefixed with letter A)
     * @param vboard The board voltage for this arduino (usually 5V or 3.3V).
     * @param r1 The top resistor of the voltage divider
     * @param r2 The bottom resistor of the voltage divider (the one voltage is measured across)
     */
    public VoltageMonitor(String pin, double vboard, int r1, int r2){
        this(pin, vboard, r1, r2, true, -1);
    }

    /**
     * @param pin The analog input pin number this voltage monitor is connected
     * @param vboard The board voltage for this arduino (usually 5V or 3.3V).
     * @param r1 The top resistor of the voltage divider
     * @param r2 The bottom resistor of the voltage divider (the one voltage is measured across)
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public VoltageMonitor(int pin, double vboard, int r1, int r2, boolean createDevice, int deviceId){
        this("" + pin, vboard, r1, r2, createDevice, deviceId);
    }

    /**
     * @param pin The analog input pin this voltage monitor is connected to (can be prefixed with letter A)
     * @param vboard The board voltage for this arduino (usually 5V or 3.3V).
     * @param r1 The top resistor of the voltage divider
     * @param r2 The bottom resistor of the voltage divider (the one voltage is measured across)
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public VoltageMonitor(String pin, double vboard, int r1, int r2, boolean createDevice, int deviceId){
        ptr = Bridge.arpirobot.VoltageMonitor_create(pin, vboard, r1, r2, createDevice, deviceId);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.VoltageMonitor_destroy(ptr);
    }

    /**
     * Get the voltage read by this voltage monitor (in volts)
     * @return The voltage in volts
     */
    public double getVoltage(){
        verifyNotDestroyed();
        return Bridge.arpirobot.VoltageMonitor_getVoltage(ptr);
    }
    
}
