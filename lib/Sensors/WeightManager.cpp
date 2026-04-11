#include "WeightManager.h"
#include <Arduino.h> 

WeightManager::WeightManager(float calib)
    : logger(Logger::getInstance()),
      calibrationFactor(calib),
      initialized(false) {}

bool WeightManager::isValid(float value) {
    return !isnan(value) && isfinite(value) &&
           value > minValidWeight && value < maxValidWeight;
}

bool WeightManager::begin(int dout, int pd_sck) {
    logger.log(Logger::INFO, WEIGHT_LOG_MODULE, "Initializing HX711...");

    scale.begin(dout, pd_sck);

    if (!scale.is_ready()) {
        logger.log(Logger::ERROR, WEIGHT_LOG_MODULE, "HX711 not responding!");
        return false;
    }

    scale.set_scale(calibrationFactor);
    scale.tare();

    initialized = true;

    logger.log(Logger::INFO, WEIGHT_LOG_MODULE, "HX711 initialized successfully");
    return true;
}

float WeightManager::getWeight() {
    if (!initialized) {
        logger.log(Logger::ERROR, WEIGHT_LOG_MODULE, "Read before initialization");
        return -1.0f;
    }

    if (!scale.is_ready()) {
        logger.log(Logger::WARN, WEIGHT_LOG_MODULE, "HX711 not ready");
        return -1.0f;
    }

    float weight = scale.get_units(samples);

    logger.logf(Logger::DEBUG, WEIGHT_LOG_MODULE,
                "Raw weight: %.2f", weight);

    if (!isValid(weight)) {
        logger.log(Logger::WARN, WEIGHT_LOG_MODULE, "Invalid weight value");
        return -1.0f;
    }

    return weight;
}

float WeightManager::getStableWeight(int attempts) {
    float total = 0.0f;
    int validReads = 0;

    for (int i = 0; i < attempts; i++) {
        float w = getWeight();

        if (w > 0) {
            total += w;
            validReads++;
        }

        delay(50); // small delay for stability
    }

    if (validReads == 0) {
        logger.log(Logger::ERROR, WEIGHT_LOG_MODULE, "No valid readings");
        return -1.0f;
    }

    float avg = total / validReads;

    logger.logf(Logger::INFO, WEIGHT_LOG_MODULE, "Stable weight: %.2f", avg);

    return avg;
}

void WeightManager::tare() {
    if (!initialized) {
        logger.log(Logger::WARN, WEIGHT_LOG_MODULE, "Tare before init ignored");
        return;
    }

    logger.log(Logger::INFO, WEIGHT_LOG_MODULE, "Taring scale...");
    scale.tare();
}

void WeightManager::setCalibration(float factor) {
    calibrationFactor = factor;
    scale.set_scale(calibrationFactor);

    logger.logf(Logger::INFO, WEIGHT_LOG_MODULE, "Calibration updated: %.2f", calibrationFactor);
}

float WeightManager::getCalibration() const {
    return calibrationFactor;
}