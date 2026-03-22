# Task 1.1P - Switching ON Lights
SIT210 - Embedded Systems Development

## What it does
Controls a porch light (30s) and hallway light (60s) when Linda slides
the switch ON after getting home in the dark.

## Hardware
- Arduino Nano 33 IoT
- 2x LEDs (red and yellow)
- 2x 220 ohm resistors
- 3-pin slide switch
- Breadboard and jumper wires

## Wiring
- D2 - centre pin of slide switch, other outer pin to GND
- D7 - resistor - porch LED (long leg), short leg to GND rail
- D8 - resistor - hallway LED (long leg), short leg to GND rail
- GND pin - negative rail on breadboard

## Code overview

**setup()** - sets pin modes and makes sure both lights are off on startup

**loop()** - checks if the switch has been flicked ON, if so triggers
the lights. Runs the timer check every cycle.

**isSwitchOn()** - reads the switch pin. Uses INPUT_PULLUP so LOW means ON.

**activateLights()** - turns both LEDs on and records the start time for
each timer.

**manageLightTimers()** - checks how long each light has been on and turns
them off once they hit 30s and 60s respectively.

**turnOffPorchLight()** - turns off porch LED and clears its active flag.

**turnOffHallwayLight()** - turns off hallway LED and clears its active flag.

**turnOffAllLights()** - turns both off, used at startup.

## Notes
- millis() is used instead of delay() so both timers can run at the
  same time without blocking each other
- lastSwitchState tracks the previous switch position so the lights
  only trigger on the flick, not the whole time the switch stays ON
- 220 ohm resistors limit current to ~6mA, safe for the Arduino's 3.3V pins
