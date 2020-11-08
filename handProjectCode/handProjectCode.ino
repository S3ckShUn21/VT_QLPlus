#include <Servo.h>

/*
   Global enum for hand state positions
*/
enum HandPos {
  CLOSED = 1000,
  MIDDLE = 1500,
  OPEN = 2000
};

/*
   Begin Variables
*/

// Pin numbers for the hardware
#define MYOWARE_SENSOR_PIN A0
#define ACTUATOR_PIN 3
#define LED_PIN 7

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

/*
   End Variables
*/


/*
   Initial setup of the hand
*/
void setup() 
{
  // Setup debug
  Serial.begin(115200);
  Serial.println(F("VT QL+ Hand Actuator initializing..."));

  // Setup hardware
  pinMode(MYOWARE_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  hand.attach(ACTUATOR_PIN, 1000, 2000); // sets up the actuator so the range or motion is [1000, 2000]

  // Blink the LED to show setup complete
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);

  // Finish initialization
  Serial.println(F("VT QL+ Hand Actuator setup complete"));
  Serial.println(F("Zero,FlexReading,FlexLimit,OneThousand"));
}


/*
   Main loop
*/
void loop()
{

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
    // Blink the LED to inform the user they hit the flex limit
    digitalWrite( LED_PIN, HIGH );
    delay(20);
    digitalWrite( LED_PIN, LOW );
  }

  // Check if 'waitTime' has passed yet
  if ( !canFlex &&  (millis() - lastFlexTime > waitTime) ) {
    canFlex = true;
  }

}
