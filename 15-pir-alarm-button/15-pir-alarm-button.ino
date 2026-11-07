// PIR alarm with an arm/disarm button, built as a state machine
// DISARMED: nothing happens. ARMED: waiting for motion. ALARM: buzzer beeps until the button is pressed.

// set pin numbers
const int pirPin = 33;     // the number of the PIR OUT pin
const int buzPin = 18;     // the number of the active buzzer pin
const int ledPin = 5;      // the number of the LED pin (on while armed or in alarm)
const int buttonPin = 4;   // the number of the button pin (external 10k pull-down, pressed = HIGH)

// the three states of the alarm
enum State { DISARMED, ARMED, ALARM };
State state = DISARMED;

// the PIR needs time to settle after power-up, so it is ignored at first
const unsigned long warmUpTime = 30000;

// time to leave the room after arming (the PIR keeps its output HIGH for a few seconds after motion)
const unsigned long armDelay = 5000;

// how long the buzzer is on and off, in milliseconds
const unsigned long onTime = 200;   // beep
const unsigned long offTime = 800;  // silence

// the button state from the previous loop pass
int lastButtonState = LOW;

// remembers whether the buzzer is currently on
bool buzzerOn = false;

// the time (in ms) when the alarm was armed
unsigned long armedSince = 0;

// the time (in ms) when the buzzer state was last changed
unsigned long lastChange = 0;

void setup() {
  Serial.begin(115200);

  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 1. BUTTON: a new press arms the alarm, or disarms it (also stops a running alarm)
  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH && lastButtonState == LOW) {
    if (state == DISARMED) {
      state = ARMED;
      armedSince = millis();
      Serial.println("Armed");
    } else {
      state = DISARMED;
      Serial.println("Disarmed");
    }
    // wait for the button contacts to stop bouncing (debounce)
    delay(50);
  }

  // remember the current button state for the next pass
  lastButtonState = buttonState;

  // 2. PIR: only checked while armed, after the warm-up and after the exit delay
  if (state == ARMED) {
    bool pirReady = (millis() >= warmUpTime) && (millis() - armedSince >= armDelay);

    if (pirReady && digitalRead(pirPin) == HIGH) {
      state = ALARM;
      // pretend the silence has already lasted long enough, so the first beep starts right away
      lastChange = millis() - offTime;
      Serial.println("ALARM!");
    }
  }

  // 3. OUTPUTS: what the LED and the buzzer do in each state
  switch (state) {
    case DISARMED:
      digitalWrite(ledPin, LOW);
      digitalWrite(buzPin, LOW);
      buzzerOn = false;
      break;

    case ARMED:
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzPin, LOW);
      buzzerOn = false;
      break;

    case ALARM: {
      digitalWrite(ledPin, HIGH);

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
      break;
    }
  }
}