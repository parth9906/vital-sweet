#pragma once
#include "JeggeryProcessState.hpp"
#include <Arduino.h>
#include <Preferences.h>
#include "Logger.h"
#include "MachineConfig.h"

static RTC_DATA_ATTR uint32_t lastSystemTimeBeforeReset; 

class JeggeryProcessingMachine {
private:
    Logger& logger;
    JeggeryProcessState* currentState;
    Preferences prefs;
    uint32_t restartThreshold = 60000;
    int stateToId(JeggeryProcessState* state);
    JeggeryProcessState* idToState(int id);
    uint32_t stateStartTime = 0;

public:
    JeggeryProcessState* idleState;
    JeggeryProcessState* fillingState;
    JeggeryProcessState* boilingState;
    JeggeryProcessState* oilSprayState;
    JeggeryProcessState* finishingState;
    JeggeryProcessState* unloadingState;
    JeggeryProcessState* resettingState;

    
    MachineConfig config;

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
    const char* getCurrentStateName();

    // Declare these (Implementation must be in .cpp)
    void setJuiceFillingValve(bool open);
    void setHeater(int powerLevel); 
    void sprayOil();
    void setPanTilt(bool tilted);

    // Sensor Getters (Keep inline or move to .cpp)
    float getTemperature() { return 0;}
    float getBrix() {return 0 ;}
    float getWeight() {return 0 ;}
    uint32_t getStateStartTime(){return stateStartTime;}
};

