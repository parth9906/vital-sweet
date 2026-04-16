#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "HttpTypes.h"

class NetworkDispatcher {
private:
    static QueueHandle_t reqQ;
    static QueueHandle_t resQ;
    static uint8_t counter;

    static void task(void* pv);
    
public:
    static void begin();

    // async calls
    static uint8_t get(const char* url);
    static void post(const char* url, const char* json);

    // response reader (Core1)
    static bool read(HttpResponse &res);
};