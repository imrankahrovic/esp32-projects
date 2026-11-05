// People counter: an IR sensor counts every new obstacle, the OLED shows the count,
// and a button resets the counter to zero

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// set pin numbers
const int irPin = 35;      // the number of the IR module OUT pin
const int buttonPin = 4;   // the number of the reset button pin (external 10k pull-down, pressed = HIGH)

// OLED settings
const int screenWidth = 128;   // OLED display width in pixels
const int screenHeight = 64;   // OLED display height in pixels
const int oledAddress = 0x3C;  // OLED I2C address (try 0x3D if nothing shows up)

// how many obstacles have been counted so far
int counter = 0;

// variable for storing the pushbutton status
int buttonState = 0;

// variable for storing the button status from the previous loop pass
int lastButtonState = LOW;

// remembers the previous obstacle state, so only a NEW obstacle is counted
bool lastObstacle = false;

// create the OLED display object
Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // the IR module drives the pin, so a plain INPUT is enough
  pinMode(irPin, INPUT);

  // initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT);

  // start the OLED; if it is not found, print a message and stop
  if (!display.begin(SSD1306_SWITCHCAPVCC, oledAddress)) {
    Serial.println("OLED not found, check wiring and address");
    for (;;);
  }

  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // clear the previous picture before drawing a new one
  display.clearDisplay();
  display.setTextSize(2);

  // read the IR sensor: this module gives LOW when an obstacle is detected
  bool obstacle = (digitalRead(irPin) == LOW);

  // read the state of the reset button
  buttonState = digitalRead(buttonPin);

  // has the obstacle state changed since the last pass?
  if (obstacle != lastObstacle) {
    // print a message when the state changes
    Serial.println(obstacle ? "Obstacle detected" : "No obstacle");

    // count only when a new obstacle appears, not when it leaves
    if (obstacle) {
      counter++;
    }

    // remember the state for the next pass
    lastObstacle = obstacle;
  }

  // detect a NEW press of the button: HIGH now, but LOW on the previous pass
  if (buttonState == HIGH && lastButtonState == LOW) {
    // reset the counter
    counter = 0;
    // wait for the button contacts to stop bouncing (debounce)
    delay(50);
  }

  // remember the current button state for the next pass
  lastButtonState = buttonState;

  // draw the counter on the OLED
  display.setCursor(0, 0);
  display.print("Count: ");
  display.println(counter);

  // nothing is shown until display() is called
  display.display();

  // short pause between passes
  delay(50);
}