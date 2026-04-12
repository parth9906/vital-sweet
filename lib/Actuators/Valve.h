#ifndef VALVE_H
#define VALVE_H

#include <Arduino.h>
#include "Logger.h"

class Valve {
public:
    Valve(uint8_t pin, const char* name, bool activeHigh = true);

    void begin();

    void open(uint32_t durationMs = 0);
    void close();
    void toggle();

    void update();

    bool isOpen();

private:
    Logger& logger;
    uint8_t _pin;
    bool _activeHigh;
    bool _state;

    const char* _name;

    uint32_t _onTime;
    uint32_t _duration;
    bool _useTimer;

    void write(bool state);
};

#endif