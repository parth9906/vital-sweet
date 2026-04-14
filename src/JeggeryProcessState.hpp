#pragma once
#include "Logger.h"

class JeggeryProcessingMachine; // Forward declaration

class JeggeryProcessState {
public:
    virtual ~JeggeryProcessState() = default;

    virtual void update(JeggeryProcessingMachine& machine) = 0;
    
    // Optional: Actions triggered by external buttons/events
    virtual void onStart(JeggeryProcessingMachine& machine) {}
    virtual void onEmergencyStop(JeggeryProcessingMachine& machine){}
};
