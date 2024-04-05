/**
 * @file GeigerTest.ino
 * @author Tyler Nielsen
 * @brief Example code for basic Might Ohm Geiger Counter Arduino library
 * @version 0.1
 * @date 2024-04-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "GeigerCounter.h"

#define GEIGER_PIN A2

GeigerCounter gc;

void setup(){
    Serial.begin(9600);
    while(!Serial);

    gc.begin(GEIGER_PIN);

    Serial.println("Beginning Geiger Counter Running Test...");
    delay(1000);
}

void loop(){
    Serial.print("CPS: "); Serial.println(gc.getCPSRunning());
    Serial.print("Dose: "); Serial.println(gc.getDoseRunning());

    delay(1000);
}