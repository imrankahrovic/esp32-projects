# ESP32 Learning Projects

Small hands-on projects I build while learning embedded programming and electronics with the ESP32.
Each project is a separate folder with the code, a wiring table and notes on what I learned.
I am working toward robotics, so the projects go step by step from the basics to sensors, motors and control.

## Projects

| # | Project | What it teaches |
|---|---|---|
| 01 | [Button controls LED](01-button-led) | digital input and output, pull-down resistor |
| 02 | [Button toggle](02-button-toggle) | state, edge detection, debounce |
| 03 | [Toggle with INPUT_PULLUP](03-input-pullup-toggle) | internal pull-up, inverted logic |
| 04 | [PWM LED fade](04-pwm-led-fade) | PWM, duty cycle |
| 05 | [Potentiometer controls LED](05-potentiometer-led) | analog input, voltage divider |
| 06 | [Traffic light with pedestrian button](06-traffic-light-pedestrian) | state machine, millis(), button request |
| 07 | [LDR automatic light](07-ldr-automatic-light) | analog sensor, thresholds, hysteresis |
| 08 | [Active buzzer beep](08-active-buzzer-beep) | millis() timing, active buzzer |
| 09 | [Passive buzzer siren](09-passive-buzzer-siren) | frequency, tone(), non-blocking sweep |

More projects are coming: traffic light with `millis()`, sensors (light, motion, temperature), buzzers, OLED display, WiFi.

## Hardware and tools
- ESP32 development board
- Breadboard, LEDs, resistors, buttons, potentiometer, sensor modules
- Arduino IDE with ESP32 core 3.x, C++

## Notes
- Every project folder has its own README with components, wiring and explanation.
- Code that is based on a tutorial says so in its README.
- All wiring is on a breadboard.