#include "JeggeryProcessingMachine.hpp"

// 1. Declare the classes and their method signatures first
// Use 'public' inheritance so the Machine can see the update method
class IdleState : public JeggeryProcessState {
public:
    void update(JeggeryProcessingMachine& machine) override;
};

class FillingState : public JeggeryProcessState {
public:
    void update(JeggeryProcessingMachine& machine) override;
};

class BoilingState : public JeggeryProcessState {
public:
    void update(JeggeryProcessingMachine& machine) override;
};

class OilSprayState : public JeggeryProcessState { // Fixed name to match your machine
public:
    void update(JeggeryProcessingMachine& machine) override;
};

class FinishingState : public JeggeryProcessState { // Fixed name to match your machine
public:
    void update(JeggeryProcessingMachine& machine) override;
};

class UnloadingState : public JeggeryProcessState {
public:
    void update(JeggeryProcessingMachine& machine) override;
};

class ResettingState : public JeggeryProcessState { // Fixed name to match your machine
public:
    void update(JeggeryProcessingMachine& machine) override;
};

// 2. Now implement the methods

void IdleState::update(JeggeryProcessingMachine& machine) {
    machine.setHeater(0);
    machine.setValve(false);
    machine.setPanTilt(false);
    // Trigger to start:
    // if (machine.getStartButton()) machine.transitionTo(machine.fillingState);
}

void FillingState::update(JeggeryProcessingMachine& machine) {
    float weight = machine.getWeight();
    machine.setValve(true);

    if (weight >= 20.0f) {
        machine.setHeater(2); 
    }

    if (weight >= 50.0f) {
        machine.setValve(false);
        machine.transitionTo(machine.boilingState);
    }
}

void BoilingState::update(JeggeryProcessingMachine& machine) {
    machine.setHeater(2); 
    if (machine.getTemperature() >= 110.0f) {
        machine.transitionTo(machine.oilSprayState);
    }
}

void OilSprayState::update(JeggeryProcessingMachine& machine) {
    machine.sprayOil();
    machine.transitionTo(machine.finishingState);
}

void FinishingState::update(JeggeryProcessingMachine& machine) {
    machine.setHeater(1); 
    if (machine.getBrix() >= 83.0f && machine.getTemperature() >= 115.0f) {
        machine.transitionTo(machine.unloadingState);
    }
}

void UnloadingState::update(JeggeryProcessingMachine& machine) {
    machine.setHeater(0);
    machine.setPanTilt(true);
    machine.processStartTime = millis(); 
    machine.transitionTo(machine.resettingState);
}

void ResettingState::update(JeggeryProcessingMachine& machine) {
    if (millis() - machine.processStartTime >= 1000) {
        machine.setPanTilt(false);
        machine.transitionTo(machine.idleState);
    }
}
