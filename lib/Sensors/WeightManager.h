#pragma once

#include "HX711.h"
#include "Logger.h"

#define WEIGHT_LOG_MODULE "WeightManager"

class WeightManager {
private:
    HX711 scale;
    Logger& logger;

    float calibrationFactor;
    bool initialized;

    static const int samples = 5;
    static constexpr float minValidWeight = -100.0f;
    static constexpr float maxValidWeight = 100000.0f;

    bool isValid(float value);

public:
    explicit WeightManager(float calib = -7050.0f);

    bool begin(int dout, int pd_sck);

    float getWeight();
    float getStableWeight(int attempts = 3);

    void tare();

    void setCalibration(float factor);
    float getCalibration() const;
};
