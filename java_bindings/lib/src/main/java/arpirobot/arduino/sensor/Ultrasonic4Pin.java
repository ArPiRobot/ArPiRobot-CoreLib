package arpirobot.arduino.sensor;

import arpirobot.Bridge;
import arpirobot.arduino.device.ArduinoDevice;

/**
 * Device for 4-pin ultrasonic sensor.
 */
public class Ultrasonic4Pin extends ArduinoDevice {

    /**
     * @param triggerPin Digital pin number for the trigger pin
     * @param echoPin Digital pin number for the echo pin
     */
    public Ultrasonic4Pin(int triggerPin, int echoPin){
        this("" + triggerPin, "" + echoPin);
    }

    /**
     * @param triggerPin Digital pin number for the trigger pin
     * @param echoPin Pin number for the echo pin (prefix with letter A for analog pin)
     */
    public Ultrasonic4Pin(int triggerPin, String echoPin){
        this("" + triggerPin, echoPin);
    }

    /**
     * @param triggerPin Pin number for the trigger pin (prefix with letter A for analog pin)
     * @param echoPin Digital pin number for the echo pin
     */
    public Ultrasonic4Pin(String triggerPin, int echoPin){
        this(triggerPin, "" + echoPin);
    }

    /**
     * @param triggerPin Pin number for the trigger pin (prefix with letter A for analog pin)
     * @param echoPin Pin number for the echo pin (prefix with letter A for analog pin)
     */
    public Ultrasonic4Pin(String triggerPin, String echoPin){
        this(triggerPin, echoPin, true, -1);
    }

    /**
     * @param triggerPin Digital pin number for the trigger pin
     * @param echoPin Digital pin number for the echo pin
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public Ultrasonic4Pin(int triggerPin, int echoPin, boolean createDevice, int deviceId){
        this("" + triggerPin, "" + echoPin, createDevice, deviceId);
    }

    /**
     * @param triggerPin Digital pin number for the trigger pin
     * @param echoPin Pin number for the echo pin (prefix with letter A for analog pin)
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public Ultrasonic4Pin(int triggerPin, String echoPin, boolean createDevice, int deviceId){
        this("" + triggerPin, echoPin, createDevice, deviceId);
    }

    /**
     * @param triggerPin Pin number for the trigger pin (prefix with letter A for analog pin)
     * @param echoPin Digital pin number for the echo pin
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public Ultrasonic4Pin(String triggerPin, int echoPin, boolean createDevice, int deviceId){
        this(triggerPin, "" + echoPin, createDevice, deviceId);
    }

    /**
     * @param triggerPin Pin number for the trigger pin (prefix with letter A for analog pin)
     * @param echoPin Pin number for the echo pin (prefix with letter A for analog pin)
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public Ultrasonic4Pin(String triggerPin, String echoPin, boolean createDevice, int deviceId){
        ptr = Bridge.arpirobot.Ultrasonic4Pin_create(triggerPin, echoPin, createDevice, deviceId);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.Ultrasonic4Pin_destroy(ptr);
    }

    /**
     * Get the last distance read by the ultrasonic sensor
     * @return The distance in cm
     */
    public int getDistance(){
        verifyNotDestroyed();
        return Bridge.arpirobot.Ultrasonic4Pin_getDistance(ptr);
    }

}
