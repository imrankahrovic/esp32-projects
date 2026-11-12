// Temperature monitor: a DHT11 is read every 5 seconds and a push notification
// is sent to the phone (through ntfy.sh) when the temperature gets too high

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "DHT.h"

// WiFi name, password and ntfy topic are kept in secrets.h, which is not uploaded to GitHub
#include "secrets.h"

// set pin numbers
const int dhtPin = 4;  // the number of the DHT11 DATA pin
const int ledPin = 5;  // the number of the LED pin (on while the alert is active)

// two thresholds (hysteresis), so the alert does not flip back and forth at the limit
const float highThreshold = 23.0;  // above this: send the alert
const float lowThreshold = 22.0;   // below this: the alert is over

// how often the sensor is read, in milliseconds (the DHT11 is slow)
const unsigned long readInterval = 5000;

// the sensor must fail this many times in a row before an error notification is sent
const int maxFailures = 3;

DHT dht(dhtPin, DHT11);

// remembers whether an alert is currently active, so it is sent only once
bool alertActive = false;

// remembers whether the sensor error was already reported
bool sensorErrorSent = false;

// counts failed readings in a row
int failCount = 0;

// the time (in ms) of the last sensor reading
unsigned long lastRead = 0;

// sends a push notification through ntfy.sh, returns true if it was sent
bool sendNotification(const char* title, const String& message, const char* priority, const char* tags) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, notification not sent");
    return false;
  }

  WiFiClientSecure client;
  client.setInsecure();  // skips the certificate check: fine for learning, not for anything important

  HTTPClient http;
  String url = String("https://ntfy.sh/") + ntfyTopic;
  http.begin(client, url);

  // keep the headers plain ASCII (no special characters)
  http.addHeader("Title", title);
  http.addHeader("Priority", priority);
  http.addHeader("Tags", tags);

  // the message is the body of the POST request
  int code = http.POST(message);
  http.end();

  Serial.print("Notification sent, HTTP code: ");
  Serial.println(code);

  return code == 200;
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

  // a test message, so you know the phone is set up correctly
  sendNotification("Monitor started", "The temperature monitor is running", "default", "white_check_mark");
}

void loop() {
  // read the sensor only when readInterval has passed (no delay, so loop() never blocks)
  if (millis() - lastRead < readInterval) {
    return;
  }
  lastRead = millis();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // a failed reading returns NAN (not a number)
  if (isnan(temperature) || isnan(humidity)) {
    failCount++;
    Serial.println("Failed to read from the DHT sensor");

    if (failCount >= maxFailures && !sensorErrorSent) {
      if (sendNotification("Sensor error", "The temperature sensor is not responding", "high", "warning")) {
        sensorErrorSent = true;
      }
    }
    return;
  }

  // the sensor works again
  failCount = 0;
  sensorErrorSent = false;

  Serial.print("Temperature: ");
  Serial.print(temperature, 1);
  Serial.print(" C, humidity: ");
  Serial.print(humidity, 1);
  Serial.println(" %");

  if (temperature > highThreshold && !alertActive) {
    // too warm and no alert sent yet: send it once
    String message = "Temperature is " + String(temperature, 1) + " C, humidity " + String(humidity, 1) + " %";
    if (sendNotification("Temperature too high", message, "urgent", "warning")) {
      alertActive = true;
    }
  } else if (temperature < lowThreshold && alertActive) {
    // back to normal: send one message and end the alert
    String message = "Temperature is back to " + String(temperature, 1) + " C";
    if (sendNotification("Temperature back to normal", message, "default", "white_check_mark")) {
      alertActive = false;
    }
  }

  // the LED shows that an alert is active
  digitalWrite(ledPin, alertActive ? HIGH : LOW);
}