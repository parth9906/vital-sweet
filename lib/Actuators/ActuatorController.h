#ifndef ACTUATOR_CONTROLLER_H
#define ACTUATOR_CONTROLLER_H

#include <Arduino.h> // This defines pinMode, digitalWrite, HIGH, LOW

class ActuatorController {
private:
    int _relayPin;
    int _valves[3]; 
    int _laUp, _laDown;

public:
    // Constructor
    ActuatorController(int relay, int v1, int v2, int v3, int lup, int ldown);
    
    // Setup pins
    void begin();

    // Motor Control
    void startMotor();
    void stopMotor();
    
    // Valve Control (id 0, 1, or 2)
    void setValve(int id, bool state);

    // Linear Actuator Control
    // 0 = Stop, 1 = Extend, 2 = Retract
    void moveActuator(int direction);
};

#endif