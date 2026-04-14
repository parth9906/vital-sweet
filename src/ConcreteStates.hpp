#include "JeggeryProcessingMachine.hpp"
#include "Logger.h" // Ensure this is included

#define STATE_MODULE "PROCESS_STATE"

// 1. Declare the classes
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

class OilSprayState : public JeggeryProcessState {
public:
    void update(JeggeryProcessingMachine& machine) override;
};

class FinishingState : public JeggeryProcessState {
public:
    void update(JeggeryProcessingMachine& machine) override;
};

class UnloadingState : public JeggeryProcessState {
public:
    void update(JeggeryProcessingMachine& machine) override;
};

class ResettingState : public JeggeryProcessState {
public:
    void update(JeggeryProcessingMachine& machine) override;
};

// 2. Implementation using Logger::getInstance()

void IdleState::update(JeggeryProcessingMachine& machine) {
    machine.setHeater(0);
    machine.setValve(false);
    machine.setPanTilt(false);
    
    if (1) { // Process Start Trigger
        Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Starting Jaggery Cycle");
        machine.transitionTo(machine.fillingState);
        Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Juice filling started......");
    }
}

void FillingState::update(JeggeryProcessingMachine& machine) {
    float weight = machine.getWeight();
    machine.setValve(true);
    Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "filling Juice ......");

    if (weight >= 20.0f) machine.setHeater(2); 

    if (weight >= 50.0f) {
        Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Filling Complete. Weight: %.2f kg", weight);
        machine.setValve(false);
        Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Filling Complete. valve closed.");
        machine.transitionTo(machine.boilingState);
        Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Moving to boiling state.......");
    }
}

void BoilingState::update(JeggeryProcessingMachine& machine) {
    machine.setHeater(2); 
    if (machine.getTemperature() >= 110.0f) {
        Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Boiling Point Reached. Temperature: %.2f C", machine.getTemperature());
        machine.transitionTo(machine.oilSprayState);
    }
}

void OilSprayState::update(JeggeryProcessingMachine& machine) {
    Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Spraying Oil...");
    machine.sprayOil();
    Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Oil sprayed ...");
    machine.transitionTo(machine.finishingState);
}

void FinishingState::update(JeggeryProcessingMachine& machine) {
    machine.setHeater(1);
    float brix = machine.getBrix();
    float temperature = machine.getTemperature();
    Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Brix: %.1f, Temperature: %.1f", brix, temperature);
    if (brix >= 83.0f && temperature >= 115.0f) {
        Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Finishing Reached. Brix: %.1f", brix);
        machine.transitionTo(machine.unloadingState);
        Logger::getInstance().logf(Logger::WARN, STATE_MODULE, "Initiating Pan Tilt for Unloading");
    }
}

void UnloadingState::update(JeggeryProcessingMachine& machine) {
    machine.setHeater(0);
    machine.setPanTilt(true); 

    if (millis() - machine.stateStartTime >= 5000) { 
        Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Tilt Complete. Moving to Reset.");
        machine.transitionTo(machine.resettingState);
    }
}



void ResettingState::update(JeggeryProcessingMachine& machine) {
    machine.setPanTilt(false);

    // Wait for the return movement time
    if (millis() - machine.stateStartTime >= 8000) {
        Logger::getInstance().logf(Logger::INFO, STATE_MODULE, "Pan Home. Ready for next batch.");
        machine.transitionTo(machine.idleState);
    }
}



