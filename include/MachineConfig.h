#pragma once
#include <Arduino.h>

struct MachineConfig {
    // Operation Mode
    bool isAutoModeEnabled = false;

    // Weight Setpoints
    float startHeatingWeightThreshold = 20.0f; // Minimum juice to submerge heaters
    float targetJuiceWeight = 50.0f;           // Total batch weight to fill
    float oilSprayTemperatureThreshold = 105.0f; // Temperature at which oil is automatically sprayed

    // Process Targets (Setpoints)
    float targetJaggeryBrix = 83.0f;          // Final sugar concentration
    float finishingPhaseTempThreshold = 115.0f;// Safety/Quality temperature check

    // Mechanical Timings (in milliseconds)
    uint32_t panTiltUpDurationMs = 5000;      // Time to fully empty the pan
    uint32_t panResetDownDurationMs = 8000;   // Time to return pan to home position
};
