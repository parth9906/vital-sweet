#include <Arduino.h>
#include <esp_task_wdt.h>  // ESP32 Watchdog Library
#include "JeggeryProcessingMachine.hpp"
#include "Logger.h"

// Define Watchdog Timeout (5 seconds is safe for industrial tasks)
#define WDT_TIMEOUT_SECONDS 5

JeggeryProcessingMachine machine;
Logger &logger = Logger::getInstance();

void machineProcessTask(void *pvParameters) {
    // 1. Initialize Watchdog for this specific task
    esp_task_wdt_init(WDT_TIMEOUT_SECONDS, true); // true = panic/reboot on timeout
    esp_task_wdt_add(NULL); // NULL adds the current task (Core 1) to WDT monitoring

    // 2. Restore state from NVS
    machine.begin();
    logger.logf(Logger::INFO, "PROCESS_TASK", "Logic Task Started on Core 1");

    for (;;) {
        // 3. Reset (Pet) the Watchdog 
        // If machine.update() hangs longer than 5s, the system reboots
        esp_task_wdt_reset();

        // 4. Run State Machine Logic
        machine.update();

        // 5. Update Persistence
        machine.handleHeartbeat();

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void setup() {
    // Note: USB CDC (USB Serial) might not be ready immediately 
    // Add a small delay so you don't miss initial logs
    Serial.begin(115200);
    delay(1000); 

    logger.logf(Logger::INFO, "SYSTEM", "Starting Jeggery Machine...");

    // 6. START THE TASK (Missing in your snippet)
    xTaskCreatePinnedToCore(
        machineProcessTask,   /* Function */
        "JeggeryLogic",       /* Name */
        8192,                 /* Stack Size */
        NULL,                 /* Param */
        5,                    /* Priority */
        NULL,                 /* Handle */
        1                     /* Core 1 */
    );

    logger.logf(Logger::INFO, "SYSTEM", "Setup Complete. Task Dispatched.");
}

void loop() {
    // Standard loop on Core 0
    vTaskDelay(pdMS_TO_TICKS(1000)); 
}
