#pragma once

#include <thread>
#include <vector>

namespace arpirobot{

    // Forward declare
    class ArduinoDevice;

    class BaseArduinoInterface{
    public:

        // User accessible functions

        virtual ~BaseArduinoInterface();

        bool begin();

        void addDevice(ArduinoDevice *device);

        bool isReady();

    protected:
        
        // Communication functions

        void run();

        uint16_t calcCCittFalse(const std::vector<uint8_t> &data, size_t len);

        /**
         * Writes a message to the arduino using proper escape sequences
         */
        bool writeData(const std::vector<uint8_t> &data);

        /**
         * This method blocks until at least one byte is available. One byte is then read
         * and stored in the workingBuffer. Escape, start, and end sequences are handled
         * appropriately.
         * When a full dataset has been read into the workingBuffer it is moved into the read_dataset.
         * Then, this method return true.
         * Otherwise it returns false
         */
        bool readData();

        /**
         * Using the complete dataset in read_dataset, calculates the CRC and reads the CRC
         * Compares the two
         */
        bool checkData();

        std::vector<uint8_t> waitForMessage(const std::vector<uint8_t> &prefix, int timeoutMs);

        // Implementation specific
        virtual void open() = 0;
        virtual void close() = 0;
        virtual bool isOpen() = 0;
        virtual int available() = 0;
        virtual uint8_t readOne() = 0;
        virtual std::vector<uint8_t> readAll() = 0;
        virtual void write(const uint8_t &b) = 0;
        virtual std::string getDeviceName() = 0;

    private:

        static bool msgStartsWith(const std::vector<uint8_t> &msg, const std::vector<uint8_t> &prefix);
        static bool msgEquals(const std::vector<uint8_t> &msg1, const std::vector<uint8_t> &msg2);

        std::vector<uint8_t> workingBuffer;
        std::vector<uint8_t> readDataset;
        bool parseStarted = false;
        bool parseEscaped = false;
        std::thread *processThread = nullptr;
        std::vector<ArduinoDevice*> devices;
        bool initialized = false;
        bool arduinoReady = false;

        const static uint8_t START_BYTE;
        const static uint8_t END_BYTE;
        const static uint8_t ESCAPE_BYTE;

        const static std::vector<uint8_t> MSG_START;
        const static std::vector<uint8_t> MSG_ADD;
        const static std::vector<uint8_t> MSG_ADDSUCCESS;

        const static std::vector<uint8_t> CMD_RESET;
        const static std::vector<uint8_t> CMD_END;

        friend class ArduinoDevice;
    };

    class ArduinoUartInterface : public BaseArduinoInterface{
    public:
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