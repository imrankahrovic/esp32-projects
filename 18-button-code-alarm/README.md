# Button Code Alarm with Push Notifications

A motion alarm armed and disarmed with a **code typed on three buttons**, built as a state machine with five states.
When motion is detected, there are 5 seconds to type the code. If it is not typed in time, the alarm sounds and a push notification is sent to a phone through **ntfy.sh**.

## Components
- ESP32
- 3 pushbuttons
- 3 × 10 kΩ resistors (pull-down, one per button)
- PIR motion sensor module HC-SR501
- Active buzzer
- Red LED and green LED
- 2 × 220 Ω resistors (one per LED)
- Breadboard and jumper wires
- A phone with the **ntfy** app

## Wiring

| Part | Pin | Connection |
|---|---|---|
| Button 1, 2, 3 | D25, D26, D27 | 3V3 → button → node; the pin and a 10 kΩ resistor to GND are on the same node |
| PIR | VCC | VIN (5V) |
| PIR | OUT (middle pin) | D33 |
| PIR | GND | GND |
| Buzzer | longer leg (+) | D18 |
| Buzzer | shorter leg (−) | GND |
| Red LED | | D5 → LED (+) ... LED (−) → 220 Ω → GND |
| Green LED | | D19 → LED (+) ... LED (−) → 220 Ω → GND |

The PIR is powered with 5V, but its output is 3.3V, so OUT can go straight to the ESP32 pin.
PIR module settings: jumper on **H**, time delay screw turned all the way counter-clockwise (shortest), sensitivity around the middle.

## States

| State | Green | Red | Buzzer | Next state |
|---|---|---|---|---|
| **DISARMED** | on | off | silent | correct code → ARMING |
| **ARMING** (10 s) | off | blinking | silent | 10 s → ARMED, code → DISARMED |
| **ARMED** | off | on | silent | motion → ENTRY, code → DISARMED |
| **ENTRY** (5 s) | off | on | slow beeping | code → DISARMED, 5 s → ALARM |
| **ALARM** | off | on | fast beeping | code → DISARMED |

The same code arms and disarms the system. The default code is in `secretCode[]` (buttons 1, 3, 2, 1).

## Setup
1. Copy `secrets.example.h` to `secrets.h` and put in your WiFi name, password and an ntfy topic. `secrets.h` is in `.gitignore`, so it is never uploaded.
2. Install the **ntfy** app on your phone and subscribe to the same topic. The topic works like a password, so use a long random name.
3. Upload the sketch. For quick testing, lower `warmUpTime` and `armingTime` to 5000 ms.

The ESP32 works only on a **2.4 GHz** network, not 5 GHz.

## How it works
- A `switch` on `state` decides what the LEDs and the buzzer do. `setState()` changes the state, clears a half-typed code and stops the buzzer.
- **`readButton()`** returns 1, 2 or 3 only at the moment a button is newly pressed. It keeps the previous state and the time of the last change for every button, so bounce is filtered with `millis()` and `loop()` never blocks.
- **`checkCode()`** collects the presses in `entered[]`. When 4 presses are in, it compares them with `secretCode[]`: any difference means a wrong code. It returns `true` for exactly one pass of `loop()`, at the moment the correct code is completed, so `if (codeOk)` in a state runs once.
- A half-typed code is forgotten after 5 seconds, or when the state changes.
- **Timers:** warm-up (the PIR needs about 30 seconds after power-up), exit delay (10 s), entry delay (5 s) and the beeping patterns all use `millis()`.
- **Notifications:** `setState()` only sets a flag (`alarmPending` or `disarmedPending`). `handleNotifications()` at the end of `loop()` sends the waiting message and retries every 10 seconds if sending fails, so a message is not lost when WiFi drops. The alarm message goes first.
- `setup()` waits for WiFi for at most 10 seconds. Without a network the alarm still works, only without notifications.
- `sendNotification()` sends an HTTPS POST to `https://ntfy.sh/<topic>`. The message is the body, and the title, priority and tags are headers.

## Limits
- Three buttons and four presses give only 81 combinations, and the code is stored in the program. This is a learning project and not protection for a real room.
- Sending a notification blocks the program for about a second. A button press during that time can be missed, so the code is not checked while a message is being sent.
- `client.setInsecure()` skips the certificate check. The connection is encrypted, but the server is not verified.
- The code is printed to the Serial Monitor for testing (`Button 1`...). Remove those lines for real use.

## What I learned
- State machines with five states, `enum` and `switch`
- Reading several buttons without `delay()`: edge detection, debounce and a typed code compared with an array
- Events versus states: why `codeOk` is true for one pass only
- Combining a sensor, buttons, LEDs, a buzzer and WiFi in one non-blocking program
- Retrying a failed notification instead of losing it
- Keeping secrets out of a public repository

## Source
Own code. Notifications through [ntfy.sh](https://ntfy.sh).