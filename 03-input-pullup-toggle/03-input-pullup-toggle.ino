// set pin numbers
const int buttonPin = 4;  // the number of the pushbutton pin
const int ledPin =  5;    // the number of the LED pin

// variable for storing the pushbutton status 
int buttonState = 0;

// variable for storing the button status from the previous loop pass
int lastButtonState = HIGH;  // HIGH because the released button reads HIGH with INPUT_PULLUP

// variable for remembering whether the LED is currently on or off
bool ledState = false;

void setup() {
  Serial.begin(115200);  
  // initialize the pushbutton pin as an input with the internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
  // initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the state of the pushbutton value
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);

  // detect a NEW press: the button is LOW now (pressed), but was HIGH on the previous pass
  if (buttonState == LOW && lastButtonState == HIGH) {
    // flip the LED state (true becomes false, false becomes true)
    ledState = !ledState;
    // write the new state to the LED pin
    digitalWrite(ledPin, ledState);
    // wait for the button contacts to stop bouncing (debounce)
    delay(50);
  }

  // remember the current button state for the next pass
  lastButtonState = buttonState;
}