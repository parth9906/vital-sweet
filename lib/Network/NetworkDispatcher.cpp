#include "NetworkDispatcher.h"
#include "NetworkConfig.h"
#include "Logger.h"
#define NET_MODULE "NET_MODULE"

QueueHandle_t NetworkDispatcher::reqQ = NULL;
QueueHandle_t NetworkDispatcher::resQ = NULL;
uint8_t NetworkDispatcher::counter = 1;

void NetworkDispatcher::task(void* pv) {
    HTTPClient http;

    for (;;) {

    
        if (WiFi.status() != WL_CONNECTED) {
            Logger::getInstance().log(Logger::WARN, NET_MODULE, "WiFi lost, reconnecting");
            WiFi.begin(NetworkConfig::SSID, NetworkConfig::PASS);
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        HttpRequest req;

        if (xQueueReceive(reqQ, &req, pdMS_TO_TICKS(100))) {

            Logger::getInstance().logf(Logger::DEBUG, NET_MODULE, "REQ id=%d url=%s", req.id, req.url);
            http.begin(req.url);
            http.setTimeout(3000);

            int code;

            if (req.method == HTTP_POST) {
                http.addHeader("Content-Type", "application/json");
                code = http.POST((uint8_t*)req.payload, strlen(req.payload));
            } else {
                code = http.GET();
            }

            HttpResponse res;
            res.id = req.id;
            res.code = code;

            if (code > 0) {
                String body = http.getString();
                strncpy(res.body, body.c_str(), sizeof(res.body));
                Logger::getInstance().logf(Logger::INFO, NET_MODULE, "RES id=%d code=%d",
                            res.id, res.code);
            } else {
                res.body[0] = '\0';
                Logger::getInstance().logf(Logger::ERROR, "NET",
                            "HTTP FAIL id=%d",
                            req.id);
            }

            xQueueSend(resQ, &res, 0);

            http.end();
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void NetworkDispatcher::begin() {
    reqQ = xQueueCreate(20, sizeof(HttpRequest));
    resQ = xQueueCreate(20, sizeof(HttpResponse));

    xTaskCreatePinnedToCore(
        task,
        "net",
        8192,
        NULL,
        2,
        NULL,
        0
    );
}

uint8_t NetworkDispatcher::get(const char* url) {
    HttpRequest r;
    r.method = HTTP_GET;
    r.id = counter++;

    strncpy(r.url, url, sizeof(r.url));
    r.payload[0] = '\0';

    xQueueSend(reqQ, &r, 0);

    return r.id;
}

void NetworkDispatcher::post(const char* url, const char* json) {
    HttpRequest r;
    r.method = HTTP_POST;
    r.id = counter++;

    strncpy(r.url, url, sizeof(r.url));
    strncpy(r.payload, json, sizeof(r.payload));

    xQueueSend(reqQ, &r, 0);
}

bool NetworkDispatcher::read(HttpResponse &res) {
    return xQueueReceive(resQ, &res, 0) == pdTRUE;
}