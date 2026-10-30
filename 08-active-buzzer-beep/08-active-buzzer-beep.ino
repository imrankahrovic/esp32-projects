// Beeping buzzer using millis() instead of delay()

// set pin number
const int buzPin = 33;  // the number of the buzzer pin

// how long the buzzer is on and off, in milliseconds
const unsigned long onTime = 200;   // beep
const unsigned long offTime = 800;  // silence

// remembers whether the buzzer is currently on
bool buzzerOn = false;

// the time (in ms) when the buzzer state was last changed
unsigned long lastChange = 0;

void setup() {
  // initialize the buzzer pin as an output
  pinMode(buzPin, OUTPUT);

  // start the timer
  lastChange = millis();
}

void loop() {
  // how long the current state has lasted
  unsigned long elapsed = millis() - lastChange;

  if (buzzerOn && elapsed >= onTime) {
    // the beep lasted long enough: turn the buzzer off
    digitalWrite(buzPin, LOW);
    buzzerOn = false;
    lastChange = millis();
  } else if (!buzzerOn && elapsed >= offTime) {
    // the silence lasted long enough: turn the buzzer on
    digitalWrite(buzPin, HIGH);
    buzzerOn = true;
    lastChange = millis();
  }
}