/**
 * @file GeigerMinimumSamplePeriod.ino
 * @author Tyler Nielsen
 * @brief Example code for basic Might Ohm Geiger Counter Arduino library using custom minimum sample period
 * @version 0.1
 * @date 2024-04-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "GeigerCounter.h"

#define GEIGER_PIN A2
#define MINIMUM_SAMPLE_PERIOD_MS 5000

GeigerCounter gc(GEIGER_PIN, MINIMUM_SAMPLE_PERIOD_MS);

void setup(){
    Serial.begin(9600);
    while(!Serial);

    Serial.println("Beginning Geiger Counter Test...");
    delay(1000);
}

void loop(){
    Serial.print("CPS: "); Serial.println(gc.getCPSRunning());
    Serial.print("Dose: "); Serial.println(gc.getDoseRunning());

    delay(1000);
}