# IR Obstacle Alarm

An IR obstacle sensor turns on an LED and makes an active buzzer beep while an object is in front of it.
When the object is gone, the LED and the buzzer turn off.

## Components
- ESP32
- IR obstacle avoidance sensor module (VCC, GND, OUT)
- Active buzzer
- LED
- 220 Ω resistor
- Breadboard and jumper wires

## Wiring

| Part | Pin | Connection |
|---|---|---|
| IR module | VCC | 3V3 |
| IR module | GND | GND |
| IR module | OUT | D35 |
| Buzzer | longer leg (+) | D33 |
| Buzzer | shorter leg (−) | GND |
| LED | | D5 → 220 Ω → LED (+) ... LED (−) → GND |

The module is powered from 3V3 so its output never exceeds what the ESP32 pin can take.

## How it works
- The module has an IR LED that sends invisible infrared light and a photodiode that receives it. When an object is in front of the sensor, the light bounces back and the output changes.
- My module gives **LOW when there is an obstacle** and HIGH when there is none. I checked this in the Serial Monitor first, because some modules work the other way round.
- While there is an obstacle, the LED stays on and the buzzer beeps (200 ms on, 800 ms off) using `millis()` and the variable `buzzerOn`, so `loop()` never blocks.
- When the obstacle disappears, the LED and the buzzer are switched off explicitly. Without this, the buzzer could stay on if the obstacle left in the middle of a beep.
- `lastChange = millis() - offTime` makes the program behave as if the silence had already lasted long enough, so the first beep starts right away instead of after 800 ms.
- `lastObstacle` remembers the previous state, so the Serial Monitor prints `Obstacle detected` or `No obstacle` only when the state changes.
- The blue screw on the module sets the detection distance (roughly 2 to 30 cm).

## Notes
- White and shiny surfaces reflect IR well. Black and matte surfaces absorb it, so they are harder to detect.
- Strong sunlight or incandescent bulbs contain IR and can confuse the sensor, so it works best indoors.
- If the range is set too long, the sensor sees the wall behind and reports an obstacle all the time.

## What I learned
- How an IR reflection sensor works
- Checking the direction of a sensor output before writing the logic
- Tuning a sensor with its trimmer screw
- Reusing the non-blocking beeping pattern from the buzzer and PIR projects
- Making a beep start immediately by setting the timer in advance

## Source
Own code.