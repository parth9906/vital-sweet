#pragma once
#include <Arduino.h>

enum HttpMethod {
    HTTP_GET,
    HTTP_POST
};

struct HttpRequest {
    HttpMethod method;
    char url[128];
    char payload[256];
    uint8_t id;
};

struct HttpResponse {
    uint8_t id;
    int code;
    char body[256];
};