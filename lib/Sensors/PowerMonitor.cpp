#include "PowerMonitor.h"
#include <Arduino.h>

#define SIZE 10

PowerMonitor::PowerMonitor(HardwareSerial& port, int rx, int tx)
    : pzem(port, rx, tx), logger(Logger::getInstance()) {
        float powerBuffer[SIZE];
        int powerIndex = 0;
    }



bool PowerMonitor::isValid(float value) {
    return !isnan(value) && isfinite(value);
}

bool PowerMonitor::begin() {
    logger.log(Logger::INFO, POWER_MONITOR_LOG_MODULE, "Initializing PZEM...");

    float testPower = pzem.power();

    if (!isValid(testPower)) {
        logger.log(Logger::ERROR, POWER_MONITOR_LOG_MODULE, "PZEM not responding!");
        return false;
    }

    logger.log(Logger::INFO, POWER_MONITOR_LOG_MODULE, "PZEM initialized successfully");
    return true;
}

bool PowerMonitor::readData(float &power, float &frequency) {
    float p = pzem.power();
    float f = pzem.frequency();

    logger.logf(Logger::DEBUG, POWER_MONITOR_LOG_MODULE, "Raw Readings -> Power: %.2f, Frequency: %.2f", p, f);

    if (!isValid(p) || !isValid(f)) {
        logger.log(Logger::WARN, POWER_MONITOR_LOG_MODULE, "Invalid sensor data detected");
        return false;
    }

    power = p;
    frequency = f;

    logger.logf(Logger::DEBUG, POWER_MONITOR_LOG_MODULE, "Valid Readings -> Power: %.2f W, Frequency: %.2f Hz", power, frequency);

    return true;
}

float PowerMonitor::getVoltage() {
    float v = pzem.voltage();

    if (!isValid(v)) {
        logger.log(Logger::WARN, POWER_MONITOR_LOG_MODULE, "Invalid voltage reading");
        return -1.0f;
    }

    logger.logf(Logger::DEBUG, POWER_MONITOR_LOG_MODULE, "Voltage: %.2f V", v);

    return v;
}

float PowerMonitor::getCurrent() {
    float c = pzem.current();

    if (!isValid(c)) {
        logger.log(Logger::WARN, POWER_MONITOR_LOG_MODULE, "Invalid current reading");
        return -1.0f;
    }

    logger.logf(Logger::DEBUG, POWER_MONITOR_LOG_MODULE, "Current: %.2f A", c);

    return c;
}

float PowerMonitor::smoothPower(float newPower) {

    if (!isValid(newPower)) {
        logger.log(Logger::WARN, POWER_MONITOR_LOG_MODULE, "Skipping invalid power sample");
        return -1.0f;
    }

    // store in circular buffer
    powerBuffer[powerIndex] = newPower;
    powerIndex = (powerIndex + 1) % SIZE;

    if (powerIndex == 0) {
        bufferFilled = true;
    }

    int count = bufferFilled ? SIZE : powerIndex;

    float sum = 0.0f;
    for (int i = 0; i < count; i++) {
        sum += powerBuffer[i];
    }

    float avg = sum / count;

    logger.logf(Logger::DEBUG, POWER_MONITOR_LOG_MODULE, "Raw: %.2f | Smoothed: %.2f", newPower, avg);

    return avg;
}

float PowerMonitor::getEnergy() {
    float e = pzem.energy();

    if (!isValid(e)) {
        logger.log(Logger::WARN, POWER_MONITOR_LOG_MODULE, "Invalid energy reading");
        return -1.0f;
    }

    logger.logf(Logger::DEBUG, POWER_MONITOR_LOG_MODULE, "Energy: %.2f kWh", e);

    return e;
}

void PowerMonitor::resetEnergy() {
    logger.log(Logger::INFO, POWER_MONITOR_LOG_MODULE, "Resetting energy counter");

    pzem.resetEnergy();
}

