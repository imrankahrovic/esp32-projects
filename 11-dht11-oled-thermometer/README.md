# DHT11 Thermometer with OLED Display

Temperature and humidity from a DHT11 sensor are shown on a 0.96" SSD1306 OLED display and also printed to the Serial Monitor.

## Components
- ESP32
- DHT11 temperature and humidity sensor module (3 pins: S, +, −)
- SSD1306 OLED display, 128x64, I2C (4 pins)
- Breadboard and jumper wires

## Wiring

| Part | Pin | Connection |
|---|---|---|
| DHT11 | S (signal) | D4 |
| DHT11 | middle (+) | 3V3 |
| DHT11 | − | GND |
| OLED | VCC | 3V3 |
| OLED | GND | GND |
| OLED | SDA | D21 |
| OLED | SCL | D22 |

D21 and D22 are the default I2C pins of the ESP32. Check the markings on the DHT11 module before connecting power: swapping + and − can damage it.

## Libraries
Install through **Sketch → Include Library → Manage Libraries**:
- **DHT sensor library** (Adafruit), and **Adafruit Unified Sensor** if it asks for it
- **Adafruit SSD1306**
- **Adafruit GFX Library**

## How it works
- `dht.readTemperature()` and `dht.readHumidity()` return decimal numbers (`float`). A failed reading returns `NAN`, which is checked with `isnan()`.
- The DHT11 is slow, so it is read once every 2 seconds.
- The OLED talks over **I2C**: two wires (SDA for data, SCL for the clock) and an address. This display uses address `0x3C`. Some use `0x3D`.
- Each loop pass clears the screen, draws the new text, and calls `display.display()`. Nothing appears on the screen until `display()` is called.
- `display.print(value, 1)` prints one decimal place so the text fits in a line.
- If the OLED is not found at startup, the program prints a message and stops, which points to a wiring or address problem.

## Notes
- The DHT11 is not very accurate (about ±2 °C for temperature and ±5 % for humidity), which is fine for learning.
- The program uses `delay(2000)`. A `millis()` version would let the ESP32 do other things between readings.

## What I learned
- Installing and using Arduino libraries
- Reading a digital sensor and handling failed readings with `isnan()`
- I2C: shared bus, two wires, device address
- Drawing text on an OLED with the Adafruit GFX library
- Debugging an upload error (lower upload speed, better USB cable) and wiring a module without clear markings

## Source
Own code, built with the Adafruit DHT, GFX and SSD1306 libraries.