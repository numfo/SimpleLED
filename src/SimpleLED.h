#ifndef SIMPLE_LED_H
#define SIMPLE_LED_H

#include <Arduino.h>

enum BlinkPattern{
    SOLID,
    SLOW_BLINK,
    FAST_BLINK,
    HEARTBEAT,
    FADE,
    STROBE,
    CUSTOM
};
class SimpleLED{
    private:
        int pin;
        bool state;
        int brightness;
        bool isPWMCapable;


        // New blinking Variables
        bool isBlinking;
        unsigned long previousMillis;
        unsigned long blinkInterval;
        unsigned long blinkDuration;
        unsigned long blinkStartTime;
        BlinkPattern currentPattern;

        //pattern-specific variables
        int fadeDirection;
        int fadeStep;
        int strobeCount;
        int strobeMax;

        // helper methods
        void applyBrightness();
        void updatePattern();
        bool isPWMPin(int pin);

    
        public:
            SimpleLED(int ledPin);
            void on();
            void off();
            void toggle();
            bool isOn();


        // New Blinking methods
        void startBlinking(unsigned long interval);
        void startBlinking(unsigned long interval, unsigned long duration);
        void stopBlinking();

        // Brightness control
        void setBrightness(int brightness);  // 0-255
        int getBrightness();
        void dim(int amount);
        void brighten(int amount);
        
        // Pattern control
        void setPattern(BlinkPattern pattern);
        void setPattern(BlinkPattern pattern, unsigned long interval);
        void setCustomPattern(unsigned long interval);
        void stopPattern();



        void update();



};



// Multi-LED Controller Class
class LEDController {
    private:
        SimpleLED* leds[3];
        int ledCount;
    
    public:
        LEDController(int pin1, int pin2, int pin3);
        ~LEDController();
        
        // Control individual LEDs
        SimpleLED& led(int index);
        
        // Control all LEDs
        void allOn();
        void allOff();
        void allSetBrightness(int brightness);
        void allSetPattern(BlinkPattern pattern);
        
        // Sequence patterns
        void chase(unsigned long interval);
        void wave(unsigned long interval);
        void alternating(unsigned long interval);
        
        // Main update method
        void update();
    };



#endif // SIMPLE_LED_H