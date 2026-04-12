#include "InductionMotor.h"

#define MODULE "MOTOR"

// Safety delay between commands (anti-chatter)
#define MIN_SWITCH_TIME 1000

InductionMotor::InductionMotor(uint8_t startPin, uint8_t stopPin)
    : _startPin(startPin),
      _stopPin(stopPin),
      _running(false),
      logger(Logger::getInstance()) {}

void InductionMotor::begin() {
    pinMode(_startPin, OUTPUT);
    pinMode(_stopPin, OUTPUT);

    // Safe state
    digitalWrite(_startPin, LOW);
    digitalWrite(_stopPin, HIGH);

    _running = false;

    logger.log(Logger::INFO, MODULE, "Motor initialized (STOP state)");
}

void InductionMotor::start() {
    if (_running) {
        logger.log(Logger::DEBUG, MODULE, "Start ignored (already running)");
        return;
    }

    digitalWrite(_stopPin, LOW);
    delay(100); // ensure release (relay safety)

    digitalWrite(_startPin, HIGH);

    _running = true;

    logger.log(Logger::INFO, MODULE, "Motor STARTED");
}

void InductionMotor::stop() {
    if (!_running) {
        logger.log(Logger::DEBUG, MODULE, "Stop ignored (already stopped)");
        return;
    }

    digitalWrite(_startPin, LOW);
    delay(100);

    digitalWrite(_stopPin, HIGH);

    _running = false;

    logger.log(Logger::INFO, MODULE, "Motor STOPPED");
}

bool InductionMotor::isRunning() {
    return _running;
}