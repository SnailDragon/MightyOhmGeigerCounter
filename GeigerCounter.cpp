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
 * @brief Construct a Geiger Counter object with default minimum sample period of 1000 ms. 
 * 
 * @param geigerPin Pin connected to the Geiger Counter's data or "pulse" output. Make sure that it is interrupt-enabled. 
 */
GeigerCounter::GeigerCounter(int8_t geigerPin){
    attachInterrupt(digitalPinToInterrupt(geigerPin), this->counter, FALLING);
    this->samplePeriodStart = millis();
    this->lastCPSMeasurement = 0; 
    this->minimumSamplePeriod_ms = 1000; 
}

/**
 * @brief Construct a Geiger Counter object with specified minimum sample period. 
 * 
 * @param geigerPin Pin connected to the Geiger Counter's data or "pulse" output (make sure that it is interrupt-enabled)
 * @param minimumSamplePeriod_ms Minimum sample period in ms for "running" functions
 */
GeigerCounter::GeigerCounter(int8_t geigerPin, unsigned long minimumSamplePeriod_ms){
    attachInterrupt(digitalPinToInterrupt(geigerPin), this->counter, FALLING);
    this->samplePeriodStart = millis();
    this->lastCPSMeasurement = 0; 
    this->minimumSamplePeriod_ms = minimumSamplePeriod_ms;
}

/**
 * @brief Calculates CPS for the specified sample period, starting at function call. 
 * 
 * @param samplePeriod_ms Period to sample over
 * @return float - CPS
 */
float GeigerCounter::getCPSBlocking(unsigned long samplePeriod_ms){
    this->refreshSample();
    delay(samplePeriod_ms);
    
    return this->getInstCPSRunning();
}

/**
 * @brief Calculates dose in uSv/hr for the specified sample period, starting at function call. 
 * 
 * @param samplePeriod_ms Period to sample over
 * @return float - Dose (uSv/hr)
 */
float GeigerCounter::getDoseBlocking(unsigned long samplePeriod_ms){
    float CPS = this->getCPSBlocking(samplePeriod_ms);

    return (CPS / 60.0) * SCALE_FACTOR;
}

/**
 * @brief Calculates CPS for the period since last CPS calculation. If the period single last CPS calculation is 
 * less than the minimum sample period (minimumSamplePeriod_ms, default 1000 ms) then the last calculated CPS 
 * measurement is returned. 
 * 
 * @return float - CPS
 */
float GeigerCounter::getCPSRunning() {
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
 * @brief Calculates CPS for the period since last CPS calculation. Does NOT check if the period
 * since last CPS calculation is more than the minimum sample period (minimumSamplePeriod_ms). 
 * Due to that, calling this function too frequently will cause inaccurate readings.  
 * 
 * @return float - CPS 
 */
float GeigerCounter::getInstCPSRunning(){
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
 * @brief Calculates dose based on CPS for the period since last CPS calcuation. Uses getCPSRunning()
 * so the minimum sample period is enforced. 
 * 
 * @return float - Dose (uSv/hr)
 */
float GeigerCounter::getDoseRunning() {
    float CPS = this->getCPSRunning();

    return (CPS / 60.0) * SCALE_FACTOR;
}

/**
 * @brief Calculates dose based on CPS for the period since last CPS calcuation. Uses getInstCPSRunning()
 * so the minimum sample period is NOT enforced. 
 * 
 * @return float - Dose (uSv/hr)
 */
float GeigerCounter::getInstDoseRunning() {
    float CPS = this->getInstCPSRunning();

    return (CPS / 60.0) * SCALE_FACTOR;
}

/**
 * @brief Refreshes sample period, resetting STATIC_GEIGER_COUNT and samplePeriodStart.
 * 
 */
void GeigerCounter::refreshSample(){

    noInterrupts();

    STATIC_GEIGER_COUNT = 0;

    interrupts();

    this->samplePeriodStart = millis();
}