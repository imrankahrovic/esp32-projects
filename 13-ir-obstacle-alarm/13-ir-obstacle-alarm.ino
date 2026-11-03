// IR obstacle alarm: while an obstacle is in front of the sensor,
// the LED stays on and the buzzer beeps

// set pin numbers
const int irPin = 35;   // the number of the IR module OUT pin
const int ledPin = 5;   // the number of the LED pin
const int buzPin = 33;  // the number of the active buzzer pin

// how long the buzzer is on and off, in milliseconds
const unsigned long onTime = 200;   // beep
const unsigned long offTime = 800;  // silence

// remembers whether the buzzer is currently on
bool buzzerOn = false;

// remembers the previous obstacle state, to print only when it changes
bool lastObstacle = false;

// the time (in ms) when the buzzer state was last changed
unsigned long lastChange = 0;

void setup() {
  Serial.begin(115200);

  // the module drives the pin, so a plain INPUT is enough
  pinMode(irPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzPin, OUTPUT);

  // pretend the silence has already lasted long enough,
  // so the first beep starts right away
  lastChange = millis() - offTime;
}

void loop() {
  // this module gives LOW when there is an obstacle
  bool obstacle = (digitalRead(irPin) == LOW);

  // print only when the state changes
  if (obstacle != lastObstacle) {
    Serial.println(obstacle ? "Obstacle detected" : "No obstacle");
    lastObstacle = obstacle;
  }

  if (obstacle) {
    // how long the current buzzer state has lasted
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

    digitalWrite(ledPin, HIGH);
  } else {
    // no obstacle: make sure both the buzzer and the LED are really off
    digitalWrite(buzPin, LOW);
    buzzerOn = false;
    digitalWrite(ledPin, LOW);

    // so the next beep starts right away when an obstacle appears
    lastChange = millis() - offTime;
  }
}