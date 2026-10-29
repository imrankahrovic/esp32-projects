# Button Toggle with INPUT_PULLUP

Same toggle behavior as the previous project, but the button is wired without an external resistor.
The ESP32's internal pull-up resistor is used instead.

## Components
- ESP32
- Pushbutton
- LED
- 220 Ω resistor (for the LED)
- Breadboard and jumper wires

## Wiring

| Part | Connection |
|---|---|
| Button | D4 → button → GND |
| LED | D5 → 220 Ω → LED (+) ... LED (−) → GND |

3V3 and the external 10 kΩ resistor are not used. The LED's longer leg (anode, +) goes toward D5.

## How it works
- `pinMode(buttonPin, INPUT_PULLUP)` turns on a resistor inside the ESP32 (about 45 kΩ) that pulls D4 up to 3.3V.
- Released button: D4 is pulled to 3.3V and reads **HIGH**.
- Pressed button: D4 is connected directly to GND and reads **LOW**.
- The logic is inverted compared to the pull-down version: a new press is a change from HIGH to LOW, so `lastButtonState` starts as HIGH.
- Debounce is the same simple `delay(50)`.

## Pull-down vs pull-up

| | Pull-down (previous project) | INPUT_PULLUP (this project) |
|---|---|---|
| External resistor | 10 kΩ | none |
| Button connects D4 to | 3V3 | GND |
| Released | LOW | HIGH |
| Pressed | HIGH | LOW |

## What I learned
- Internal pull-up resistors and why a floating pin needs one
- Inverted button logic (pressed = LOW)
- How voltage divides between the pull-up resistor and the button

## Source
Extended from the previous project (02-button-toggle).