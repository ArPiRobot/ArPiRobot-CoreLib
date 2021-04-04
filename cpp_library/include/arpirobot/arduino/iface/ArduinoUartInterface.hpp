#pragma once

#include <thread>
#include <vector>
#include <string>

#include <arpirobot/arduino/iface/BaseArduinoInterface.hpp>

namespace arpirobot{

    /**
     * \class ArduinoUartInterface iface.hpp arpirobot/arduino/iface.hpp
     * 
     * Arduino interface implementation using UART to communicate with the arduino.
     */
    class ArduinoUartInterface : public BaseArduinoInterface{
    public:

        /**
         * @param port The UART port for the arduino (/dev/tty...)
         * @param baud The baud rate for UART communication
         */
        ArduinoUartInterface(std::string port, int baud);

    protected:
        void open();
        void close();
        bool isOpen();
        int available();
        uint8_t readOne();
        std::vector<uint8_t> readAll();
        void write(const uint8_t &b);
        std::string getDeviceName();
    
    private:
        std::string port;
        int baud;
        int handle = -1;
    };
}