    #include "ActuatorController.h"

    // 1. CONSTRUCTOR: This runs once when you create the object
    // We take the pin numbers from main.cpp and save them into our class variables
    ActuatorController::ActuatorController(int relay, int v1, int v2, int v3, int lup, int ldown) {
        _relayPin = relay;
        
        // Assign each valve pin to its specific spot in our list
        _valves[0] = v1; 
        _valves[1] = v2; 
        _valves[2] = v3;
        
        // Linear Actuator pins
        _laUp = lup;
        _laDown = ldown;
    }

    // 2. BEGIN: This sets up the hardware pins
    void ActuatorController::begin() {
        // Setup the Motor Relay
        pinMode(_relayPin, OUTPUT);
        digitalWrite(_relayPin, LOW); // Start with motor OFF (Safety First)

        // Setup the 3 Valves using a simple loop
        for (int i = 0; i < 3; i++) {
            int currentValvePin = _valves[i];
            pinMode(currentValvePin, OUTPUT);
            digitalWrite(currentValvePin, LOW); // Close all valves initially
        }

        // Setup the Linear Actuator pins
        pinMode(_laUp, OUTPUT);
        pinMode(_laDown, OUTPUT);
        
        // Make sure the actuator isn't moving when we power on
        digitalWrite(_laUp, LOW);
        digitalWrite(_laDown, LOW);
    }

    // 3. MOTOR CONTROL: Simple ON/OFF for the induction motor
    void ActuatorController::startMotor(){
        digitalWrite(_relayPin, HIGH);
    }

    void ActuatorController::stopMotor(){
        digitalWrite(_relayPin, LOW);
    }



    // 4. VALVE CONTROL: Pick a valve (0, 1, or 2) and turn it ON or OFF
    void ActuatorController::setValve(int valveNumber, bool turnOn) {
        
        // STEP 1: Check if the hook number actually exists (0, 1, or 2)
        // If someone asks for hook 99, we just stop so the brain doesn't crash.
        if (valveNumber >= 0 && valveNumber <= 2) {
            
            // STEP 2: Look into our cabinet (_valves) at the hook they asked for
            // and grab the GPIO pin number stored there.
                
            
            // STEP 3: Now that we have the pin number, decide to send 
            // electricity (HIGH) or cut it (LOW).
            if (turnOn == true) {
                digitalWrite(1, HIGH);
            } else {
                digitalWrite(1, LOW);
            }
        }
    }

    // 5. ACTUATOR MOVEMENT: Control the direction
    void ActuatorController::moveActuator(int direction) {
        // Direction 1 = Extend (Push out)
        if (direction == 1) { 
            digitalWrite(_laUp, HIGH);
            digitalWrite(_laDown, LOW);
        } 
        // Direction 2 = Retract (Pull in)
        else if (direction == 2) { 
            digitalWrite(_laUp, LOW);
            digitalWrite(_laDown, HIGH);
        } 
        // Anything else = Stop moving
        else { 
            digitalWrite(_laUp, LOW);
            digitalWrite(_laDown, LOW);
        }
    }