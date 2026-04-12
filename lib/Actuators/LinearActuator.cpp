#include "LinearActuator.h"

#define ACTUATOR_MODULE "ACTUATOR"

LinearActuator::LinearActuator(uint8_t pinExtend, uint8_t pinRetract)
    : _pinExtend(pinExtend),
      _pinRetract(pinRetract),
      logger(Logger::getInstance()) {}

void LinearActuator::begin() {
    pinMode(_pinExtend, OUTPUT);
    pinMode(_pinRetract, OUTPUT);

    stop();

    logger.log(Logger::INFO, ACTUATOR_MODULE, "Initialized actuator");
}

void LinearActuator::setPins(bool extendState, bool retractState) {
    digitalWrite(_pinExtend, extendState);
    digitalWrite(_pinRetract, retractState);
}

void LinearActuator::extend() {
    setPins(HIGH, LOW);

    logger.log(Logger::INFO, ACTUATOR_MODULE, "Extend command executed");
}

void LinearActuator::retract() {
    setPins(LOW, HIGH);

    logger.log(Logger::INFO, ACTUATOR_MODULE, "Retract command executed");
}

void LinearActuator::stop() {
    setPins(LOW, LOW);

    logger.log(Logger::INFO, ACTUATOR_MODULE, "Stop command executed");
}