// Web server: a web page shows the temperature and humidity from a DHT11
// and has two buttons that turn an LED on and off

#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// WiFi name and password are kept in secrets.h, which is not uploaded to GitHub
#include "secrets.h"

const int ledPin = 5;  // the number of the LED pin
const int dhtPin = 4;  // the number of the DHT11 DATA pin

// remembers whether the LED is currently on
bool ledOn = false;

DHT dht(dhtPin, DHT11);

// the web server listens on port 80 (the default web port)
WebServer server(80);

// builds and sends the main page
void handleRoot() {
  // read the sensor every time the page is opened
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  // reload the page by itself every 5 seconds
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<title>ESP32 Weather</title></head>";
  html += "<body style='font-family:Arial; text-align:center'>";
  html += "<h1>ESP32 Weather</h1>";

  // a failed reading returns NAN (not a number)
  if (isnan(temperature) || isnan(humidity)) {
    html += "<h2>Sensor error</h2>";
  } else {
    html += "<h2>Temperature: " + String(temperature, 1) + " &deg;C</h2>";
    html += "<h2>Humidity: " + String(humidity, 1) + " %</h2>";
  }

  html += "<hr>";
  html += ledOn ? "<h2>LED is ON</h2>" : "<h2>LED is OFF</h2>";
  html += "<p><a href='/on'><button style='font-size:24px; padding:16px'>ON</button></a></p>";
  html += "<p><a href='/off'><button style='font-size:24px; padding:16px'>OFF</button></a></p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// turns the LED on, then sends the browser back to the main page
void handleOn() {
  ledOn = true;
  digitalWrite(ledPin, HIGH);
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "");
}

// turns the LED off, then sends the browser back to the main page
void handleOff() {
  ledOn = false;
  digitalWrite(ledPin, LOW);
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  dht.begin();

  // connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // connect each address to its function
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // check whether a browser has sent a request
  server.handleClient();
}