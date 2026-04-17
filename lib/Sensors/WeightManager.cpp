#include "WeightManager.h"

WeightManager::WeightManager(const char* name, float defaultCalib) 
    : namespaceName(name), calibrationFactor(defaultCalib), initialized(false) {}

bool WeightManager::begin(int dout, int pd_sck) {
    doutPin = dout;
    scale.begin(dout, pd_sck);

    // HX711 Warm-up/Check
    unsigned long start = millis();
    while (!scale.is_ready() && millis() - start < 2000) {
        delay(10);
    }

    if (!scale.is_ready()) return false;

    loadSettings(); // Load saved calibration and offset from Flash
    
    scale.set_scale(calibrationFactor);
    scale.set_offset(offset);
    
    initialized = true;
    return true;
}

void WeightManager::loadSettings() {
    prefs.begin(namespaceName.c_str(), true); // Read-only mode
    calibrationFactor = prefs.getFloat("cal", calibrationFactor);
    offset = prefs.getLong("off", 0);
    prefs.end();
}

void WeightManager::saveSettings() {
    prefs.begin(namespaceName.c_str(), false); // Read-write mode
    prefs.putFloat("cal", calibrationFactor);
    prefs.putLong("off", offset);
    prefs.end();
}

float WeightManager::getWeight(int samples) {
    if (!initialized || !scale.is_ready()) return 0.0f;
    
    float val = scale.get_units(samples);
    // Suppress jitter near zero
    return (abs(val) < NOISE_THRESHOLD) ? 0.0f : val;
}

float WeightManager::getStableWeight(int samples) {
    if (!initialized || !scale.is_ready()) return 0.0f;

    float readings[samples];
    for (int i = 0; i < samples; i++) {
        readings[i] = scale.get_units(1);
        delay(5); 
    }

    // Simple Bubble Sort to find Median (Production Stable)
    for (int i = 0; i < samples - 1; i++) {
        for (int j = 0; j < samples - i - 1; j++) {
            if (readings[j] > readings[j + 1]) {
                float temp = readings[j];
                readings[j] = readings[j + 1];
                readings[j + 1] = temp;
            }
        }
    }

    float median = readings[samples / 2];
    return (abs(median) < NOISE_THRESHOLD) ? 0.0f : median;
}

void WeightManager::tare() {
    scale.tare(20); // Average of 20 for a clean zero
    offset = scale.get_offset();
    saveSettings(); // Persist to Flash
}

void WeightManager::setCalibration(float factor) {
    calibrationFactor = factor;
    scale.set_scale(calibrationFactor);
    saveSettings(); // Persist to Flash
}

bool WeightManager::isConnected() {
    return scale.is_ready();
}
