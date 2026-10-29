# Button Controls LED

The LED is on while the pushbutton is pressed and off when it is released.
The button state is also printed to the Serial Monitor.

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
- D4 is an input and reads the voltage from the button.
- D5 is an output that drives the LED.
- The 10 kΩ pull-down resistor keeps D4 at LOW when the button is not pressed.
  Without it the pin would "float" and read random values.
- `loop()` reads the button over and over: HIGH turns the LED on, LOW turns it off.

## What I learned
- Digital input and output (`pinMode`, `digitalRead`, `digitalWrite`)
- Why a floating input needs a pull-up or pull-down resistor
- Voltage, current and the role of a current-limiting resistor for an LED

## Source
Code based on the Random Nerd Tutorials example:
https://RandomNerdTutorials.com/esp32-digital-inputs-outputs-arduino/