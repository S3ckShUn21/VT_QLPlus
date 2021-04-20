#include <Servo.h>

#define ACTUATOR 3

Servo actuator;
uint8_t value;

void setup() {
    actuator.attach(ACTUATOR, 1000, 2000);
    pinMode(LED_BUILTIN, OUTPUT);
    value = 0;
}

void loop() {
  actuator.write(value); // write the position for the actuator to go to
  digitalWrite(LED_BUILTIN, value); // if the value > 0 it will be on, otherwise it'll be off
  delay(5000); // wait five seconds for the actuator to move
  
  if(value) { // flip the value each round
    value = 0;
  } else {
    value = 180;
  }
}
