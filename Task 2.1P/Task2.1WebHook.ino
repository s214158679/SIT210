// SIT210 - Embedded Systems Development
// Task 2.1P - Sending Temperature and Light Data to the Web

#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

// Pin definitions
#define DHT_PIN   2
#define DHT_TYPE  DHT22
#define LIGHT_PIN A0

// WiFi credentials
const char* WIFI_NETWORK = "removed";
const char* WIFI_PASSWORD = "removed";

// ThingSpeak settings
unsigned long CHANNEL_ID = 3301369;
const char*   API_KEY    = "removed";

// Timing
const unsigned long SEND_INTERVAL = 30000; // 30 seconds
unsigned long lastSendTime = 0;

// Objects
WiFiClient client;
DHT dht(DHT_PIN, DHT_TYPE);

// setup()
// Connects to WiFi, initialises ThingSpeak and DHT22.
void setup() {
  Serial.begin(9600);
  dht.begin();
  connectToWifi();
  ThingSpeak.begin(client);
  Serial.println("System ready.");
}

// loop()
// Reads sensors and sends to ThingSpeak every 30 seconds.
// Uses millis() so nothing blocks between readings.
void loop() {
  if (millis() - lastSendTime >= SEND_INTERVAL) {
    float temperature = readTemperature();
    int   light       = readLightLevel();
    sendToThingSpeak(temperature, light);
    lastSendTime = millis();
  }
}

// connectToWifi()
// Connects to WiFi and waits until the connection is established.
void connectToWifi() {
  Serial.print("Connecting to WiFi");
  while (WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println(" connected.");
}

// readTemperature()
// Reads temperature in celsius from the DHT22.
// Returns 0 and prints an error if the read fails.
float readTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("DHT22 read failed.");
    return 0;
  }
  Serial.print("Temperature: ");
  Serial.println(temp);
  return temp;
}

// readLightLevel()
// Reads the analogue value from the LDR on A0
int readLightLevel() {
  int light = analogRead(LIGHT_PIN);
  Serial.print("Light: ");
  Serial.println(light);
  return light;
}

// sendToThingSpeak()
// Sends temperature and light readings to ThingSpeak.
// Field 1 = temperature, Field 2 = light level.
void sendToThingSpeak(float temperature, int light) {
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, light);
  int result = ThingSpeak.writeFields(CHANNEL_ID, API_KEY);
  if (result == 200) {
    Serial.println("Data sent successfully.");
  } else {
    Serial.print("Send failed, error: ");
    Serial.println(result);
  }
}
