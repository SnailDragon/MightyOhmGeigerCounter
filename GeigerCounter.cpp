/**
 * @file GeigerCounter.cpp
 * @author Tyler Nielsen
 * @brief GeigerCounter class function implementation.
 * @version 0.1
 * @date 2024-04-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "GeigerCounter.h"

/**
 * @brief Construct a new GeigerCounter object, uses default minimum sample period of 1000 ms
 * 
 * @param geigerPin Pin that the data or "pulse" of the Geiger Counter is connected to 
 */
GeigerCounter::GeigerCounter(int8_t geigerPin){
    attachInterrupt(digitalPinToInterrupt(geigerPin), this->counter, FALLING);
    this->samplePeriodStart = millis();
    this->lastCPSMeasurement = 0; 
    this->minimumSamplePeriod_ms = 1000; 
}

/**
 * @brief Construct a new GeigerCounter object
 * 
 * @param geigerPin Pin that the data or "pulse" of the Geiger Counter is connected to 
 * @param minimumSamplePeriod_ms The minimum period for each CPS measurement in ms 
 */
GeigerCounter::GeigerCounter(int8_t geigerPin, unsigned long minimumSamplePeriod_ms){
    attachInterrupt(digitalPinToInterrupt(geigerPin), this->counter, FALLING);
    this->samplePeriodStart = millis();
    this->lastCPSMeasurement = 0; 
    this->minimumSamplePeriod_ms = minimumSamplePeriod_ms;
}

/**
 * @brief Calculates the Counts Per Second (CPS) with guarenteed minimum sample period
 * size. If the function is called sooner than the guarenteed size the last measurement 
 * is returned (the last measurement can be from getCPS() or getInstCPS() so be careful 
 * when using both together).
 * 
 * @return float - CPS
 */
float GeigerCounter::getCPS() {
    unsigned long samplePeriod = millis() - this->samplePeriodStart;
    float CPS = -1;

    if(samplePeriod < this->minimumSamplePeriod_ms){
        return this->lastCPSMeasurement; 
    }
    else {
        noInterrupts();
        CPS = STATIC_GEIGER_COUNT / (samplePeriod / 1000.0);
        STATIC_GEIGER_COUNT = 0;
        interrupts();

        this->samplePeriodStart = millis();
        
        this->lastCPSMeasurement = CPS; 
        return CPS;
    }
    
}

/**
 * @brief Calculates the Counts Per Second (CPS) without guarenteed minimum sample period 
 * size. Sampling this way too quickly can lead to inaccurate readings. 
 * 
 * @return float - CPS 
 */
float GeigerCounter::getInstCPS(){
    unsigned long samplePeriod = millis() - this->samplePeriodStart;
    float CPS = -1;
    noInterrupts();
    CPS = STATIC_GEIGER_COUNT / (samplePeriod / 1000.0);
    STATIC_GEIGER_COUNT = 0;
    interrupts();

    this->samplePeriodStart = millis();

    this->lastCPSMeasurement = CPS; 
    return CPS;
}

/**
 * @brief Calculates the measured dose in uSv/hr, using getCPS() with minimum sample 
 * period size. 
 * 
 * @return float - Dose in uSv/hr 
 */
float GeigerCounter::getDose() {
    float CPS = this->getCPS();

    return (CPS / 60.0) * SCALE_FACTOR;
}