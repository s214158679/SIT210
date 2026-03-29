// SIT210 - Embedded Systems Development
// Task 4.1P - Handling Interrupts

#define LIGHT_PIN A0

const int PIR_PIN    = 2;   // PIR motion sensor output - interrupt capable
const int SWITCH_PIN = 3;   // Slider switch - interrupt capable
const int LED1_PIN   = 4;   // Porch light LED
const int LED2_PIN   = 5;   // Hallway light LED

const int DARK_THRESHOLD = 400;  // Analogue value above which it is considered dark

volatile bool motionDetected  = false;  // Set by PIR interrupt, cleared in loop()
volatile bool switchTriggered = false;  // Set by switch interrupt, cleared in loop()

bool lightsOn = false;

// onMotionDetected()
void onMotionDetected() {
  motionDetected = true;
}

// onSwitchToggled()
void onSwitchToggled() {
  switchTriggered = true;
}

// readLightLevel()
// Reads analogue value from LDR on A0.
int readLightLevel() {
  return analogRead(LIGHT_PIN);
}

// isDark()
// Returns true if the LDR reading is above the dark threshold.
bool isDark() {
  return readLightLevel() > DARK_THRESHOLD;
}

// turnLightsOn()
// Sets both LEDs HIGH and updates state.
void turnLightsOn() {
  lightsOn = true;
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
}

// turnLightsOff()
// Sets both LEDs LOW and updates state.
void turnLightsOff() {
  lightsOn = false;
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
}

// toggleLights()
// Switches lights ON if they are OFF, and OFF if they are ON.
void toggleLights() {
  if (lightsOn) {
    turnLightsOff();
  } else {
    turnLightsOn();
  }
}

// handleMotion()
// Checks light level and toggles lights if it is dark.
// Prints result to Serial Monitor.
void handleMotion() {
  int light = readLightLevel();
  Serial.print("PIR triggered. Light level: ");
  Serial.println(light);

  if (isDark()) {
    toggleLights();
    Serial.println(lightsOn ? "Motion in dark: lights ON." : "Motion in dark: lights OFF (toggled).");
  } else {
    Serial.println("Motion detected but bright - no action taken.");
  }
}

// handleSwitch()
// Debounces the slider switch and toggles the lights.
// Prints result to Serial Monitor.
void handleSwitch() {
  delay(50);  // Debounce
  int state = digitalRead(SWITCH_PIN);
  Serial.print("Switch toggled: ");
  Serial.println(state == LOW ? "ON" : "OFF");

  toggleLights();
  Serial.println(lightsOn ? "Lights ON via switch." : "Lights OFF via switch.");
}

// setup()
// Configures pins and attaches interrupts.
void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN,    INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN,   OUTPUT);
  pinMode(LED2_PIN,   OUTPUT);

  turnLightsOff();

  // interrupt on digital pins
  attachInterrupt(digitalPinToInterrupt(PIR_PIN),    onMotionDetected, RISING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), onSwitchToggled,  CHANGE);

  Serial.print("Initial light level: ");
  Serial.println(readLightLevel());
  Serial.println("System ready.");
}

// loop()
// Checks interrupt flags and calls the appropriate handler.
void loop() {
  if (motionDetected) {
    motionDetected = false;
    handleMotion();
  }

  if (switchTriggered) {
    switchTriggered = false;
    handleSwitch();
  }

  static unsigned long lastReport = 0;
  if (millis() - lastReport >= 10000) {
    lastReport = millis();
    Serial.print("Status - Light: ");
    Serial.print(readLightLevel());
    Serial.print(" | Lights: ");
    Serial.println(lightsOn ? "ON" : "OFF");
  }
}
