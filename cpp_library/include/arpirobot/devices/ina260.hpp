#pragma once

#include <arpirobot/core/device.hpp>
#include <arpirobot/core/scheduler.hpp>

namespace arpirobot{

    class AdafruitINA260{
    public:
        AdafruitINA260(int address, int bus = 1);

        ~AdafruitINA260();

        bool begin();

        void reset();

        // mA
        double readCurrent();

        // mW
        double readPower();


        // mV
        double readBusVoltage();

        void setMode(int mode);

        int getMode();

        void setAveragingCount(int count);

        int getAveragingCount();

        void setCurrentConversionTime(int time);

        int getCurrentConversionTime();

        void setVoltageConversionTime(int time);

        int getVoltageConversiontime();

        bool conversionReady();


        // INA260 Mode values
        const static int MODE_SHUTDOWN;
        const static int MODE_TRIGGERED;
        const static int MODE_CONTINUOUS;

        // INA260 Conversion Time values
        const static int TIME_140_US;
        const static int TIME_204_US;
        const static int TIME_332_US;
        const static int TIME_558_US;
        const static int TIME_1_1_MS;
        const static int TIME_2_116_MS;
        const static int TIME_4_156_MS;
        const static int TIME_8_244_MS;

        // INA260 Averaging Count values
        const static int COUNT_1;
        const static int COUNT_2;
        const static int COUNT_16;
        const static int COUNT_64;
        const static int COUNT_128;
        const static int COUNT_256;
        const static int COUNT_512;
        const static int COUNT_1024;

    private:
        int i2cReadWordHelper(int reg);

        int i2cWriteWordHelper(int reg, int data);

        // Registers
        const static int REG_CONFIG;
        const static int REG_CURRENT;
        const static int REG_BUSVOLTAGE;
        const static int REG_POWER;
        const static int REG_MASK_ENABLE;
        const static int REG_ALERT_LIMIT;
        const static int REG_MFG_UID;
        const static int REG_DIE_UID;

        int handle = -1;
    };


    class INA260PowerSensor : public BaseDevice{
    public:
        INA260PowerSensor();

        ~INA260PowerSensor();

        bool isEnabled();

        bool shouldMatchRobotState();

        bool shouldDisableWithWatchdog();

        void _enable();

        void _disable();

        std::string getDeviceName();

        // Current in mA
        double getCurrent();

        // Voltage in V
        double getVolgate();

        // Power in mW
        double getPower();

        void makeMainVmon();

    protected:
        void begin();

    private:

        void feed();

        std::shared_ptr<AdafruitINA260> sensor;

        double current = 0, voltage = 0, power = 0;

        bool stop = false;
    };

}