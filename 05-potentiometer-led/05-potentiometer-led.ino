// LED brightness controlled by a potentiometer

const int ledPin = 5;   // the number of the LED pin
const int potPin = 35;  // the number of the potentiometer pin (ADC1)

void setup() {
  Serial.begin(115200);
  // attach the LED pin to PWM: 5000 Hz, 12 bits (values from 0 to 4095)
  ledcAttach(ledPin, 5000, 12);
}

void loop() {
  // read the potentiometer once and store the value (0 to 4095)
  int potValue = analogRead(potPin);

  // print the value so we can check it in the Serial Monitor
  Serial.println(potValue);

  // use the same value as LED brightness (12-bit PWM matches the 12-bit ADC)
  ledcWrite(ledPin, potValue);

  delay(10);
}