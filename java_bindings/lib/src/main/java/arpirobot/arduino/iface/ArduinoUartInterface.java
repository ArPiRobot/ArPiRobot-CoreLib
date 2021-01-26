package arpirobot.arduino.iface;

import arpirobot.Bridge;

/**
 * Arduino interface implementation using UART to communicate with the arduino.
 */
public class ArduinoUartInterface extends BaseArduinoInterface {
    
    /**
     * @param port The UART port for the arduino (/dev/tty...)
     * @param baud The baud rate for UART communication
     */
    public ArduinoUartInterface(String port, int baud){
        ptr = Bridge.arpirobot.ArduinoUartInterface_create(port, baud);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.ArduinoUartInterface_destroy(ptr);
    }
}
