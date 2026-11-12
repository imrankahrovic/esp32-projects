# WiFi Temperature Alert (DHT11 + Push Notifications)

The ESP32 reads a DHT11 sensor every 5 seconds and sends a push notification to a phone when the temperature gets too high. It sends another one when the temperature is back to normal, and one if the sensor stops responding.
The notifications go through **ntfy.sh**, a free service that needs no account.

## Components
- ESP32
- DHT11 temperature and humidity sensor module (3 pins: S, +, −)
- LED
- 220 Ω resistor
- Breadboard and jumper wires
- A phone with the **ntfy** app

## Wiring

| Part | Pin | Connection |
|---|---|---|
| DHT11 | S (signal) | D4 |
| DHT11 | middle (+) | 3V3 |
| DHT11 | − | GND |
| LED | | D5 → 220 Ω → LED (+) ... LED (−) → GND |

The LED is on while an alert is active.

## Setup
1. Install the **DHT sensor library** (Adafruit) and **Adafruit Unified Sensor** in the Arduino IDE.
2. Install the **ntfy** app on your phone and subscribe to a topic. The topic works like a password, so use a long random name.
3. Copy `secrets.example.h` to `secrets.h` and put in your WiFi name, password and topic. `secrets.h` is in `.gitignore`, so it is never uploaded.
4. Upload the sketch. A **Monitor started** notification confirms that everything works.
5. To test, lower `highThreshold` and `lowThreshold` to values just above the room temperature and warm the sensor with your hand. Do not use a hair dryer close to the sensor.

The ESP32 works only on a **2.4 GHz** network, not 5 GHz.

## How it works
- Every 5 seconds (a `millis()` timer, no `delay()`) the program reads the temperature and humidity.
- **Two thresholds (hysteresis):** the alert starts above `highThreshold` and ends only below `lowThreshold`, so there is no flood of messages at the limit.
- `alertActive` remembers that an alert was already sent, so each alert is sent **once**.
- `sendNotification()` sends an HTTPS POST to `https://ntfy.sh/<topic>`. The message is the body of the request, and the title, priority and tags are HTTP headers. It returns `true` only when the server answers with code 200.
- `alertActive` changes only after a successful send, so if WiFi fails at that moment the message is retried on the next reading.
- A failed sensor reading returns `NAN`. After 3 failures in a row a **Sensor error** notification is sent, once.

## Notes
- `client.setInsecure()` skips the certificate check. The connection is encrypted, but the server is not verified. This is fine for learning, not for a real product.
- Notifications can arrive with a delay of a few seconds to a minute. The delay is between the server and the phone, because the ESP32 prints the HTTP code 200 as soon as the message is sent. Disabling battery optimization for the ntfy app helps.
- The DHT11 is not very accurate (about ±2 °C and ±5 %) and reports whole degrees.
- A public ntfy topic is readable by anyone who knows its name, so notifications should not contain anything private.

## What I learned
- HTTP POST requests, headers and response codes
- Sending push notifications from a microcontroller
- Hysteresis and state flags to send each alert only once
- Functions that return a value (`bool`)
- Keeping secrets out of a public repository (`secrets.h`, `secrets.example.h`, `.gitignore`)

## Source
Own code. Notifications through [ntfy.sh](https://ntfy.sh).