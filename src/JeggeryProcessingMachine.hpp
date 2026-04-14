#pragma once
#include "JeggeryProcessState.hpp"
#include <Arduino.h>
#include <Preferences.h>
#include "Logger.h"

static RTC_DATA_ATTR uint32_t lastSystemTimeBeforeReset; 

class JeggeryProcessingMachine {
private:
    Logger& logger;
    JeggeryProcessState* currentState;
    Preferences prefs;
    uint32_t restartThreshold = 60000;
    int stateToId(JeggeryProcessState* state);
    JeggeryProcessState* idToState(int id);


    float weight = 0;
    float brix = 0;
    float temp = 0;

public:
    JeggeryProcessState* idleState;
    JeggeryProcessState* fillingState;
    JeggeryProcessState* boilingState;
    JeggeryProcessState* oilSprayState;
    JeggeryProcessState* finishingState;
    JeggeryProcessState* unloadingState;
    JeggeryProcessState* resettingState;

    uint32_t stateStartTime = 0;

    JeggeryProcessingMachine();

    void begin();
    
    void transitionTo(JeggeryProcessState* nextState) {
        if (nextState != nullptr) {
            currentState = nextState;
            prefs.putInt("last_state", stateToId(nextState)); 
            stateStartTime = millis();
        }
    }

    void update();
    void handleHeartbeat();

    // Declare these (Implementation must be in .cpp)
    void setValve(bool open);
    void setHeater(int powerLevel); 
    void sprayOil();
    void setPanTilt(bool tilted);

    // Sensor Getters (Keep inline or move to .cpp)
    float getTemperature() { return temp += 2; }
    float getBrix() { return brix += 2; }
    float getWeight() { return weight += 5; }
};
