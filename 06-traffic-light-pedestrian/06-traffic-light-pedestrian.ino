// Traffic light with a pedestrian button, using millis() instead of delay()

// set pin numbers
const int redPin = 19;      // car red LED
const int yellowPin = 5;    // car yellow LED
const int greenPin = 4;     // car green LED

const int redPedLed = 26;   // pedestrian red LED
const int greenPedLed = 27; // pedestrian green LED
const int buttonPin = 33;   // pedestrian button (external 10k pull-down, pressed = HIGH)

// which step of the cycle we are in (0 to 3)
int state = 0;

// the time (in ms) when the state was last changed
unsigned long lastChange = 0;

// how long each step lasts, in milliseconds
// 0: red, 1: red + yellow, 2: green, 3: yellow
const unsigned long durations[] = {5000, 1000, 5000, 2000};

// minimum time the cars keep green, even if a pedestrian pressed the button
const unsigned long minGreen = 2000;

// remembers that a pedestrian pressed the button
bool pedestrianRequest = false;

// set all five LEDs at once
void setLights(bool red, bool yellow, bool green, bool pedRed, bool pedGreen) {
  digitalWrite(redPin, red);
  digitalWrite(yellowPin, yellow);
  digitalWrite(greenPin, green);
  digitalWrite(redPedLed, pedRed);
  digitalWrite(greenPedLed, pedGreen);
}

// turn on the right LEDs for a given state
void applyState(int s) {
  switch (s) {
    case 0:
      setLights(HIGH, LOW, LOW, LOW, HIGH);    // cars red, pedestrians green
      break;
    case 1:
      setLights(HIGH, HIGH, LOW, HIGH, LOW);   // cars red + yellow, pedestrians red
      break;
    case 2:
      setLights(LOW, LOW, HIGH, HIGH, LOW);    // cars green, pedestrians red
      break;
    case 3:
      setLights(LOW, HIGH, LOW, HIGH, LOW);    // cars yellow, pedestrians red
      break;
  }
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPedLed, OUTPUT);
  pinMode(greenPedLed, OUTPUT);
  pinMode(buttonPin, INPUT);

  // start in state 0 right away, so red lights up immediately
  applyState(state);
  lastChange = millis();
}

void loop() {
  // 1. remember a button press, but ignore it while pedestrians already have green
  if (state != 0 && digitalRead(buttonPin) == HIGH) {
    pedestrianRequest = true;
  }

  // 2. calculate how long the current step has lasted
  unsigned long elapsed = millis() - lastChange;
  bool timeUp = elapsed >= durations[state];
  bool shorten = (state == 2) && pedestrianRequest && elapsed >= minGreen;

  // 3. change the step when time is up, or when green is cut short
  if (timeUp || shorten) {
    // move to the next step; after the last one (3) go back to 0
    state = (state + 1) % 4;
    // light up the LEDs for the new step
    applyState(state);
    // remember when this change happened
    lastChange = millis();

    // pedestrians get their green in state 0, so the request is done
    if (state == 0) {
      pedestrianRequest = false;
    }
  }
}