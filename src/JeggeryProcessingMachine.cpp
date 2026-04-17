#include "JeggeryProcessingMachine.hpp"
#include "ConcreteStates.hpp"

#define JEGGERY_PROCESSING_MACHINE_MODULE "JeggeryProcessingMachine"


JeggeryProcessingMachine::JeggeryProcessingMachine() : logger(Logger::getInstance()) {

    idleState = new IdleState();
    fillingState = new FillingState();
    boilingState = new BoilingState();
    oilSprayState = new OilSprayState();
    finishingState = new FinishingState();
    unloadingState = new UnloadingState();
    resettingState = new ResettingState();
    
    currentState = idleState;
}


void JeggeryProcessingMachine::begin() {
    prefs.begin("jeggery", false);
    
    int savedId = prefs.getInt("last_state", 0);

    // 1. Identify Restart Reason
    esp_reset_reason_t reason = esp_reset_reason();

    if (reason == ESP_RST_PANIC || reason == ESP_RST_WDT) {
        // This was a software crash/watchdog. 
        // We know exactly where we were. Auto-resume immediately.
        this->transitionTo(idToState(savedId));
        logger.logf(Logger::INFO, JEGGERY_PROCESSING_MACHINE_MODULE,"Software Recovery: Auto-Resuming...");
    } 
    else if (reason == ESP_RST_POWERON) {
        this->transitionTo(idleState);
        logger.logf(Logger::INFO, JEGGERY_PROCESSING_MACHINE_MODULE, "Cold Start: Reverting to IDLE for safety.");
    } else {
        this->transitionTo(idleState);
        logger.logf(Logger::INFO, JEGGERY_PROCESSING_MACHINE_MODULE, "Indefinite reason: %s", reason);
    }
}


void JeggeryProcessingMachine::update() {
    if (currentState) {
        currentState->update(*this);
    }
    static uint32_t lastLogMillis = 0;
    const uint32_t LOG_INTERVAL = 30000; // 30 seconds

    if (millis() - lastLogMillis >= LOG_INTERVAL) {
        logger.logf(Logger::INFO, "MACHINE", "Current State: %s", getCurrentStateName());
        lastLogMillis = millis();

        logger.logf(Logger::INFO, "CONFIG", 
                "Auto: %s | Target Brix: %.1f | Oil Temp: %.1f | Finish Temp: %.1f",
                config.isAutoModeEnabled ? "YES" : "NO",
                config.targetJaggeryBrix,
                config.oilSprayTemperatureThreshold,
                config.finishingPhaseTempThreshold
            );
    }
}


// Helper: Convert a State Pointer to a unique Integer ID
int JeggeryProcessingMachine::stateToId(JeggeryProcessState* state) {
    if (state == idleState)      return 0;
    if (state == fillingState)   return 1;
    if (state == boilingState)   return 2;
    if (state == oilSprayState)  return 3;
    if (state == finishingState) return 4;
    if (state == unloadingState) return 5;
    if (state == resettingState) return 6;
    return 0; // Default to Idle
}

// Helper: Convert a saved Integer ID back to a State Pointer
JeggeryProcessState* JeggeryProcessingMachine::idToState(int id) {
    switch (id) {
        case 0: return idleState;
        case 1: return fillingState;
        case 2: return boilingState;
        case 3: return oilSprayState;
        case 4: return finishingState;
        case 5: return unloadingState;
        case 6: return resettingState;
        default: return idleState;
    }
}

const char* JeggeryProcessingMachine::getCurrentStateName() {
    if (currentState == idleState) return "IDLE";
    if (currentState == fillingState) return "FILLING";
    if (currentState == boilingState) return "BOILING";
    if (currentState == oilSprayState) return "OIL_SPRAY";
    if (currentState == finishingState) return "FINISHING";
    if (currentState == unloadingState) return "UNLOADING";
    if (currentState == resettingState) return "RESETTING";
    return "UNKNOWN";
}

#include <ArduinoJson.h>

void JeggeryProcessingMachine::updateConfigFromJson(const char* json) {
    JsonDocument configs; 
    DeserializationError error = deserializeJson(configs, json);

    if (error) {
        Serial.printf("Config Update Failed: %s\n", error.c_str());
        return;
    }

    JsonVariant settingsObj = configs["settings"];

    config.updateFromJson(settingsObj);
    
    Serial.println("Machine Config Updated.");
}



void JeggeryProcessingMachine::setJuiceFillingValve(bool open) {
    // Add physical GPIO code here
    // digitalWrite(VALVE_PIN, open);
}

void JeggeryProcessingMachine::setHeater(int powerLevel) {
    // powerLevel: 0=Off, 1=Low, 2=Full
    // Add PWM or Relay control here
}

void JeggeryProcessingMachine::sprayOil() {
    // Trigger oil pump
}

void JeggeryProcessingMachine::setPanTilt(bool tilted) {
    // Trigger hydraulic/motor lift
}

void JeggeryProcessingMachine::handleHeartbeat() {
    // Your heartbeat logic
}

// In your main.cpp FreeRTOS Task:
// machine.update(); 
