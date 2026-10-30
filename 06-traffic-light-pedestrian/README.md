# Traffic Light with Pedestrian Button

A car traffic light (red, yellow, green) and a pedestrian light (red, green) built without `delay()`.
A pedestrian button shortens the cars' green light so the pedestrians get their turn sooner.

## Components
- ESP32
- 5 LEDs: red, yellow, green (cars), red, green (pedestrians)
- 5 × 220 Ω resistors (one per LED)
- Pushbutton
- 10 kΩ resistor (pull-down for the button)
- Breadboard and jumper wires

## Wiring

| Part | Connection |
|---|---|
| Car red LED | D19 → 220 Ω → LED (+) ... LED (−) → GND |
| Car yellow LED | D5 → 220 Ω → LED (+) ... LED (−) → GND |
| Car green LED | D4 → 220 Ω → LED (+) ... LED (−) → GND |
| Pedestrian red LED | D26 → 220 Ω → LED (+) ... LED (−) → GND |
| Pedestrian green LED | D27 → 220 Ω → LED (+) ... LED (−) → GND |
| Button | 3V3 → button → D33 |
| Pull-down resistor | D33 → 10 kΩ → GND |

The button and the 10 kΩ resistor share the same node (the row where D33 is connected).
The longer leg of every LED (anode, +) goes toward the pin.

## How it works
The program is a small **state machine** with four states:

| State | Cars | Pedestrians | Duration |
|---|---|---|---|
| 0 | red | green | 5 s |
| 1 | red + yellow | red | 1 s |
| 2 | green | red | 5 s |
| 3 | yellow | red | 2 s |

- `state` remembers where in the cycle we are, and `durations[]` holds how long each state lasts.
- `millis() - lastChange` measures how long the current state has lasted, so `loop()` never blocks and the button is checked on every pass.
- `applyState()` sets all five LEDs for a given state, using `setLights()`.
- `state = (state + 1) % 4` moves to the next state and wraps back to 0 after the last one.
- When the button is pressed during the cars' green, `pedestrianRequest` is set. The green is then cut short, but only after `minGreen` (2 s), so cars never get a green that lasts a fraction of a second.
- The request is stored in a variable, so a short press is not lost, and it is cleared when the pedestrians get their green. A press while pedestrians already have green is ignored.

## What I learned
- State machines with a `state` variable and a `switch`
- Non-blocking timing with `millis()`
- Arrays for durations and functions to avoid repeating code
- Remembering a button press with a flag instead of reacting only while it is held
- Debugging hardware: finding a wiring problem by swapping pins and testing one branch at a time

## Source
Own code.