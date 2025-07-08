#include <Arduino.h>
#include "SimpleLED.h"

LEDController controller(2, 4, 5);

void setup() {
    Serial.begin(115200);
    Serial.println("SimpleLED Advanced Patterns Example");
    
    // Set different patterns for each LED
    controller.led(0).setPattern(HEARTBEAT);
    controller.led(1).setPattern(FADE);
    controller.led(2).setPattern(STROBE);
}

void loop() {
    static unsigned long lastChange = 0;
    static int mode = 0;
    
    controller.update();
    
    // Change modes every 10 seconds
    if (millis() - lastChange > 10000) {
        lastChange = millis();
        mode = (mode + 1) % 3;
        
        switch (mode) {
            case 0:
                Serial.println("Chase mode");
                controller.allOff();
                break;
            case 1:
                Serial.println("Wave mode");
                break;
            case 2:
                Serial.println("Alternating mode");
                break;
        }
    }
    
    // Execute current mode
    switch (mode) {
        case 0:
            controller.chase(300);
            break;
        case 1:
            controller.wave(400);
            break;
        case 2:
            controller.alternating(500);
            break;
    }
}