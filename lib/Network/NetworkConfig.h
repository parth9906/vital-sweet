    #pragma once
    #include <Arduino.h>

    namespace NetworkConfig {
        // Wi-Fi Credentials
        const char* const SSID = "Airprin_2.4G";
        const char* const PASS = "par798542";

        // Server Endpoints
        const String SERVER_IP = "192.168.1.5"; // Default Windows Hotspot IP
        const uint16_t PORT = 5000;
        
        const String BASE_URL = "http://" + SERVER_IP + ":" + String(PORT);
        const String LOG_URL = BASE_URL + "/log";
        const String SYNC_URL = BASE_URL + "/get_config";
        const String SENSOR_DATA_URL = BASE_URL + "/sensor_data";

        // Timing Settings
        const uint32_t TELEMETRY_INTERVAL_MS = 5000; // How often to send logs
        const uint32_t WIFI_TIMEOUT_MS = 10000;      // Connection timeout
    }
