// Automatic light with an LDR module (AO output): the LED turns on in the dark

// set pin numbers
const int ldrPin = 35;  // the number of the LDR module AO pin
const int ledPin = 5;   // the number of the LED pin

// thresholds (0 to 4095): adjust them to your own readings
// this module gives HIGHER numbers in the dark
const int darkThreshold = 3500;   // above this value: it is dark, LED on
const int lightThreshold = 3000;  // below this value: it is light, LED off

// remembers whether the LED is currently on
bool ledOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the light level (0 to 4095)
  int light = analogRead(ldrPin);
  Serial.println(light);

  if (light > darkThreshold) {
    ledOn = true;     // dark enough: turn the LED on
  } else if (light < lightThreshold) {
    ledOn = false;    // light enough: turn the LED off
  }
  // between the two thresholds the LED keeps its previous state

  digitalWrite(ledPin, ledOn);

  delay(100);
}