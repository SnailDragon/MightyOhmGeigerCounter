/**
 * @file GeigerTest.ino
 * @author Tyler Nielsen
 * @brief Test out of getCPS() and getDose()
 * @version 0.1
 * @date 2024-04-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "GeigerCounter.h"

#define GEIGER_PIN A2

GeigerCounter gc(GEIGER_PIN);

void setup(){
    Serial.begin(9600);
    while(!Serial);

    Serial.println("Beginning Geiger Counter Test...");
}

void loop(){
    Serial.print("CPS: "); Serial.println(gc.getCPS());
    Serial.print("Dose: "); Serial.println(gc.getDose());

    delay(1000);
}