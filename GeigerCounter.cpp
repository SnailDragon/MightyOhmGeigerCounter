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

GeigerCounter::GeigerCounter(int8_t geigerPin){
    attachInterrupt(digitalPinToInterrupt(geigerPin), this->counter, FALLING);
    this->samplePeriodStart = millis();
    this->lastCPSMeasurement = 0; 
    this->minimumSamplePeriod_ms = 1000; 
}

GeigerCounter::GeigerCounter(int8_t geigerPin, unsigned long minimumSamplePeriod_ms){
    attachInterrupt(digitalPinToInterrupt(geigerPin), this->counter, FALLING);
    this->samplePeriodStart = millis();
    this->lastCPSMeasurement = 0; 
    this->minimumSamplePeriod_ms = minimumSamplePeriod_ms;
}

float GeigerCounter::getCPSBlocking(unsigned long samplePeriod_ms){
    this->refreshSample();
    delay(samplePeriod_ms);
    
    return this->getInstCPSRunning();
}

float GeigerCounter::getDoseBlocking(unsigned long samplePeriod_ms){
    float CPS = this->getCPSBlocking(samplePeriod_ms);

    return (CPS / 60.0) * SCALE_FACTOR;
}

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


float GeigerCounter::getDoseRunning() {
    float CPS = this->getCPSRunning();

    return (CPS / 60.0) * SCALE_FACTOR;
}

void GeigerCounter::refreshSample(){

    noInterrupts();

    STATIC_GEIGER_COUNT = 0;

    interrupts();

    this->samplePeriodStart = millis();
}