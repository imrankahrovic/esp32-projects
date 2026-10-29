// LED PWM fade: the LED slowly brightens and dims ("breathing" effect)

// set pin number
const int ledPin = 5;  // the number of the LED pin

void setup() {
  // attach the pin to PWM: pin, frequency (Hz), resolution (bits)
  // 5000 Hz is fast enough that the eye sees no flicker
  // 8 bits means brightness values from 0 (off) to 255 (full)
  ledcAttach(ledPin, 5000, 8);
}

void loop() {
  // fade in: brightness goes from 0 up to 255
  for (int i = 0; i <= 255; i++) {
    ledcWrite(ledPin, i);  // set the current brightness
    delay(20);             // short pause so the change is visible
  }

  // fade out: brightness goes from 255 down to 0
  for (int i = 255; i >= 0; i--) {
    ledcWrite(ledPin, i);
    delay(20);
  }
}