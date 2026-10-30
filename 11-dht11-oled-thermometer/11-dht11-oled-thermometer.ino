// Show temperature and humidity from a DHT11 on an SSD1306 OLED (and in the Serial Monitor)

#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int dhtPin = 4;          // the number of the DHT11 DATA pin

const int screenWidth = 128;   // OLED width in pixels
const int screenHeight = 64;   // OLED height in pixels
const int oledAddress = 0x3C;  // I2C address of the OLED (try 0x3D if nothing shows up)

DHT dht(dhtPin, DHT11);
Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, -1);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // start the OLED; if it is not found, print a message and stop
  if (!display.begin(SSD1306_SWITCHCAPVCC, oledAddress)) {
    Serial.println("OLED not found, check wiring and address");
    for (;;);
  }

  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // read the temperature (in °C) and the humidity (in %)
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // clear the previous picture before drawing a new one
  display.clearDisplay();
  display.setTextSize(2);

  // a failed reading returns NAN (not a number)
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from the DHT sensor");

    display.setCursor(0, 0);
    display.println("Sensor");
    display.println("error");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    display.setCursor(0, 0);
    display.print("T: ");
    display.print(temperature, 1);   // one decimal place
    display.println(" C");

    display.setCursor(0, 32);
    display.print("H: ");
    display.print(humidity, 1);
    display.println(" %");
  }

  // nothing is shown until display() is called
  display.display();

  // the DHT11 is slow, so read it at most once every 2 seconds
  delay(2000);
}