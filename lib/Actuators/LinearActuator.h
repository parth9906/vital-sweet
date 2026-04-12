#ifndef LINEAR_ACTUATOR_H
#define LINEAR_ACTUATOR_H

#include <Arduino.h>
#include "Logger.h"

class LinearActuator {
public:
    LinearActuator(uint8_t pinExtend, uint8_t pinRetract);

    void begin();
    void extend();
    void retract();
    void stop();

private:
    uint8_t _pinExtend;
    uint8_t _pinRetract;
    Logger& logger;

    void setPins(bool extendState, bool retractState);
};

#endif