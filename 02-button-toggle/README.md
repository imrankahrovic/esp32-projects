# Button Toggle

One press of the button turns the LED on, the next press turns it off.
The LED keeps its state after the button is released.

## Components
- ESP32
- Pushbutton
- LED
- 220 Ω resistor (for the LED)
- 10 kΩ resistor (pull-down for the button)
- Breadboard and jumper wires

## Wiring

| Part | Connection |
|---|---|
| Button | 3V3 → button → D4 |
| Pull-down resistor | D4 → 10 kΩ → GND |
| LED | D5 → 220 Ω → LED (+) ... LED (−) → GND |

The LED's longer leg (anode, +) goes toward D5, the shorter leg to GND.

## How it works
- `buttonState` holds the current button reading, `lastButtonState` holds the reading from the previous pass of `loop()`.
- A **new press** is detected when the button is HIGH now but was LOW before. Holding the button does not trigger it again.
- `ledState = !ledState` flips the LED between on and off, and the result is written to the LED pin.
- `delay(50)` is a simple debounce: mechanical contacts bounce for a few milliseconds, and without the delay one press could be read as several.

## What I learned
- Remembering state between loop passes with variables
- Edge detection: reacting to the moment of a press, not to the pressed state
- Button bounce and a simple delay-based debounce
- Boolean logic (`&&`, `!`) in `if` conditions

## Source
Based on the Random Nerd Tutorials button example, extended with toggle logic and debounce:
https://RandomNerdTutorials.com/esp32-digital-inputs-outputs-arduino/