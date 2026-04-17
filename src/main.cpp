#include <Arduino.h>
#include <esp_task_wdt.h>  // ESP32 Watchdog Library
#include "JeggeryProcessingMachine.hpp"
#include "Logger.h"
#include "NetworkDispatcher.h"
#include "NetworkConfig.h"
#include "WeightManager.h"

// Define Watchdog Timeout (5 seconds is safe for industrial tasks)
#define WDT_TIMEOUT_SECONDS 10
void syncConfiguration(); 

JeggeryProcessingMachine machine;
WeightManager leftScale("left_side", -7050.0f);
WeightManager rightScale("right_side", -7100.0f);
Logger &logger = Logger::getInstance();

void machineProcessTask(void *pvParameters) {
    esp_task_wdt_init(WDT_TIMEOUT_SECONDS, true); // true = panic/reboot on timeout
    esp_task_wdt_add(NULL); // NULL adds the current task (Core 1) to WDT monitoring
    machine.begin();
    logger.logf(Logger::INFO, "PROCESS_TASK", "Logic Task Started on Core 1");

    for (;;) {
        esp_task_wdt_reset();
        machine.update();
        syncConfiguration();
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void setup() {
    Serial.begin(115200);

    logger.setLevel(Logger::INFO);
    logger.logf(Logger::INFO, "SYSTEM", "Starting Jeggery Machine...");
    NetworkDispatcher::begin();

    xTaskCreatePinnedToCore(
        machineProcessTask,   /* Function */
        "JeggeryLogic",       /* Name */
        8192,                 /* Stack Size */
        NULL,                 /* Param */
        5,                    /* Priority */
        NULL,                 /* Handle */
        1                     /* Core 1 */
    );

    if (leftScale.begin(5, 4)) logger.logf(Logger::INFO, "SYSTEM","Left Scale Online");
    if (rightScale.begin(18, 19)) logger.logf(Logger::INFO, "SYSTEM","Right Scale Online");

    logger.logf(Logger::INFO, "SYSTEM", "Setup Complete. Task Dispatched.");
}

void loop() {

    float w1 = leftScale.getStableWeight();
    float w2 = rightScale.getStableWeight();
    vTaskDelay(pdMS_TO_TICKS(1000)); 
}



void syncConfiguration() {
    static uint32_t lastSyncMillis = 0;
    static uint8_t pendingRequestId = 0;
    static bool isRequestActive = false;
    
    const uint32_t SYNC_INTERVAL = 30000; // 30 seconds

    // Phase 1: Send Request
    if (!isRequestActive && (millis() - lastSyncMillis > SYNC_INTERVAL)) {
        pendingRequestId = NetworkDispatcher::get(NetworkConfig::SYNC_URL.c_str());
        isRequestActive = true;
        lastSyncMillis = millis();
    }

    // Phase 2: Poll for Response
    HttpResponse res;
    if (NetworkDispatcher::read(res)) {
        if (res.id == pendingRequestId) {
            if (res.code == 200) {
                machine.updateConfigFromJson(res.body);
            } else {
                Serial.printf("Sync failed: %d\n", res.code);
            }
            isRequestActive = false; // Reset for next cycle
        }
    }
}
