// TempManager.h
#ifndef TEMP_MANAGER_H
#define TEMP_MANAGER_H

#include <OneWire.h>
#include <DallasTemperature.h>

class TempManager {
private:
    OneWire oneWire;
    DallasTemperature sensors;
    int sensorCount;
    Logger& logger;

public:
    TempManager(int pin);
    void begin();
    float getTempByIndex(int index);
    float getAverageTemp();
};

#endif