#include <Arduino.h>
#include "TempManager.h"
#include "WeightManager.h"
#include "PowerMonitor.h"
#include "ActuatorController.h"
#include "pins.h"


TempManager temps(TEMP_BUS_PIN);
WeightManager weight;
PowerMonitor power(Serial2, 16, 17); // RX, TX
ActuatorController control(18, 19, 20, 21, 22, 23); // Relay, V1, V2, V3, LA_U, LA_D

// ESP32 usually has the internal LED on GPIO 2
const int ledPin = 2;

void setup() {
    Serial.begin(115200);
    temps.begin();
    weight.begin(LOADCELL_DOUT, LOADCELL_SCK);
    control.begin();
}

void loop() {
    // 1. Read everything
    float t1 = temps.getTempByIndex(0);
    float w = weight.getWeight();
    float p, f;
    power.readData(p, f);

    // 2. Logic (Example: Stop motor if weight > 10kg)
    if (w > 10.0) control.stopMotor();

    delay(500);
}