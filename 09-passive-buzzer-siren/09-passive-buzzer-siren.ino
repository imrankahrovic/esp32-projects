// Siren on a passive buzzer using millis() instead of delay()
// The frequency goes up and down between 500 and 2000 Hz without blocking the program

// set pin number
const int buzPin = 33;  // the number of the buzzer pin

// the current frequency of the tone, in Hz
int frequency = 500;

// how much the frequency changes in every step (positive = rising, negative = falling)
int step = 5;

// the time (in ms) when the frequency was last changed
unsigned long lastStep = 0;

void setup() {
  // tone() sets up the pin by itself, so nothing is needed here
}

void loop() {
  // has 10 ms passed since the last step?
  if (millis() - lastStep >= 10) {
    // play the current frequency
    tone(buzPin, frequency);
    // remember when this step happened
    lastStep = millis();
    // move the frequency one step up or down
    frequency += step;

    // reverse the direction when either end of the range is reached
    if (frequency >= 2000 || frequency <= 500) {
      step = -step;
    }
  }

  // nothing blocks here: other code could run on every pass
}