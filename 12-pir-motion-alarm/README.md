# PIR Motion Alarm

A PIR motion sensor turns on an LED and makes an active buzzer beep while it detects motion.
When the motion stops, the LED and the buzzer turn off.

## Components
- ESP32
- PIR motion sensor module HC-SR501
- Active buzzer
- LED
- 220 Ω resistor (for the LED)
- Breadboard and jumper wires

## Wiring

| Part | Pin | Connection |
|---|---|---|
| PIR | VCC | VIN (5V) |
| PIR | OUT (middle pin) | D13 |
| PIR | GND | GND |
| Buzzer | longer leg (+) | D33 |
| Buzzer | shorter leg (−) | GND |
| LED | | D5 → 220 Ω → LED (+) ... LED (−) → GND |

The PIR is powered with 5V, but its output is 3.3V, so OUT can go straight to the ESP32 pin.
The pins of my module had no markings, so I found the VCC and GND pins under the white dome before connecting power.

## PIR module settings
- **Jumper:** position **H** (repeat trigger), so the output stays HIGH while there is motion.
- **Time delay screw:** turned all the way counter-clockwise (shortest, about 3 seconds). Turned the other way, the output stays HIGH for minutes, so the alarm seems to beep forever.
- **Sensitivity screw:** around the middle. Higher means a longer range, but also false alarms.

## How it works
- The PIR output is HIGH while it sees a moving heat source and LOW otherwise.
- After power-up the PIR needs 30 to 60 seconds to settle, and its output is random during that time. The program ignores it for `warmUpTime` (30 s).
- While there is motion, the LED stays on and the buzzer beeps (200 ms on, 800 ms off) using `millis()` and the variable `buzzerOn`. It is the same pattern as the beeping buzzer project, so `loop()` never blocks.
- When the motion stops, the LED and the buzzer are switched off explicitly. Without this, the buzzer could stay on if the motion ended in the middle of a beep.
- `lastMotion` remembers the previous state so the Serial Monitor prints a message only when the state changes, not thousands of times per second.

## Notes
- The PIR reacts to heat, so keep it away from radiators, a window with direct sun and fans. Waving sideways across its field of view works better than waving straight at it.
- Dark or cold objects at room temperature are not detected the same way as a person.

## What I learned
- How a PIR sensor works and why it needs a warm-up time
- Calibrating a sensor with the trimmer screws and a raw-value test program
- Combining a sensor, an LED and a buzzer in one non-blocking program
- Making sure outputs are turned off in every branch of the logic
- Reading markings and checking a module before connecting power

## Source
Own code.