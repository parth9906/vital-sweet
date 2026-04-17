#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

struct MachineConfig {
    // Operation Mode
    bool isAutoModeEnabled = false;

    // Weight Setpoints
    float startHeatingWeightThreshold = 20.0f;
    float targetJuiceWeight = 50.0f;           
    float oilSprayTemperatureThreshold = 105.0f;

    // Process Targets
    float targetJaggeryBrix = 83.0f;          
    float finishingPhaseTempThreshold = 115.0f;

    // Mechanical Timings
    uint32_t panTiltUpDurationMs = 5000;      
    uint32_t panResetDownDurationMs = 8000;   

    // Added 'ArduinoJson::' namespace to JsonObject
    void updateFromJson(ArduinoJson::JsonVariant doc) {
        if (doc.isNull()) return;

        // The | operator works perfectly here for defaults
        isAutoModeEnabled = doc["isAutoModeEnabled"] | isAutoModeEnabled;
        startHeatingWeightThreshold = doc["startHeatingWeightThreshold"] | startHeatingWeightThreshold;
        targetJuiceWeight = doc["targetJuiceWeight"] | targetJuiceWeight;
        oilSprayTemperatureThreshold = doc["oilSprayTemperatureThreshold"] | oilSprayTemperatureThreshold;
        targetJaggeryBrix = doc["targetJaggeryBrix"] | targetJaggeryBrix;
        finishingPhaseTempThreshold = doc["finishingPhaseTempThreshold"] | finishingPhaseTempThreshold;
        panTiltUpDurationMs = doc["panTiltUpDurationMs"] | panTiltUpDurationMs;
        panResetDownDurationMs = doc["panResetDownDurationMs"] | panResetDownDurationMs;
    }
};
