#include "SimpleLED.h"

SimpleLED::SimpleLED(int ledPin) {
    pin = ledPin;
    state = false;
    brightness = 255;
    isPWMCapable = isPWMPin(pin);
    
    isBlinking = false;
    previousMillis = 0;
    blinkInterval = 500;
    blinkDuration = 0;
    blinkStartTime = 0;
    currentPattern = SOLID;
    
    fadeDirection = 1;
    fadeStep = 5;
    strobeCount = 0;
    strobeMax = 5;
    
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

bool SimpleLED::isPWMPin(int pin) {
    // ESP32 can do PWM on most pins, but let's define common ones
    int pwmPins[] = {2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};
    for (int i = 0; i < sizeof(pwmPins) / sizeof(pwmPins[0]); i++) {
        if (pin == pwmPins[i]) return true;
    }
    return false;
}

void SimpleLED::applyBrightness() {
    if (isPWMCapable && brightness < 255) {
        analogWrite(pin, state ? brightness : 0);
    } else {
        digitalWrite(pin, state ? HIGH : LOW);
    }
}

void SimpleLED::on() {
    isBlinking = false;
    currentPattern = SOLID;
    state = true;
    applyBrightness();
}

void SimpleLED::off() {
    isBlinking = false;
    currentPattern = SOLID;
    state = false;
    applyBrightness();
}

void SimpleLED::toggle() {
    isBlinking = false;
    currentPattern = SOLID;
    state = !state;
    applyBrightness();
}

bool SimpleLED::isOn(){
    return state;
}

void SimpleLED::setBrightness(int newBrightness) {
    brightness = constrain(newBrightness, 0, 255);
    applyBrightness();
}

int SimpleLED::getBrightness() {
    return brightness;
}

void SimpleLED::dim(int amount) {
    setBrightness(brightness - amount);
}

void SimpleLED::brighten(int amount) {
    setBrightness(brightness + amount);
}

void SimpleLED::setPattern(BlinkPattern pattern) {
    currentPattern = pattern;
    isBlinking = true;
    previousMillis = millis();
    blinkStartTime = millis();
    strobeCount = 0;
    
    switch (pattern) {
        case SLOW_BLINK:
            blinkInterval = 1000;
            break;
        case FAST_BLINK:
            blinkInterval = 200;
            break;
        case HEARTBEAT:
            blinkInterval = 100;
            break;
        case FADE:
            blinkInterval = 50;
            fadeDirection = 1;
            break;
        case STROBE:
            blinkInterval = 50;
            strobeMax = 5;
            break;
        case SOLID:
            isBlinking = false;
            on();
            break;
        default:
            blinkInterval = 500;
            break;
    }
}

void SimpleLED::setPattern(BlinkPattern pattern, unsigned long interval) {
    setPattern(pattern);
    blinkInterval = interval;
}

void SimpleLED::setCustomPattern(unsigned long interval) {
    currentPattern = CUSTOM;
    blinkInterval = interval;
    isBlinking = true;
    previousMillis = millis();
}

void SimpleLED::stopPattern() {
    isBlinking = false;
    currentPattern = SOLID;
    off();
}


void SimpleLED::updatePattern() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= blinkInterval) {
        previousMillis = currentMillis;
        
        switch (currentPattern) {
            case SLOW_BLINK:
            case FAST_BLINK:
            case CUSTOM:
                toggle();
                break;
                
            case HEARTBEAT:
                if (strobeCount < 2) {
                    toggle();
                    strobeCount++;
                    blinkInterval = 100;
                } else {
                    strobeCount = 0;
                    blinkInterval = 800;
                    state = false;
                    applyBrightness();
                }
                break;
                
            case FADE:
                if (!isPWMCapable) {
                    toggle(); // Fallback to blinking
                } else {
                    brightness += fadeDirection * fadeStep;
                    if (brightness >= 255) {
                        brightness = 255;
                        fadeDirection = -1;
                    } else if (brightness <= 0) {
                        brightness = 0;
                        fadeDirection = 1;
                    }
                    state = true;
                    applyBrightness();
                }
                break;
                
            case STROBE:
                if (strobeCount < strobeMax) {
                    toggle();
                    strobeCount++;
                    blinkInterval = 50;
                } else {
                    strobeCount = 0;
                    blinkInterval = 1000;
                    state = false;
                    applyBrightness();
                }
                break;
                
            default:
                break;
        }
    }
}


void SimpleLED::startBlinking(unsigned long interval)
{
    isBlinking = true;
    blinkInterval = interval;
    blinkDuration = 0;
    previousMillis = millis();
    blinkStartTime = millis();

}

void SimpleLED::startBlinking(unsigned long interval, unsigned long duration) {
    isBlinking = true;
    blinkInterval = interval;
    blinkDuration = duration;
    previousMillis = millis();
    blinkStartTime = millis();
}

void SimpleLED::stopBlinking() {
    isBlinking = false;
    off();
}

void SimpleLED::update() {
    if (!isBlinking) return;
    
    unsigned long currentMillis = millis();
    
    // Check if blinking should stop (for timed blinking)
    if (blinkDuration > 0 && (currentMillis - blinkStartTime) >= blinkDuration) {
        stopPattern();
        return;
    }
    
    updatePattern();
}



// LEDController Implementation
LEDController::LEDController(int pin1, int pin2, int pin3) {
    ledCount = 3;
    leds[0] = new SimpleLED(pin1);
    leds[1] = new SimpleLED(pin2);
    leds[2] = new SimpleLED(pin3);
}

LEDController::~LEDController() {
    for (int i = 0; i < ledCount; i++) {
        delete leds[i];
    }
}

SimpleLED& LEDController::led(int index) {
    return *leds[constrain(index, 0, ledCount - 1)];
}

void LEDController::allOn() {
    for (int i = 0; i < ledCount; i++) {
        leds[i]->on();
    }
}

void LEDController::allOff() {
    for (int i = 0; i < ledCount; i++) {
        leds[i]->off();
    }
}

void LEDController::allSetBrightness(int brightness) {
    for (int i = 0; i < ledCount; i++) {
        leds[i]->setBrightness(brightness);
    }
}

void LEDController::allSetPattern(BlinkPattern pattern) {
    for (int i = 0; i < ledCount; i++) {
        leds[i]->setPattern(pattern);
    }
}

void LEDController::chase(unsigned long interval) {
    static unsigned long lastChase = 0;
    static int chaseIndex = 0;
    
    if (millis() - lastChase >= interval) {
        lastChase = millis();
        
        // Turn off all LEDs
        allOff();
        
        // Turn on current LED
        leds[chaseIndex]->on();
        
        // Move to next LED
        chaseIndex = (chaseIndex + 1) % ledCount;
    }
}
void LEDController::wave(unsigned long interval) {
    static unsigned long lastWave = 0;
    static int waveIndex = 0;
    static int waveDirection = 1;
    
    if (millis() - lastWave >= interval) {
        lastWave = millis();
        
        // Turn off all LEDs
        allOff();
        
        // Turn on current LED
        leds[waveIndex]->on();
        
        // Move wave
        waveIndex += waveDirection;
        if (waveIndex >= ledCount - 1) {
            waveDirection = -1;
        } else if (waveIndex <= 0) {
            waveDirection = 1;
        }
    }
}

void LEDController::alternating(unsigned long interval) {
    static unsigned long lastAlt = 0;
    static bool altState = false;
    
    if (millis() - lastAlt >= interval) {
        lastAlt = millis();
        altState = !altState;
        
        for (int i = 0; i < ledCount; i++) {
            if ((i % 2 == 0) == altState) {
                leds[i]->on();
            } else {
                leds[i]->off();
            }
        }
    }
}

void LEDController::update() {
    for (int i = 0; i < ledCount; i++) {
        leds[i]->update();
    }
}