/**
 * @file GeigerBlockingTest.ino
 * @author Tyler Nielsen
 * @brief 
 * @version 0.1
 * @date 2024-04-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "GeigerCounter.h"

#define GEIGER_PIN A2

#define SAMPLE_PERIOD 1000

GeigerCounter gc;

void setup(){
    Serial.begin(9600);
    while(!Serial);

    gc.begin(GEIGER_PIN);

    Serial.println("Beginning Geiger Counter Blocking Test...");
    delay(1000);
}

void loop(){
    Serial.print("CPS: "); Serial.println(gc.getCPSBlocking(SAMPLE_PERIOD));
    Serial.print("Dose: "); Serial.println(gc.getDoseBlocking(SAMPLE_PERIOD));

    delay(100);
}