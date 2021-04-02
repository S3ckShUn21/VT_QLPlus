#include <Servo.h>

// Pin numbers for the hardware
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

#define LOW_BAT_PIN 4

#define MYOWARE_SENSOR_PIN A5
#define ACTUATOR_PIN 3

// The bit field is set up as
// 0b {RED} {GREEN} {BLUE}
enum LEDColor {
  OFF = 0b000,
  WHITE = 0b111,
  RED = 0b100,
  GREEN = 0b010,
  BLUE = 0b001,
  YELLOW = 0b110,
  CYAN = 0b011,
  MAGENTA = 0b101
};

/*
   Global enum for hand state positions
*/
enum HandPos {
  CLOSED = 1500,
  MIDDLE = 1650,
  OPEN = 1800
};

// Objects
Servo hand; // The object for the linear actuator

// Global Vars
uint16_t  flexReading = 0;      // The value read from the MyoWare sensor
uint16_t  flexLimit = 600;      // This value has been calculated experimentally (may need to change per user)
uint16_t  waitTime = 800;       // Amount of time between actuations in ms
uint16_t  handState = MIDDLE;   // Uses the HandPos enum above (represents us for servo timing)
bool      canFlex = true;       // turns back to TRUE after waiting 'waitTime' ms from last actuation

unsigned long lastFlexTime = 0; // stores when the last flex happend -- used to determine when to
// allow to user to flex again
// Debug Data
char plotData[20];

void setup() {

  // Setup debug
  Serial.begin(115200);
  Serial.println(F("VT QL+ Hand Actuator initializing..."));

  // Enable all the LED pina
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Setup input pin
  pinMode(LOW_BAT_PIN, INPUT_PULLUP);

  // Setup hardware
  pinMode(MYOWARE_SENSOR_PIN, INPUT);
  hand.attach(ACTUATOR_PIN, 1000, 2000); // sets up the actuator so the range or motion is [1000, 2000]

  // Finish initialization
  Serial.println(F("VT QL+ Hand Actuator setup complete"));
  Serial.println(F("Zero,FlexReading,FlexLimit,OneThousand"));

  // The guantlet is on at this point
  // so enable the WHITE led
  setLED(WHITE);
}

void loop() {

  // Poll and plot data
  flexReading = analogRead(MYOWARE_SENSOR_PIN); // read
  sprintf(plotData, "0,%05d,%05d,1000\n", flexLimit, flexReading); // format
  Serial.println(plotData); // plot

  // Check if the user is flexing
  if ( flexReading > flexLimit  && canFlex ) {
    // Update the position
    switch (handState) {
      case CLOSED:
        handState = OPEN;
        break;
      case MIDDLE:
        handState = CLOSED;
        break;
      case OPEN:
        handState = MIDDLE;
        break;
    }
    // Tell the hand to move to the updated position
    hand.writeMicroseconds(handState);
    // Update vars so the user can't flex until 'waitTime' has passed
    lastFlexTime = millis();
    canFlex = false;

    // setLED(BLUE);
    // delay(500);
  }

  // Check if 'waitTime' has passed yet
  if ( !canFlex &&  (millis() - lastFlexTime > waitTime) ) {
    canFlex = true;
  }

  // Check sequence for LED status
  if ( !digitalRead(LOW_BAT_PIN) ) {
    setLED(RED);
  } else {
    setLED(WHITE);
  }

} // End Loop


void setLED( LEDColor col ) {
  digitalWrite( RED_PIN, !((col & 0b100) >> 2) );
  digitalWrite( GREEN_PIN, !((col & 0b010) >> 1) );
  digitalWrite( BLUE_PIN, !((col & 0b001) >> 0) );
}
