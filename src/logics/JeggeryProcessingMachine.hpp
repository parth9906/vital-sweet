#pragma once
#include "JeggeryProcessState.hpp"
#include <Arduino.h>

class JeggeryProcessingMachine {
private:
    JeggeryProcessState* currentState;

public:
    // Pointers to pre-allocated states
    JeggeryProcessState* idleState;
    JeggeryProcessState* fillingState;
    JeggeryProcessState* boilingState;
    JeggeryProcessState* oilSprayState; // Fixed spelling
    JeggeryProcessState* finishingState; // 'Critical' usually implies error, renamed to finishing
    JeggeryProcessState* unloadingState;
    JeggeryProcessState* resettingState; // Renamed from ReadyToGo

    uint32_t processStartTime = 0; // Use uint32_t for millis()

    JeggeryProcessingMachine();
    
    void transitionTo(JeggeryProcessState* nextState) {
        if (nextState != nullptr) {
            currentState = nextState;
        }
    }

    void update() {
        if (currentState) currentState->update(*this);
    }
    
    // Industrial Actuator Methods
    void setValve(bool open);
    void setHeater(int powerLevel); // 0=Off, 1=Low, 2=Full
    void sprayOil();
    void setPanTilt(bool tilted);

    // Sensor Getters (Connect these to your actual sensor classes)
    float getTemperature();
    float getBrix();
    float getWeight();
};
