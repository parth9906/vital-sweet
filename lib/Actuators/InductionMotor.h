#ifndef INDUCTION_MOTOR_H
#define INDUCTION_MOTOR_H

#include <Arduino.h>
#include "Logger.h"

class InductionMotor {
public:
    InductionMotor(uint8_t startPin, uint8_t stopPin);

    void begin();

    void start();
    void stop();

    bool isRunning();

private:
    uint8_t _startPin;
    uint8_t _stopPin;

    bool _running;

    Logger& logger;
};

#endif