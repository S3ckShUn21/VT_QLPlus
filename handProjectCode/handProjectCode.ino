#include <Servo.h>

#define MUSCLE_SENSOR_PIN A0

Servo myservo;  // create servo object to control a servo

char plotData[16];
int MuscleSensor = 0;
bool flexed = false;
int flexLimit = 100;
long startTime = 0;
int flexTime = 1000; //written in millisec
int handState = 2000; //open=2000, mid=1500, closed=1000 in uS
uint8_t led_pin = 7;

void setup() {
  Serial.begin(115200);

  // Setup the debug LED
  pinMode(led_pin, OUTPUT );
  digitalWrite(led_pin, HIGH);
  delay(1000);
  digitalWrite(led_pin, LOW);

  myservo.attach(9);  // Sends the PWM signal for the servo over pin 9

  pinMode(MUSCLE_SENSOR_PIN, INPUT); // Allows the data to be read from the sensor on the pin A0
}

void loop() {
  MuscleSensor = analogRead(A0); // Read data
  sprintf(plotData, "0 %d 300", MuscleSensor); // format
  Serial.println(plotData); // plot
  checkForFlex(); // check if flexing
}


/*
 * A basic state machine, checking...
 *  - if we were flexing and stopped
 *  - if we weren't flexing and started
 */
void checkForFlex() {
  if (flexed && MuscleSensor < flexLimit) {
    int delta = millis() - startTime;
    if (delta > flexTime) {
      triggerHand();
    }
    flexed = false;
  }
  else if (!flexed && MuscleSensor > flexLimit) {
    startTime = millis();
    flexed = true;
  }
}


/*
 * Handles debug led and moving the hand between states
 */
void triggerHand() {

  // Blink the LED for a sanity check that we're doing something right
  digitalWrite(led_pin, HIGH);
  delay(250);
  digitalWrite(led_pin, LOW);
  delay(250);
  digitalWrite(led_pin, HIGH);
  delay(250);
  digitalWrite(led_pin, LOW);

  // Move the hand between the three different positions we have
  if (handState == 2000) {
    interpolateHand( handState, 1500 );
    handState = 1500;
  }
  else if (handState == 1500) {
    interpolateHand( handState, 1000 );
    handState = 1000;
  }
  else {
    interpolateHand( handState, 2000 );
    handState = 2000;
  }
}


/*
 * An interpolating function, gradually moving the hand between positions
 */
void interpolateHand( int startPos, int endPos ) {
  // If moving from out to in
  if ( endPos - startPos < 0 ) {
    for ( ; startPos > endPos; --startPos ) {
      myservo.writeMicroseconds(startPos);
      delay(1);
    }
  }
  // If moving from in to out
  else {
    for ( ; startPos < endPos; ++startPos ) {
      myservo.writeMicroseconds(startPos);
      delay(1);
    }
  }
}
