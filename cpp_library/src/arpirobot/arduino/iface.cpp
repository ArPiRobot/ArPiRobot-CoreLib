#include <arpirobot/arduino/iface.hpp>
#include <arpirobot/arduino/device.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/core/robot.hpp>

using namespace arpirobot;

#include <functional>
#include <algorithm>
#include <pigpio.h>
#include <cstring>


////////////////////////////////////////////////////////////////////////////////
/// BaseArduinoInterface
////////////////////////////////////////////////////////////////////////////////

const uint8_t BaseArduinoInterface::START_BYTE = 253;
const uint8_t BaseArduinoInterface::END_BYTE = 254;
const uint8_t BaseArduinoInterface::ESCAPE_BYTE = 255;

const std::vector<uint8_t> BaseArduinoInterface::MSG_START = {'S', 'T', 'A', 'R', 'T'};
const std::vector<uint8_t> BaseArduinoInterface::MSG_ADD = {'A', 'D', 'D'};
const std::vector<uint8_t> BaseArduinoInterface::MSG_ADDSUCCESS = {'A', 'D', 'D', 'S', 'U', 'C', 'C', 'E', 'S', 'S'};

const std::vector<uint8_t> BaseArduinoInterface::CMD_RESET = {'R', 'E', 'S', 'E', 'T'};
const std::vector<uint8_t> BaseArduinoInterface::CMD_END = {'E', 'N', 'D'};

BaseArduinoInterface::~BaseArduinoInterface(){
    if(processThread != nullptr){
        processThread->join();
        delete processThread;
    }
}

bool BaseArduinoInterface::begin(){
    initialized = true;
    parseStarted = false;
    parseEscaped = false;

    if(!isOpen())
        open();
    
    if(!isOpen()){
        Logger::logWarningFrom(getDeviceName(), "Unable to open arduino interface");
        return false;
    }

    Logger::logDebugFrom(getDeviceName(), "Opened interface.");

    try{
        // Clear any start message from the buffer
        // This will be there from when the arduino reset on opening UART
        waitForMessage(MSG_START, 3000);

        // Begin by sending reset command (just in case it did not reset on opening UART)
        // Some devices don't reset on UART (Teensy for example)
        writeData(CMD_RESET);

        // Wait for START message
        auto msg = waitForMessage(MSG_START, 5000);
        if(msg.size() == 0){
            Logger::logWarningFrom(getDeviceName(), "Timed out while waiting for the Arduino to become ready.");
            return false;
        }
    }catch(const std::runtime_error &e){
        Logger::logWarningFrom(getDeviceName(), "Error while configuring devices.");
        Logger::logDebugFrom(getDeviceName(), e.what());
        return false;
    }

    Logger::logDebugFrom(getDeviceName(), "Arduino is ready. Creating devices.");

    try{
        for(auto dev : devices){
            if(dev->createDevice){
                auto createData = dev->getCreateData();
                
                // Empty create data means the device failed to generate its create data
                // The device should handle error messages
                if(createData.size() > 0){
                    writeData(createData);
                    auto msg = waitForMessage(MSG_ADD, 5000);
                    if(msg.size() == 0){
                        Logger::logWarningFrom(getDeviceName(), "Timed out while adding device " + dev->getDeviceName());
                        dev->setDeviceId(-1);
                    }else if(msgStartsWith(msg, MSG_ADDSUCCESS)){
                        dev->setDeviceId(msg[10]);
                        dev->applyDefaultState();
                        Logger::logDebugFrom(getDeviceName(), "Created device with ID " + std::to_string(dev->deviceId));
                    }else{
                        Logger::logWarningFrom(getDeviceName(), "Arduino failed to add device " + dev->getDeviceName());
                        dev->setDeviceId(-1);
                    }
                }
            }
        }
    }catch(const std::runtime_error &e){
        Logger::logWarningFrom(getDeviceName(), "Error while configuring devices.");
        Logger::logDebugFrom(getDeviceName(), e.what());
        return false;
    }

    Logger::logDebugFrom(getDeviceName(), "Done creating devices. Starting sensor processing.");

    try{
        writeData(CMD_END);
        auto msg = waitForMessage(CMD_END, 5000);
        if(msg.size() == 0){
            Logger::logWarningFrom(getDeviceName(), "Timed out while starting sensor processing.");
            return false;
        }
    }catch(const std::runtime_error &e){
        Logger::logWarningFrom(getDeviceName(), "Error while configuring devices.");
        Logger::logDebugFrom(getDeviceName(), e.what());
        return false;
    }

    Logger::logDebugFrom(getDeviceName(), "Sensor processing started successfully.");

    if(processThread != nullptr){
        arduinoReady = false;
        processThread->join();
        delete processThread;
    }
    arduinoReady = true;
    processThread = new std::thread(std::bind(&BaseArduinoInterface::run, this));
    return true;
}

void BaseArduinoInterface::addDevice(ArduinoDevice *device){
    if(initialized){
        Logger::logWarningFrom(getDeviceName(), "Attempted to add device after calling begin. This will not work.");
        return;
    }
    if(std::find(devices.begin(), devices.end(), device) == devices.end()){
        devices.push_back(device);
    }
}

bool BaseArduinoInterface::isReady(){
    return arduinoReady;
}

void BaseArduinoInterface::run(){
    while(arduinoReady){
        try{
            if(readData() && checkData()){
                // Special case check
                if(msgEquals(readDataset, MSG_START)){
                    Logger::logWarningFrom(getDeviceName(), "Arduino was reset while running. Sensor data is now INVALID! Will reconfigure.");
                    break;
                }

                int deviceId = readDataset[0];
                for(auto dev : devices){
                    if(dev->deviceId == deviceId){
                        dev->handleData(readDataset);
                        break;
                    }
                }
            }
        }catch(const std::runtime_error &e){
            Logger::logWarningFrom(getDeviceName(), "Lost communication with the arduino. Sensor data is now INVALID!. Will reconfigure.");
            Logger::logDebugFrom(getDeviceName(), e.what());
            arduinoReady = false;
        }
    }

    // The above loop exits when communication is lost
    // Have begin run asynchronously
    BaseRobot::runOnceSoon(std::bind(&BaseArduinoInterface::begin, this));
}

uint16_t BaseArduinoInterface::calcCCittFalse(const std::vector<uint8_t> &data, size_t len){
    uint16_t crc = 0xFFFF;
    int pos = 0;
    while(pos < len){
        uint8_t b = data[pos];
        for(int i = 0; i < 8; ++i){
            uint8_t bit = ((b >> (7 - i) & 1) == 1);
            uint8_t c15 = ((crc >> 15 & 1) == 1);
            crc <<= 1;
            if(c15 ^ bit){
                crc ^= 0x1021;
            }
        }
        pos++;
    }
    return crc;
}

bool BaseArduinoInterface::writeData(const std::vector<uint8_t> &data){
    try{
        write(START_BYTE);
        for(int i = 0; i < data.size(); ++i){
            const uint8_t &b = data[i];
            if(b == START_BYTE || b == END_BYTE || b == ESCAPE_BYTE)
                write(ESCAPE_BYTE);
            write(b);
        }

        // Calculate CRC
        uint16_t crc = calcCCittFalse(data, data.size());
        const uint8_t crc_low = crc & 0x00FF;
        const uint8_t crc_high = crc >> 8 & 0x00FF;
        if(crc_high == START_BYTE || crc_high == END_BYTE || crc_high == ESCAPE_BYTE)
            write(ESCAPE_BYTE);
        write(crc_high);
        if(crc_low == START_BYTE || crc_low == END_BYTE || crc_low == ESCAPE_BYTE)
            write(ESCAPE_BYTE);
        write(crc_low);

        write(END_BYTE);

    }catch(const std::runtime_error &e){
        Logger::logDebugFrom(getDeviceName(), e.what());
    }
    return false;
}

bool BaseArduinoInterface::readData(){
    uint8_t b = readOne();
    if(parseEscaped){
        // Only allow valid escape sequences
        if(b == START_BYTE || b == END_BYTE || b == ESCAPE_BYTE)
            workingBuffer.push_back(b);
        parseEscaped = false;
    }else{
        if(b == START_BYTE){
            if(parseStarted){
                // Got a second start byte. Trashing buffer.
                workingBuffer.clear();
            }
            parseStarted = true;
        }else if(b == END_BYTE && parseStarted){
            readDataset = workingBuffer;
            workingBuffer.clear();
            parseStarted = false;
            return true;
        }else if(b == ESCAPE_BYTE && parseStarted){
            parseEscaped = true;
        }else if(b != START_BYTE && b != END_BYTE && b != ESCAPE_BYTE){
            workingBuffer.push_back(b);
        }
    }
    return false;
}

bool BaseArduinoInterface::checkData(){
    if(readDataset.size() == 0)
        return false;
    // CRC is sent big endian (high_byte, low_byte)
    uint16_t readCrc = readDataset[readDataset.size() - 2] << 8 | readDataset[readDataset.size() - 1];
    uint16_t calcCrc = calcCCittFalse(readDataset, readDataset.size() - 2);

    return readCrc == calcCrc;
}

std::vector<uint8_t> BaseArduinoInterface::waitForMessage(const std::vector<uint8_t> &prefix, int timeoutMs){
    auto start = std::chrono::steady_clock::now();
    while(true){
        while(available()  == 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start).count();
            if(elapsed >= timeoutMs){
                return {};
            }
        }
        if(readData() && checkData()){
            if(msgStartsWith(readDataset, prefix)){
                return readDataset;
            }
        }
    }
}

bool BaseArduinoInterface::msgStartsWith(const std::vector<uint8_t> &msg, const std::vector<uint8_t> &prefix){
    // If prefix is longer than msg then msg cannot start with prefix
    if(prefix.size() > msg.size())
        return false;
    for(size_t i = 0; i < prefix.size(); ++i){
        if(prefix[i] != msg[i])
            return false;
    }
    return true;
}

bool BaseArduinoInterface::msgEquals(const std::vector<uint8_t> &msg1, const std::vector<uint8_t> &msg2){
    if(msg1.size() != msg2.size())
        return false;
    for(size_t i = 0; i < msg1.size(); ++i){
        if(msg1[i] != msg2[i])
            return false;
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////
/// ArduinoUartInterface
////////////////////////////////////////////////////////////////////////////////

ArduinoUartInterface::ArduinoUartInterface(std::string port, int baud) : port(port), baud(baud){

}

void ArduinoUartInterface::open() {
    if(handle < 0){
        char tty[port.length() + 1];
        strcpy(tty, port.c_str());
        handle = serOpen(tty, baud, 0);
    }
}

void ArduinoUartInterface::close() {
	if(handle >= 0){
        serClose(handle);
    }
}

bool ArduinoUartInterface::isOpen() {
	return handle >= 0;
}

int ArduinoUartInterface::available() {
    if(handle < 0)
        return 0;
    return serDataAvailable(handle);
}

uint8_t ArduinoUartInterface::readOne() {
    int b = serReadByte(handle);
    if(b < 0 && b != PI_SER_READ_NO_DATA){
        throw std::runtime_error("Failed to read from serial port.");
    }
    return (uint8_t) b;
}

std::vector<uint8_t> ArduinoUartInterface::readAll() {
    int a = available();
	std::vector<uint8_t> data;
    data.reserve(a);
    for(int i = 0; i < a; ++i){
        data.push_back(readOne());
    }
    return data;
}

void ArduinoUartInterface::write(const uint8_t &b) {
	int res = serWriteByte(handle, b);
    if(res < 0){
        throw std::runtime_error("Failed to write to serial port.");
    }
}

std::string ArduinoUartInterface::getDeviceName() {
	return "ArduinoUartInterface(" + port + ", " + std::to_string(baud) + ")";
}
