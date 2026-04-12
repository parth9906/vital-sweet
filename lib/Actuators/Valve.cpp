#include "Valve.h"

#define VALVE_MODULE "VALVE"

Valve::Valve(uint8_t pin, const char* name, bool activeHigh)
    : _pin(pin),
      _activeHigh(activeHigh),
      _state(false),
      _name(name),
      _onTime(0),
      _duration(0),
      _useTimer(false),
      logger(Logger::getInstance()) {}

void Valve::begin() {
    pinMode(_pin, OUTPUT);
    close();

    logger.log(Logger::INFO, VALVE_MODULE, "Valve initialized");
}

void Valve::write(bool state) {
    _state = state;

    if (_activeHigh)
        digitalWrite(_pin, state ? HIGH : LOW);
    else
        digitalWrite(_pin, state ? LOW : HIGH);
}

void Valve::open(uint32_t durationMs) {
    write(true);

    logger.logf(Logger::INFO, VALVE_MODULE,
        "%s opened", _name);

    if (durationMs > 0) {
        _useTimer = true;
        _duration = durationMs;
        _onTime = millis();

        logger.logf(Logger::DEBUG, VALVE_MODULE,
            "%s auto-close in %lu ms", _name, durationMs);
    } else {
        _useTimer = false;
    }
}

void Valve::close() {
    if (!_state) return;

    write(false);
    _useTimer = false;

    logger.logf(Logger::INFO, VALVE_MODULE, "%s closed", _name);
}

void Valve::toggle() {
    logger.logf(Logger::DEBUG, VALVE_MODULE, "%s toggle", _name);

    if (_state)
        close();
    else
        open();
}

bool Valve::isOpen() {
    return _state;
}

void Valve::update() {
    if (_useTimer && _state) {
        if (millis() - _onTime >= _duration) {
            logger.logf(Logger::WARN, VALVE_MODULE, "%s auto timeout reached", _name);

            close();
        }
    }
}