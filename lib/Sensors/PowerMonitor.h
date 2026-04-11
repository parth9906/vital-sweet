#pragma once

#include <PZEM004Tv30.h>
#include "Logger.h"

#define POWER_MONITOR_LOG_MODULE "PowerMonitor"

class PowerMonitor {
private:
    PZEM004Tv30 pzem;
    Logger& logger;
    static const int SIZE = 10;
    float powerBuffer[SIZE];
    int powerIndex;
    bool bufferFilled;

    bool isValid(float value);

public:
    PowerMonitor(HardwareSerial& port, int rx, int tx);

    bool begin();

    bool readData(float &power, float &frequency);

    float getVoltage();
    float getCurrent();
    float getEnergy();
    float smoothPower(float power);

    void resetEnergy();
};