#pragma once

#include <Arduino.h>
#include "HX711.h"
#include <Preferences.h>

class WeightManager {
private:
    HX711 scale;
    Preferences prefs;
    
    String namespaceName;
    float calibrationFactor;
    long offset; // The 'tare' value
    bool initialized;
    int doutPin;

    static constexpr float NOISE_THRESHOLD = 0.5f; // Ignore fluctuations smaller than this
    
    void loadSettings();
    void saveSettings();

public:
    // Give each instance a unique name (e.g., "scale1", "scale2") for NVS storage
    explicit WeightManager(const char* name, float defaultCalib = -7050.0f);

    bool begin(int dout, int pd_sck);
    
    // Core functions
    float getWeight(int samples = 5);
    float getStableWeight(int samples = 15); // Uses median filter for stability
    
    void tare();
    void setCalibration(float factor);
    
    // Production helpers
    bool isConnected();
    void resetFactory(); // Clears NVS settings
};
