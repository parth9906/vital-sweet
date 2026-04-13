#include "JeggeryProcessingMachine.hpp"
#include "ConcreteStates.hpp"

JeggeryProcessingMachine::JeggeryProcessingMachine() {
    // Correctly initialize member pointers
    idleState = new IdleState();
    fillingState = new FillingState();
    boilingState = new BoilingState();
    unloadingState = new UnloadingState();
    iolSpayState = new IolSpayState();
    criticalState = new CriticalState();
    readyToGoState = new ReadyToGoState();

    // Set initial state
    currentState = idleState;
}

// In your main.cpp FreeRTOS Task:
// machine.update(); 
