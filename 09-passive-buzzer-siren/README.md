# Passive Buzzer Siren (non-blocking)

A passive buzzer plays a siren: the pitch rises from 500 Hz to 2000 Hz and falls back, over and over.
The timing uses `millis()` instead of `delay()` and `for` loops, so the program is never blocked.

## Components
- ESP32
- Passive buzzer (open bottom, both legs the same length)
- Breadboard and jumper wires

## Wiring

| Part | Connection |
|---|---|
| Buzzer, one leg | D33 |
| Buzzer, other leg | GND |

A passive buzzer has no polarity, so it can be connected either way round.

## How it works
- A passive buzzer has no built-in oscillator. The pitch comes from the **frequency** of the signal the ESP32 sends: a lower frequency gives a deeper tone, a higher one gives a higher tone.
- `tone(buzPin, frequency)` plays a tone of the given frequency until it is changed or stopped. It sets up the pin by itself, so `pinMode` is not needed.
- `frequency` and `step` are variables that remember the current pitch and the direction. In every step the frequency changes by `step` (5 Hz).
- A step happens only when 10 ms have passed since `lastStep`, so `loop()` runs freely in between.
- When the frequency reaches 2000 Hz or 500 Hz, `step = -step` reverses the direction. This check is inside the 10 ms block, so it runs once per step and not thousands of times.
- One sweep in one direction takes about 3 seconds (300 steps × 10 ms).

## Notes
- A faint hiss is audible during the sweep. Each call of `tone()` starts a new wave, and a small passive buzzer is not a real speaker, so this is normal.
- The sound is loudest around 2 to 4 kHz, which is the resonance of the buzzer.
- An active buzzer would not work here: it makes a single fixed tone no matter what frequency is sent.

## Things to try
- Interval of 20 ms with `step = 10`: the same speed with fewer `tone()` calls, so less hiss
- Range of 1000 to 3000 Hz: closer to the buzzer's resonance, so it is louder
- `ledcWriteTone()` instead of `tone()`: needs `ledcAttach(pin, 2000, 8)` in `setup()`

## What I learned
- Frequency, pitch and the difference between active and passive buzzers
- Replacing `for` loops and `delay()` with `millis()` and state variables
- Why a check that changes direction must be inside the timed block
- `tone()` and `noTone()` on the ESP32

## Source
Own code.