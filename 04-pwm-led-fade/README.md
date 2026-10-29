# PWM LED Fade

The LED slowly brightens and then dims in a loop (a "breathing" effect), using PWM.

## Components
- ESP32
- LED
- 220 Ω resistor
- Breadboard and jumper wires

## Wiring

| Part | Connection |
|---|---|
| LED | D5 → 220 Ω → LED (+) ... LED (−) → GND |

The LED's longer leg (anode, +) goes toward D5, the shorter leg to GND.

## How it works
- A pin cannot output "half voltage", but it can switch on and off very fast. The eye sees the average as a dimmer light.
- The share of time the pin is on in each cycle is the **duty cycle**. The longer it is on, the brighter the LED.
- `ledcAttach(pin, frequency, resolution)` sets up PWM on the pin: 5000 Hz, 8 bits (values from 0 to 255).
- `ledcWrite(pin, value)` sets the brightness: 0 is off, 255 is full.
- Two `for` loops count the brightness up and then down, with `delay(20)` between steps. One fade takes about 5 seconds.

## Things I tried
- A very low frequency (about 5 Hz) makes the flicker visible, which shows what PWM really does.
- More resolution bits (for example 10) means more brightness steps, so the loop limit must match the maximum value (1023).

## Requirements
- ESP32 Arduino core **3.x**. Older 2.x versions use `ledcSetup` and `ledcAttachPin` instead of `ledcAttach`.

## What I learned
- PWM, duty cycle, frequency and resolution
- `for` loops that count up and down
- Why `delay()` is needed to see a gradual change (and that it blocks the program)

## Source
Own code.