// TempManager.cpp
#include "TempManager.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include "Logger.h"

#define TEMPRETURE_LOG_MODULE "TempManager"

const float TEMP_DISCONNECTED = -127.0;
const float TEMP_POWER_ON_GLITCH = 85.0;
const float TEMP_MAX_VALID = 125.0;
const float BUBBLE_DEVIATION_THRESHOLD = 1.5;

TempManager::TempManager(int dataPin)
    : oneWire(dataPin), sensors(&oneWire), sensorCount(0), logger(Logger::getInstance()) {}

void TempManager::begin()
{
    sensors.begin();
    sensorCount = sensors.getDeviceCount();

    logger.logf(Logger::INFO, TEMPRETURE_LOG_MODULE, "Initialized with %d sensors", sensorCount);
}

float TempManager::getTempByIndex(int sensorIndex)
{
    sensors.requestTemperatures();
    float temperature = sensors.getTempCByIndex(sensorIndex);

    logger.logf(Logger::DEBUG, TEMPRETURE_LOG_MODULE,  "Sensor %d reading: %.2f°C", sensorIndex, temperature);
    return temperature;
}

float TempManager::getAverageTemp()
{
    sensors.requestTemperatures();

    std::vector<float> validTemperatures;

    for (int i = 0; i < sensorCount; i++)
    {
        float currentTemp = sensors.getTempCByIndex(i);

        logger.logf(Logger::DEBUG, TEMPRETURE_LOG_MODULE, "Raw reading (Sensor %d): %.2f°C", i, currentTemp);
        // 1. Check disconnected sensor
        if (currentTemp == TEMP_DISCONNECTED || std::isnan(currentTemp))
        {
            logger.logf(Logger::WARN, TEMPRETURE_LOG_MODULE, "Sensor %d disconnected or invalid", i);
            continue;
        }

        // 2. Handle 85°C power-on glitch
        if (currentTemp == TEMP_POWER_ON_GLITCH)
        {
            int neighborIndex = (i == 0) ? 1 : 0;

            if (neighborIndex < sensorCount)
            {
                float neighborTemp = sensors.getTempCByIndex(neighborIndex);

                if (neighborTemp != TEMP_DISCONNECTED &&
                    !std::isnan(neighborTemp) &&
                    std::abs(currentTemp - neighborTemp) > 10.0)
                {

                    logger.logf(Logger::WARN, TEMPRETURE_LOG_MODULE, "Ignoring 85°C glitch on sensor %d (neighbor: %.2f°C)", i, neighborTemp);
                    continue;
                }
            }
        }

        // 3. Check physical limit
        if (currentTemp <= TEMP_MAX_VALID)
        {
            validTemperatures.push_back(currentTemp);
        }
        else
        {
            logger.logf(Logger::ERROR, TEMPRETURE_LOG_MODULE, "Sensor %d exceeded max limit: %.2f°C", i, currentTemp);
        }
    }

    int validCount = validTemperatures.size();

    if (validCount == 0)
    {
        logger.log(Logger::ERROR, TEMPRETURE_LOG_MODULE, "No valid temperature readings");
        return TEMP_DISCONNECTED;
    }

    // STEP 2: Handle bubble / outlier filtering
    if (validCount >= 3)
    {
        std::sort(validTemperatures.begin(), validTemperatures.end());

        float medianTemp = validTemperatures[validCount / 2];
        float filteredSum = 0.0;
        int filteredCount = 0;

        logger.logf(Logger::DEBUG, TEMPRETURE_LOG_MODULE, "Median temperature: %.2f°C", medianTemp);

        for (float temp : validTemperatures)
        {
            if (std::abs(temp - medianTemp) > BUBBLE_DEVIATION_THRESHOLD)
            {
                logger.logf(Logger::WARN, TEMPRETURE_LOG_MODULE, "Outlier detected (possible bubble): %.2f°C", temp);
            }
        }

        return medianTemp;
    }

    // STEP 3: Fallback (1–2 sensors)
    float simpleSum = 0.0;

    for (float temp : validTemperatures)
    {
        simpleSum += temp;
    }

    float average = simpleSum / validCount;

    logger.logf(Logger::DEBUG, TEMPRETURE_LOG_MODULE, "Simple average: %.2f°C (from %d sensors)", average, validCount);

    return average;
}