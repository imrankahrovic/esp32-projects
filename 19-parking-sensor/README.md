# Parking Sensor (HC-SR04, LED Bar and Buzzer)

An ultrasonic sensor measures the distance to an obstacle. A bar of four LEDs lights up one by one as the obstacle gets closer, and an active buzzer beeps faster the closer it is. Very close, the buzzer gives a continuous tone.

## Components
- ESP32
- HC-SR04 ultrasonic sensor
- 4 LEDs (for example green, yellow, orange, red)
- 4 × 220 Ω resistors (one per LED)
- 3 × 10 kΩ resistors (voltage divider for the Echo pin)
- Active buzzer
- Breadboard and jumper wires

## Wiring

| Part | Pin | Connection |
|---|---|---|
| HC-SR04 | VCC | VIN (5V) |
| HC-SR04 | GND | GND |
| HC-SR04 | Trig | D18 |
| HC-SR04 | Echo | voltage divider, then D27 (see below) |
| LED 1 (green) | | D5 → LED (+) → LED (−) → 220 Ω → GND |
| LED 2 (yellow) | | D19 → LED (+) → LED (−) → 220 Ω → GND |
| LED 3 (orange) | | D23 → LED (+) → LED (−) → 220 Ω → GND |
| LED 4 (red) | | D25 → LED (+) → LED (−) → 220 Ω → GND |
| Buzzer | longer leg (+) | D33 |
| Buzzer | shorter leg (−) | GND |

### Voltage divider for Echo
The HC-SR04 is powered with 5V and its Echo output can be 5V, but ESP32 pins are rated for 3.3V only. A divider brings the signal down to about 3.3V:

```text
Echo ──[ 10 kΩ ]──┬──── D27
                  │
               [ 10 kΩ ]
                  │
               [ 10 kΩ ]
                  │
                 GND
```

One resistor goes between Echo and D27, and two resistors in series go between D27 and GND. D27 and the three resistor legs meet in the same breadboard row. This gives 5V × 20 kΩ / 30 kΩ ≈ 3.3V.

Trig can go straight to the pin, because the ESP32 output (3.3V) is read as HIGH by the sensor.

I measured the resistance with a multimeter before connecting the power: about 30 kΩ from the Echo row to GND and about 20 kΩ from the D27 row to GND.

## How it works
- A 10 µs pulse on Trig makes the sensor send ultrasound. `pulseIn()` measures how long Echo stays HIGH, which is the travel time of the sound there and back, so `distance = time × 0.0343 / 2` (in cm).
- `pulseIn()` has a timeout, so an echo that comes back later than `maxDistanceCm` (100 cm) is ignored. Without it, a door behind me at 2.6 m was read as an obstacle when I waved my hand.
- **Median filter:** the last three readings are stored in `readings[]`, and the middle one is used. One wrong reading (a reflection from a wall or a door) is discarded. The measurement runs every 60 ms with `millis()`, so `loop()` never blocks.
- **LED bar:** arrays `ledPins[]` and `ledThresholds[]` hold the pin and the distance for every LED (90, 65, 40 and 20 cm). A LED turns on when the obstacle is at or below its threshold, and the earlier LEDs stay on, so the bar grows as the obstacle gets closer.
- **Hysteresis:** a LED turns off only when the obstacle is 3 cm beyond its threshold. Without it the LED would flicker when the distance is right at the limit, because readings vary by about ±1 cm.
- **Buzzer:** the beep is always 60 ms long, and `map()` shortens the pause between beeps from 800 ms at 100 cm to 60 ms at 10 cm. Under 10 cm it is a continuous tone. It is the same non-blocking beeping pattern as in the earlier buzzer projects.

## Notes
- Hard, flat surfaces (a book, a box) are detected best. Soft surfaces and objects at an angle reflect little sound, so a reading can be missed.
- The sensor's beam is a cone of about 15°, so objects to the side can be detected too.
- The median filter delays the reaction by about 120 ms, because two of the three readings must show the obstacle.
- Thresholds, range and pauses are constants at the top of the sketch and are easy to change.

## What I learned
- How an ultrasonic sensor works and how to calculate distance from time
- Level shifting with a resistor voltage divider (5V sensor, 3.3V pins)
- Timeouts in `pulseIn()` and filtering wrong readings with a median
- Arrays and `for` loops to control several LEDs
- Hysteresis to stop flicker at a threshold
- `map()` to turn a distance into a beeping speed

## Source
Own code.