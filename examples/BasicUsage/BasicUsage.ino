#include <Arduino.h>
#include "SimpleLED.h"

SimpleLED led(2);

void setup() {
    Serial.begin(115200);
    Serial.println("SimpleLED Basic Usage Example");
    
    // Basic on/off
    led.on();
    delay(1000);
    led.off();
    delay(1000);
    
    // Set brightness (0-255)
    led.setBrightness(128);
    led.on();
    delay(1000);
    
    // Start blinking
    led.setPattern(FAST_BLINK);
}

void loop() {
    led.update();
}