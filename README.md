# SimpleLED Library

An advanced LED control library for Arduino/ESP32 with brightness control, multiple blink patterns, and multi-LED coordination.

## Features

- **Individual LED Control**: On/off, toggle, brightness control
- **Multiple Blink Patterns**: Heartbeat, fade, strobe, custom patterns
- **Multi-LED Support**: Control multiple LEDs with coordinated effects
- **Non-blocking**: All operations are non-blocking using millis()
- **PWM Support**: Automatic brightness control on PWM-capable pins

## Installation

### Method 1: Clone Repository

```bash
git clone https://github.com/numfo/SimpleLED.git
```

Copy the SimpleLED folder to your PlatformIO lib directory.

### Method 2: PlatformIO

Add to your platformio.ini:
lib_deps = https://github.com/numfo/SimpleLED.git

## Quick Start

#include "SimpleLED.h"

SimpleLED led(2); // Pin 2

void setup() {
led.setPattern(HEARTBEAT);
}

void loop() {
led.update(); // Must be called regularly
}

## API Reference

- **SimpleLED Class**

### Basic Control

- **SimpleLED(int pin)** - Constructor
- **void on()** - Turn LED on
- **void off()** - Turn LED off
- **void toggle()** - Toggle LED state
- **bool isOn()** - Check if LED is on

Brightness Control

void setBrightness(int brightness) - Set brightness (0-255)
int getBrightness() - Get current brightness
void dim(int amount) - Decrease brightness
void brighten(int amount) - Increase brightness

Pattern Control

void setPattern(BlinkPattern pattern) - Set blink pattern
void setPattern(BlinkPattern pattern, unsigned long interval) - Set pattern with custom interval
void stopPattern() - Stop current pattern
void update() - Update pattern state (call in loop)

Available Patterns

SOLID - Always on
SLOW_BLINK - Slow blinking
FAST_BLINK - Fast blinking
HEARTBEAT - Double blink pattern
FADE - Fade in/out (PWM pins only)
STROBE - Strobe effect
CUSTOM - Custom interval blinking

LEDController Class
Constructor

LEDController(int pin1, int pin2, int pin3) - Control 3 LEDs

Individual Control

SimpleLED& led(int index) - Access individual LED (0-2)

Group Control

void allOn() - Turn all LEDs on
void allOff() - Turn all LEDs off
void allSetBrightness(int brightness) - Set brightness for all LEDs
void allSetPattern(BlinkPattern pattern) - Set pattern for all LEDs

Sequence Effects

void chase(unsigned long interval) - Chase effect
void wave(unsigned long interval) - Wave effect
void alternating(unsigned long interval) - Alternating effect
void update() - Update all LEDs

## Hardware Requirements

- Arduino/ESP32 compatible board
- LEDs with appropriate resistors
- PWM-capable pins for brightness control (optional)

## License

MIT License - see LICENSE file for details.
Contributing

## Fork the repository

1. Create a feature branch
2. Make your changes
3. Submit a pull request

## Changelog

### v1.0.0

- Initial release
- Basic LED control
- Multiple blink patterns
- Multi-LED support
- Brightness control
- Non-blocking operation
