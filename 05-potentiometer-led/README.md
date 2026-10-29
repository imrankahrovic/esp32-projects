# Potentiometer Controls LED Brightness

Turning the potentiometer changes the brightness of the LED. The raw reading is also printed to the Serial Monitor.

## Components
- ESP32
- Potentiometer
- LED
- 220 Ω resistor
- Breadboard and jumper wires

## Wiring

| Part | Connection |
|---|---|
| Potentiometer, one outer pin | 3V3 |
| Potentiometer, other outer pin | GND |
| Potentiometer, middle pin | D35 |
| LED | D5 → 220 Ω → LED (+) ... LED (−) → GND |

Swapping the two outer pins only reverses the direction of the knob. D35 is an ADC1 pin, so it keeps working when WiFi is used later (ADC2 pins do not).

## How it works
- The potentiometer is a **voltage divider**: the middle pin outputs a voltage between 0 V and 3.3 V depending on the knob position.
- `analogRead(potPin)` converts that voltage into a number from 0 to 4095 (12 bits).
- `ledcAttach(ledPin, 5000, 12)` sets PWM to 12 bits too, so the reading can be written directly with `ledcWrite`, without `map()`.
- No `pinMode` is needed for the analog input or for the pin set up with `ledcAttach`.

## What I learned
- Analog input with `analogRead` (0 to 4095 on the ESP32)
- Voltage divider with a potentiometer
- Matching the ADC resolution and the PWM resolution
- Reading a value once and storing it in a variable instead of calling `analogRead` twice
- Using the Serial Monitor to check sensor values

## Source
Own code.