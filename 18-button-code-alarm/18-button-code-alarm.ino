// Alarm with a button code and push notifications (ntfy.sh)
// DISARMED -> (code) -> ARMING 10 s -> ARMED -> (motion) -> ENTRY 5 s -> ALARM
// The same code disarms the system in the ARMED, ENTRY and ALARM states.
// a push notification is sent when the alarm starts and when it is disarmed after an alarm.

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// WiFi name, password and ntfy topic are kept in secrets.h, which is not uploaded to GitHub
#include "secrets.h"

// set pin numbers
const int buttonPins[] = {25, 26, 27};  // the numbers of the three button pins (external pull-down, pressed = HIGH)
const int buttonCount = 3;
const int pirPin = 33;     // the number of the PIR OUT pin
const int buzPin = 18;     // the number of the active buzzer pin
const int redPin = 5;      // the number of the red LED pin
const int greenPin = 19;   // the number of the green LED pin

// the secret code: the numbers are the buttons (1, 2 or 3) in the order they must be pressed
const int secretCode[] = {1, 3, 2, 1};
const int codeLength = 4;

// the times, in milliseconds
const unsigned long warmUpTime = 30000;    // the PIR is ignored for this long after power-up
const unsigned long armingTime = 10000;    // time to leave the room after the code is entered
const unsigned long entryTime = 5000;      // time to enter the code after motion is detected
const unsigned long codeTimeout = 5000;    // a half-typed code is forgotten after this long
const unsigned long debounceTime = 50;     // button bounce filter
const unsigned long wifiTimeout = 10000;   // how long setup() waits for WiFi
const unsigned long retryInterval = 10000; // pause between attempts to send a notification

// the five states of the alarm
enum State { DISARMED, ARMING, ARMED, ENTRY, ALARM };
State state = DISARMED;
const char* stateNames[] = {"DISARMED", "ARMING", "ARMED", "ENTRY", "ALARM"};

// the time (in ms) when the current state started
unsigned long stateStart = 0;

// button reading: previous state and time of the last change for each button
bool lastState[buttonCount] = {false, false, false};
unsigned long lastChangeTime[buttonCount] = {0, 0, 0};

// the code that is being typed
int entered[codeLength];
int entryIndex = 0;
unsigned long lastKeyTime = 0;

// non-blocking beeping
bool buzzerOn = false;
unsigned long lastBuzzChange = 0;

// notifications that still have to be sent
bool alarmPending = false;
bool disarmedPending = false;
unsigned long lastSendAttempt = 0;

// turns the buzzer off and forgets its state
void buzzerOff() {
  digitalWrite(buzPin, LOW);
  buzzerOn = false;
}

// beeps without delay(): onTime of sound, then offTime of silence
void beepPattern(unsigned long onTime, unsigned long offTime) {
  unsigned long elapsed = millis() - lastBuzzChange;

  if (buzzerOn && elapsed >= onTime) {
    digitalWrite(buzPin, LOW);
    buzzerOn = false;
    lastBuzzChange = millis();
  } else if (!buzzerOn && elapsed >= offTime) {
    digitalWrite(buzPin, HIGH);
    buzzerOn = true;
    lastBuzzChange = millis();
  }
}

// sends a push notification through ntfy.sh, returns true if it was sent
bool sendNotification(const char* title, const String& message, const char* priority, const char* tags) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, notification not sent");
    return false;
  }

  WiFiClientSecure client;
  client.setInsecure();  // skips the certificate check: fine for learning, not for anything important

  HTTPClient http;
  String url = String("https://ntfy.sh/") + ntfyTopic;
  http.begin(client, url);

  // keep the headers plain ASCII (no special characters)
  http.addHeader("Title", title);
  http.addHeader("Priority", priority);
  http.addHeader("Tags", tags);

  // the message is the body of the POST request
  int code = http.POST(message);
  http.end();

  Serial.print("Notification sent, HTTP code: ");
  Serial.println(code);

  return code == 200;
}

// sends the notifications that are waiting; a failed one is retried every retryInterval
void handleNotifications() {
  if (!alarmPending && !disarmedPending) {
    return;
  }
  if (millis() - lastSendAttempt < retryInterval) {
    return;
  }
  lastSendAttempt = millis();

  // the alarm message always goes first
  if (alarmPending) {
    if (sendNotification("ALARM", "Motion detected and the code was not entered in time", "urgent", "rotating_light")) {
      alarmPending = false;
    }
  } else if (disarmedPending) {
    if (sendNotification("Alarm disarmed", "The correct code was entered", "default", "white_check_mark")) {
      disarmedPending = false;
    }
  }
}

// moves to a new state and resets everything that belongs to the old one
void setState(State newState) {
  // remember which notification has to be sent (the sending itself happens in handleNotifications)
  if (newState == ALARM) {
    alarmPending = true;
    lastSendAttempt = millis() - retryInterval;   // so the first attempt starts right away
  } else if (state == ALARM && newState == DISARMED) {
    disarmedPending = true;
    lastSendAttempt = millis() - retryInterval;
  }

  state = newState;
  stateStart = millis();
  entryIndex = 0;                      // forget a half-typed code
  buzzerOff();
  lastBuzzChange = millis() - 1000;    // so the first beep starts right away
  Serial.print("State: ");
  Serial.println(stateNames[state]);
}

// returns 1, 2 or 3 at the moment a button is newly pressed, otherwise 0
int readButton() {
  int result = 0;

  for (int i = 0; i < buttonCount; i++) {
    bool pressed = (digitalRead(buttonPins[i]) == HIGH);

    if (pressed != lastState[i]) {
      // a new press counts only if the button was stable for debounceTime before
      if (pressed && millis() - lastChangeTime[i] >= debounceTime) {
        result = i + 1;
      }
      lastChangeTime[i] = millis();
      lastState[i] = pressed;
    }
  }

  return result;
}

// collects button presses and returns true only when the correct code was just completed
bool checkCode() {
  // forget a half-typed code if no button was pressed for a while
  if (entryIndex > 0 && millis() - lastKeyTime >= codeTimeout) {
    entryIndex = 0;
    Serial.println("Code entry timed out");
  }

  int key = readButton();
  if (key == 0) {
    return false;
  }

  entered[entryIndex] = key;
  entryIndex++;
  lastKeyTime = millis();
  Serial.print("Button ");
  Serial.println(key);

  // the code is complete only when enough buttons were pressed
  if (entryIndex < codeLength) {
    return false;
  }

  // compare the typed code with the secret code
  entryIndex = 0;
  for (int i = 0; i < codeLength; i++) {
    if (entered[i] != secretCode[i]) {
      Serial.println("Wrong code");
      return false;
    }
  }

  Serial.println("Correct code");
  return true;
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttonPins[i], INPUT);  // external pull-down resistors
  }
  pinMode(pirPin, INPUT);
  pinMode(buzPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  // connect to WiFi, but do not wait forever: the alarm must also work without a network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  unsigned long wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < wifiTimeout) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("No WiFi, the alarm works without notifications");
  }

  setState(DISARMED);
}

void loop() {
  // check the buttons on every pass; true only at the moment the correct code is completed
  bool codeOk = checkCode();

  // how long the current state has lasted
  unsigned long elapsed = millis() - stateStart;

  switch (state) {
    case DISARMED:
      // green on: the system is off
      digitalWrite(greenPin, HIGH);
      digitalWrite(redPin, LOW);
      if (codeOk) {
        setState(ARMING);
      }
      break;

    case ARMING:
      // red blinks while the exit countdown runs
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, (millis() / 250) % 2 == 0 ? HIGH : LOW);
      if (codeOk) {
        setState(DISARMED);               // the code cancels arming
      } else if (elapsed >= armingTime) {
        setState(ARMED);
      }
      break;

    case ARMED:
      // red on: the system is watching
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, HIGH);
      if (codeOk) {
        setState(DISARMED);
      } else if (millis() >= warmUpTime && digitalRead(pirPin) == HIGH) {
        setState(ENTRY);                  // motion: the entry countdown starts
      }
      break;

    case ENTRY:
      // slow beeping: type the code before the time runs out
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, HIGH);
      beepPattern(150, 850);
      if (codeOk) {
        setState(DISARMED);
      } else if (elapsed >= entryTime) {
        setState(ALARM);
      }
      break;

    case ALARM:
      // fast beeping until the correct code is typed
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, HIGH);
      beepPattern(200, 200);
      if (codeOk) {
        setState(DISARMED);
      }
      break;
  }

  // send a waiting notification, if there is one
  handleNotifications();
}