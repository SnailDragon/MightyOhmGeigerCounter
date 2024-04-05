/**
 * @file GeigerCounter.h
 * @author Tyler Nielsen
 * @brief GeigerCounter class definition for interfacing with the "pulse" out of a Mighty Ohm Geiger Counter.
 * @version 0.1
 * @date 2024-04-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef GEIGER_COUNTER_H
#define GEIGER_COUNTER_H

#include "Arduino.h"
#include <stdint.h>

#define SCALE_FACTOR 57

static volatile uint32_t STATIC_GEIGER_COUNT = 0; 

class GeigerCounter {

    private:
        unsigned long minimumSamplePeriod_ms; 
        float lastCPSMeasurement;
        unsigned long samplePeriodStart; 
        static void counter(){
            STATIC_GEIGER_COUNT++;
        }
    public:
        GeigerCounter(int8_t geigerPin);
        GeigerCounter(int8_t geigerPin, unsigned long minimumSamplePeriod_ms);
        float getCPS();
        float getInstCPS();
        float getDose();

};

#endif