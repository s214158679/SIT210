// SIT210 - Embedded Systems Development
// Task 1.1P - Switching ON Lights

// Pin Definitions ─
const int SWITCH_PIN  = 2;   // Slide switch centre pin
const int PORCH_PIN   = 7;   // Porch light LED output pin
const int HALLWAY_PIN = 8;   // Hallway light LED output pin

// Timing Constants ─
const unsigned long PORCH_DURATION   = 30000; // 30 seconds in ms
const unsigned long HALLWAY_DURATION = 60000; // 60 seconds in ms

//State Variables ─
unsigned long porchStartTime   = 0;
unsigned long hallwayStartTime = 0;
bool porchActive    = false;
bool hallwayActive  = false;
bool lastSwitchState = false;  // tracks previous switch state

// setup()
// initial setup run on power up, configures pins
void setup() {
  Serial.begin(9600);
  pinMode(SWITCH_PIN,  INPUT_PULLUP);
  pinMode(PORCH_PIN,   OUTPUT);
  pinMode(HALLWAY_PIN, OUTPUT);
  turnOffAllLights();
  Serial.println("System ready. Slide switch to ON to activate lights.");
}

// loop() -
// Main program loop; detects switch flick and manages timers
void loop() {
  bool currentSwitchState = isSwitchOn();

  // Only trigger on the transition from OFF to ON
  if (currentSwitchState && !lastSwitchState) {
    activateLights();
  }

  lastSwitchState = currentSwitchState;
  manageLightTimers();
}

// isSwitchOn()
// Returns true when the slide switch is in the ON position
bool isSwitchOn() {
  return (digitalRead(SWITCH_PIN) == LOW);
}

// activateLights()
// Turns ON both LEDs and records the start time for each timer
void activateLights() {
  porchActive      = true;
  hallwayActive    = true;
  porchStartTime   = millis();
  hallwayStartTime = millis();
  digitalWrite(PORCH_PIN,   HIGH);
  digitalWrite(HALLWAY_PIN, HIGH);
  Serial.println("Porch Light ON: 30s");
  Serial.println("Hallway Light ON: 60s");
}

// manageLightTimers()
// Checks time elapsed and calls functions to turn each light OFF after its defined duration
void manageLightTimers() {
  if (porchActive && (millis() - porchStartTime >= PORCH_DURATION)) {
    turnOffPorchLight();
  }
  if (hallwayActive && (millis() - hallwayStartTime >= HALLWAY_DURATION)) {
    turnOffHallwayLight();
  }
}

// turnOffPorchLight()
// Turns OFF the porch LED and clears its active flag.
void turnOffPorchLight() {
  porchActive = false;
  digitalWrite(PORCH_PIN, LOW);
  Serial.println("Porch light OFF.");
}

// turnOffHallwayLight()
// Turns OFF the hallway LED and clears its active flag.
void turnOffHallwayLight() {
  hallwayActive = false;
  digitalWrite(HALLWAY_PIN, LOW);
  Serial.println("Hallway light OFF.");
}


// turnOffAllLights()
// intalise both LEDs to ensure they start OFF
void turnOffAllLights() {
  digitalWrite(PORCH_PIN,   LOW);
  digitalWrite(HALLWAY_PIN, LOW);
  porchActive   = false;
  hallwayActive = false;
}
