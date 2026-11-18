// Parking sensor: an HC-SR04 measures the distance. A bar of four LEDs lights up one by one
// as an obstacle gets closer, and the buzzer beeps faster the closer the obstacle is.

// set pin numbers
const int trigPin = 18;  // the number of the Trig pin
const int echoPin = 27;  // the number of the Echo pin (through the voltage divider)
const int buzPin = 33;   // the number of the active buzzer pin

// the LED bar: ledPins[0] is the farthest zone, ledPins[3] is the closest zone
const int ledCount = 4;
const int ledPins[ledCount] = {5, 19, 23, 25};

// a LED turns on when the distance is at or below its threshold (in cm)
const float ledThresholds[ledCount] = {90, 65, 40, 20};

// a LED turns off only when the distance is this much above its threshold, so it does not flicker
const float hysteresisCm = 3.0;

// distances in cm
const float maxDistanceCm = 100.0;  // farther than this: silence
const float stopDistanceCm = 10.0;  // closer than this: continuous tone

// the longest time we wait for an echo: distance * 2 / speed of sound
const unsigned long timeoutUs = (unsigned long)(maxDistanceCm * 2 / 0.0343);

// beeping: the beep is always the same length, the pause gets shorter when the obstacle is closer
const unsigned long beepTime = 60;
const long longPause = 800;   // pause at maxDistanceCm
const long shortPause = 60;   // pause just above stopDistanceCm

// how often to measure, in milliseconds
const unsigned long measureInterval = 60;
unsigned long lastMeasure = 0;

// the last three readings, for the median filter
float readings[3];
int readingIndex = 0;

// the filtered distance
float distance = maxDistanceCm + 1;

// remembers which LEDs are on
bool ledOn[ledCount] = {false, false, false, false};

// non-blocking beeping
bool buzzerOn = false;
unsigned long lastBuzzChange = 0;

// one measurement: returns the distance in cm, or a value above maxDistanceCm if nothing came back
float measureOnce() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, timeoutUs);
  if (duration == 0) {
    return maxDistanceCm + 1;  // no echo: treat as out of range
  }
  return duration * 0.0343 / 2;
}

// returns the middle one of three numbers
float median3(float a, float b, float c) {
  if (a > b) { float t = a; a = b; b = t; }
  if (b > c) { float t = b; b = c; c = t; }
  if (a > b) { float t = a; a = b; b = t; }
  return b;
}

// turns the LEDs of the bar on and off depending on the distance
void updateLeds() {
  for (int i = 0; i < ledCount; i++) {
    if (distance <= ledThresholds[i]) {
      ledOn[i] = true;                                  // the obstacle is close enough: on
    } else if (distance > ledThresholds[i] + hysteresisCm) {
      ledOn[i] = false;                                 // clearly farther away: off
    }
    // in between, the LED keeps its previous state
    digitalWrite(ledPins[i], ledOn[i] ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzPin, OUTPUT);

  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // start with "nothing in range" in all three readings
  for (int i = 0; i < 3; i++) {
    readings[i] = maxDistanceCm + 1;
  }
}

void loop() {
  // 1. MEASURE: one reading every measureInterval, the median of the last three is used
  if (millis() - lastMeasure >= measureInterval) {
    lastMeasure = millis();
    readings[readingIndex] = measureOnce();
    readingIndex = (readingIndex + 1) % 3;
    distance = median3(readings[0], readings[1], readings[2]);
  }

  // 2. LED BAR
  updateLeds();

  // 3. BUZZER: depends on the distance
  if (distance > maxDistanceCm) {
    // nothing near: silence
    buzzerOn = false;
    digitalWrite(buzPin, LOW);
  } else if (distance <= stopDistanceCm) {
    // very close: continuous tone
    buzzerOn = true;
    digitalWrite(buzPin, HIGH);
  } else {
    // in between: the closer the obstacle, the shorter the pause between beeps
    unsigned long pause = map((long)distance, (long)stopDistanceCm, (long)maxDistanceCm, shortPause, longPause);
    unsigned long elapsed = millis() - lastBuzzChange;

    if (buzzerOn && elapsed >= beepTime) {
      buzzerOn = false;
      digitalWrite(buzPin, LOW);
      lastBuzzChange = millis();
    } else if (!buzzerOn && elapsed >= pause) {
      buzzerOn = true;
      digitalWrite(buzPin, HIGH);
      lastBuzzChange = millis();
    }
  }
}