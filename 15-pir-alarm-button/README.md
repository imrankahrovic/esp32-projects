# PIR Alarm with Arm/Disarm Button

A PIR motion alarm built as a **state machine** with three states. A button arms and disarms it.
Once motion is detected while the alarm is armed, the buzzer beeps until the button is pressed.

## Components
- ESP32
- PIR motion sensor module HC-SR501
- Active buzzer
- LED
- 220 Ω resistor (for the LED)
- Pushbutton
- 10 kΩ resistor (pull-down for the button)
- Breadboard and jumper wires

## Wiring

| Part | Pin | Connection |
|---|---|---|
| PIR | VCC | VIN (5V) |
| PIR | OUT (middle pin) | D33 |
| PIR | GND | GND |
| Buzzer | longer leg (+) | D18 |
| Buzzer | shorter leg (−) | GND |
| LED | | D5 → 220 Ω → LED (+) ... LED (−) → GND |
| Button | | 3V3 → button → D4 |
| Pull-down resistor | | D4 → 10 kΩ → GND |

The PIR is powered with 5V, but its output is 3.3V, so OUT can go straight to the ESP32 pin.
PIR module settings: jumper on **H**, time delay screw turned all the way counter-clockwise (shortest), sensitivity around the middle.

## States

| State | LED | Buzzer | Next state |
|---|---|---|---|
| **DISARMED** | off | silent | button → ARMED |
| **ARMED** | on | silent | motion → ALARM, button → DISARMED |
| **ALARM** | on | beeping | button → DISARMED |

## How it works
- `enum State { DISARMED, ARMED, ALARM }` gives the states names, and a `switch` decides what the outputs do in each state. It is the same idea as the traffic light.
- The button is read with edge detection (a **new** press, not a held button), so one press changes the state only once. A short `delay(50)` debounces it.
- **Warm-up:** after power-up the PIR needs 30 to 60 seconds to settle, so it is ignored for `warmUpTime` (30 s).
- **Exit delay:** `millis() - armedSince >= armDelay` ignores the PIR for 5 seconds after arming. The PIR keeps its output HIGH for a few seconds after motion, so without this delay the alarm would trigger because of the person leaving the room.
- When the alarm starts, `lastChange = millis() - offTime` makes the first beep start immediately. After that the buzzer beeps (200 ms on, 800 ms off) using `millis()` and `buzzerOn`, so `loop()` never blocks.
- The alarm does not stop by itself. It sounds until the button is pressed, as real alarms do.

## Things to try
- Make the alarm stop after 10 seconds and return to ARMED (needs one more timer and a check inside `case ALARM`)
- Play two short tones when arming and disarming
- Show the state on an OLED display

## What I learned
- State machines with `enum`, `switch` and state transitions
- Combining a sensor, a button, an LED and a buzzer in one non-blocking program
- Why a PIR needs a warm-up time and an exit delay
- Using several `millis()` timers at once (warm-up, exit delay, beeping)

## Source
Own code.