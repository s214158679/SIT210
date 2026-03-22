// SIT210 - Embedded Systems Development
// Task 3.1P - Trigger and Notification Based on Sensor Data
// Description: Reads light level from XC4446 LDR and publishes to MQTT 

#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

#define LIGHT_PIN A0

const char* WIFI_NETWORK   = "removed";
const char* WIFI_PASSWORD  = "removed";
const char* MQTT_BROKER = "192.168.1.2";
const int   MQTT_PORT   = 1883;
const char* MQTT_TOPIC  = "terrarium-light";

const int          SUNLIGHT_THRESHOLD = 600;
const unsigned long READ_INTERVAL     = 5000;
unsigned long lastReadTime = 0;

bool sunlightActive = false;

WiFiClient   wifiClient;
MqttClient   mqtt(wifiClient);

// setup()
// Connects to WiFi and MQTT broker.
void setup() {
  Serial.begin(9600);
  connectToWifi();
  connectToMQTT();
  Serial.println("System ready.");
}

// loop()
// Reads light level every 5 seconds and checks against threshold.
void loop() {
  mqtt.poll();

  if (millis() - lastReadTime >= READ_INTERVAL) {
    Serial.println("Checking Light level..");
    int light = readLightLevel();
    checkSunlightTrigger(light);
    lastReadTime = millis();
  }
}

// connectToWifi()
// Connects to WiFi and waits until established.
void connectToWifi() {
  Serial.print("Connecting to Wifi");
  while (WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println("Wifi Connection Established");
}

// connectToMQTT()
// Connects to the MQTT broker and waits until established.
void connectToMQTT() {
  Serial.println("Connecting to MQTT");
  while (!mqtt.connect(MQTT_BROKER, MQTT_PORT)) {
    Serial.print(".");
    delay(2000);
  }
  Serial.println("MQTT Connection Established.");
}

// readLightLevel()
// Reads analogue value from LDR on A0
int readLightLevel() {
  int light = analogRead(LIGHT_PIN);
  Serial.print("Light: ");
  Serial.println(light);
  return light;
}

// publishMessage()
// Publishes a message to the MQTT topic.
void publishMessage(const char* message) {
  mqtt.beginMessage(MQTT_TOPIC);
  mqtt.print(message);
  mqtt.endMessage();
  Serial.print("MQTT sent: ");
  Serial.println(message);
}

// checkSunlightTrigger()
// Checks light level against threshold and publishes an MQTT
// message on state change only, avoiding repeated triggers.
void checkSunlightTrigger(int light) {
  if (light >= SUNLIGHT_THRESHOLD && !sunlightActive) {
    sunlightActive = true;
    publishMessage("sunlight_start");
    Serial.println("Trigger: sunlight started.");
  } else if (light < SUNLIGHT_THRESHOLD && sunlightActive) {
    sunlightActive = false;
    publishMessage("sunlight_stop");
    Serial.println("Trigger: sunlight stopped.");
  }
}
