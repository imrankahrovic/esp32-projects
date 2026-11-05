# IR People Counter

An IR obstacle sensor counts every new obstacle detected in front of it. The current count is displayed on an OLED screen, and a pushbutton resets the counter to zero.

## Components
- ESP32
- IR obstacle avoidance sensor module (VCC, GND, OUT)
- 0.96" OLED display (SSD1306, I2C, 128x64)
- Pushbutton
- 10 kΩ resistor (pull-down)
- Breadboard and jumper wires

## Wiring

| Part | Pin | Connection |
|---|---|---|
| IR module | VCC | 3V3 |
| IR module | GND | GND |
| IR module | OUT | D35 |
| OLED display | VCC | 3V3 |
| OLED display | GND | GND |
| OLED display | SDA | D21 |
| OLED display | SCL | D22 |
| Pushbutton | One side | 3V3 |
| Pushbutton | Other side | D4 |
| Resistor | 10 kΩ | D4 → GND |

The 10 kΩ resistor acts as an external pull-down, keeping the button input LOW when the button is not pressed.

The IR module is powered from 3V3 so its output never exceeds what the ESP32 pin can take.

## How it works
- The IR sensor detects objects using infrared light. When an object is in front of the sensor, the module changes its output signal.
- My module gives **LOW when an obstacle is detected** and HIGH when there is none.
- The program remembers the previous obstacle state using `lastObstacle`, so it counts only when a new obstacle appears.
- When the obstacle leaves and the sensor returns to HIGH, the counter does not increase.
- The pushbutton resets the counter to zero when a new press is detected (HIGH now, LOW previously).
- A 50 ms delay is used for basic button debouncing to reduce unwanted multiple presses caused by contact bouncing.
- The OLED display is updated continuously and shows the current number of detected obstacles.
- The Serial Monitor prints `Obstacle detected` and `No obstacle` only when the sensor state changes.

## Notes
- The counter increases once when an obstacle appears, not continuously while it remains in front of the sensor.
- If an object stays in front of the sensor, it will only be counted once until it leaves and is detected again.
- The detection distance can be adjusted using the trimmer screw on the IR module.
- The OLED uses I2C communication. The default ESP32 SDA and SCL pins are GPIO21 and GPIO22.
- If nothing appears on the OLED, check the wiring and try changing the I2C address from `0x3C` to `0x3D`.
- The button requires an external 10 kΩ pull-down resistor because the pin is configured as `INPUT`.

## What I learned
- Counting sensor events instead of continuously reading the same detection.
- Detecting state changes using previous and current values.
- Using an IR sensor as a digital input.
- Connecting and controlling an SSD1306 OLED display using I2C.
- Using a pushbutton to reset a variable.
- Detecting a new button press using edge detection.
- Basic button debouncing.
- Combining multiple inputs and an OLED display in one ESP32 project.

## Source
Own code.