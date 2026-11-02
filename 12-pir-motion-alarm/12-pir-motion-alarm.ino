// PIR motion alarm: motion turns on the LED and makes the buzzer beep

// set pin numbers
const int pirPin = 13;  // the number of the PIR OUT pin
const int ledPin = 5;   // the number of the LED pin
const int buzPin = 33;  // the number of the active buzzer pin

// the PIR needs time to settle after power-up, so it is ignored at first
const unsigned long warmUpTime = 30000;

// how long the buzzer is on and off, in milliseconds
const unsigned long onTime = 200;   // beep
const unsigned long offTime = 800;  // silence

// remembers whether the buzzer is currently on
bool buzzerOn = false;

// remembers the previous motion state, to print only when it changes
bool lastMotion = false;

// the time (in ms) when the buzzer state was last changed
unsigned long lastChange = 0;

void setup() {
  Serial.begin(115200);

  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzPin, OUTPUT);

  lastChange = millis();
}

void loop() {
  // ignore the PIR during the warm-up time
  bool motion = false;
  if (millis() >= warmUpTime) {
    motion = (digitalRead(pirPin) == HIGH);
  }

  // print only when the state changes
  if (motion != lastMotion) {
    Serial.println(motion ? "Motion detected" : "No motion");
    lastMotion = motion;
  }

  if (motion) {
    // how long the current buzzer state has lasted
    unsigned long elapsed = millis() - lastChange;

    if (buzzerOn && elapsed >= onTime) {
      digitalWrite(buzPin, LOW);
      buzzerOn = false;
      lastChange = millis();
    } else if (!buzzerOn && elapsed >= offTime) {
      digitalWrite(buzPin, HIGH);
      buzzerOn = true;
      lastChange = millis();
    }

    digitalWrite(ledPin, HIGH);
  } else {
    // no motion: make sure both the buzzer and the LED are really off
    digitalWrite(buzPin, LOW);
    buzzerOn = false;
    digitalWrite(ledPin, LOW);
  }
}