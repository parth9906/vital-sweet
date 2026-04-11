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

public:
    TempManager(int pin);
    void begin();
    float getTempByIndex(int index); // Returns temp for sensor 0, 1, or 2
    float getAverageTemp();
};

#endif