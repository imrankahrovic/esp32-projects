# WiFi Weather Station (DHT11 Web Server)

The ESP32 connects to WiFi and runs a small web server. Opening its IP address in a browser shows the temperature and humidity from a DHT11 sensor, and two buttons turn an LED on and off.

## Components
- ESP32
- DHT11 temperature and humidity sensor module (3 pins: S, +, −)
- LED
- 220 Ω resistor
- Breadboard and jumper wires

## Wiring

| Part | Pin | Connection |
|---|---|---|
| DHT11 | S (signal) | D4 |
| DHT11 | middle (+) | 3V3 |
| DHT11 | − | GND |
| LED | | D5 → 220 Ω → LED (+) ... LED (−) → GND |

## Setup
1. Install the **DHT sensor library** (Adafruit) and **Adafruit Unified Sensor** in the Arduino IDE.
2. Copy `secrets.example.h` to `secrets.h` and put in your own WiFi name and password. `secrets.h` is listed in `.gitignore`, so it is never uploaded.
3. Upload the sketch and open the Serial Monitor (115200). Wait for the IP address.
4. On a phone or computer **on the same WiFi network**, open that IP address in a browser.

The ESP32 works only on a **2.4 GHz** network, not 5 GHz.

## How it works
- `WebServer server(80)` creates a web server on port 80. `server.on("/on", handleOn)` connects an address to a function that runs when a browser asks for it.
- `handleRoot()` reads the DHT11 every time the page is opened, builds the HTML in a `String` and sends it with `server.send(200, "text/html", html)`.
- A failed reading returns `NAN`, which is checked with `isnan()`, so the page shows "Sensor error" instead of nonsense numbers.
- `<meta http-equiv='refresh' content='5'>` reloads the page every 5 seconds.
- The ON and OFF buttons open `/on` and `/off`. The ESP32 changes the LED and redirects the browser back to the main page.
- `server.handleClient()` in `loop()` checks for requests, so `loop()` must never block for long.

## Notes
- The page has no password: anyone on the same network can use it. That is fine for an LED, but not for a relay, a lock or anything important.
- The DHT11 is slow and not very accurate (about ±2 °C and ±5 %).

## What I learned
- Connecting the ESP32 to WiFi and finding its IP address
- A web server with routes and HTML generated in code
- Reading a sensor inside a web request
- Keeping passwords out of a public repository with `secrets.h` and `.gitignore`

## Source
Own code.