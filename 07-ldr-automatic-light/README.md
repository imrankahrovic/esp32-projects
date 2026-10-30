# Automatic Light with an LDR Module

The LED turns on when it gets dark and turns off when it gets light again.
The light level is read from the analog output of an LDR (photoresistor) module.

## Components
- ESP32
- LDR (photoresistor) module with VCC, GND, DO and AO pins
- LED
- 220 Ω resistor
- Breadboard and jumper wires

## Wiring

| Part | Connection |
|---|---|
| LDR module VCC | 3V3 |
| LDR module GND | GND |
| LDR module AO | D35 |
| LDR module DO | not connected |
| LED | D5 → 220 Ω → LED (+) ... LED (−) → GND |

The module is powered from 3V3 so its output never exceeds what the ESP32 pin can take. D35 is an ADC1 pin, which keeps working when WiFi is used later.

## How it works
- `analogRead(ldrPin)` gives a number from 0 to 4095. On my module the number gets **higher in the dark** (some modules work the other way round, so I checked the values in the Serial Monitor first).
- The LED uses **two thresholds** (hysteresis):
  - above `darkThreshold` (3500) the LED turns on,
  - below `lightThreshold` (3000) the LED turns off,
  - in between it keeps its previous state, stored in `ledOn`.
- With a single threshold the LED would flicker when the light level is right at the limit. The gap between the two thresholds prevents that.
- The thresholds are specific to my room and my module. To set your own, print the readings in the Serial Monitor in normal light and with the sensor covered, then put the thresholds between them, closer to the dark value.

## AO vs DO
- **AO** (analog) gives the actual number, so the threshold is chosen in code.
- **DO** (digital) gives only 0 or 1, and the threshold is set with the blue screw on the module.
- This project uses AO because the thresholds and hysteresis can be changed in code without a screwdriver.

## What I learned
- Analog sensors and `analogRead` on the ESP32
- Finding thresholds from real measurements instead of guessing
- Hysteresis with two thresholds and a state variable
- Difference between the analog and digital output of a sensor module
- Checking the direction of a sensor first (higher number in the dark or in the light)

## Source
Own code.