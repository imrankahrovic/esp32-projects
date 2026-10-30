# Beeping Active Buzzer (non-blocking)

An active buzzer beeps for 200 ms and stays silent for 800 ms, over and over.
The timing uses `millis()` instead of `delay()`, so the program is never blocked.

## Components
- ESP32
- Active buzzer (closed bottom, the longer leg is +)
- Breadboard and jumper wires

## Wiring

| Part | Connection |
|---|---|
| Buzzer, longer leg (+) | D33 |
| Buzzer, shorter leg (−) | GND |

An active buzzer has a built-in oscillator, so it is controlled like an LED: HIGH makes a sound, LOW is silence.
Some buzzer modules have inverted logic (LOW = sound). If it beeps when it should be silent, swap HIGH and LOW.

## How it works
- `buzzerOn` remembers whether the buzzer is currently on, and `lastChange` remembers when the state last changed.
- In every pass of `loop()` the program calculates `elapsed = millis() - lastChange`.
- If the buzzer is on and `elapsed` reached `onTime` (200 ms), it turns off. If it is off and `elapsed` reached `offTime` (800 ms), it turns on. In both cases `lastChange` is updated.
- The two conditions are different because the buzzer has two states with different durations. This is the same pattern as the traffic light, with two states instead of four.
- `loop()` never waits, so other code (reading a button or a sensor) could run on every pass.

## Things to try
- `onTime = 100`, `offTime = 100`: fast alarm beeping
- `onTime = 500`, `offTime = 500`: slow, even beeping

## What I learned
- Non-blocking timing with `millis()` and a state variable
- Why a program with `else if` needs two different conditions
- Difference between active and passive buzzers (active: built-in oscillator, passive: needs a frequency)

## Source
Own code.