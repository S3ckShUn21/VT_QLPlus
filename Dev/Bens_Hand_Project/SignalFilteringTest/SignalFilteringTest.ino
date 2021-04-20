#include "ADC.h"
#include "FastSerial.h"

volatile unsigned int counter = 0;

void setup() {
  initADC();
  initSerial(BAUDRATE_115200);
}

void loop() {
  
}

ISR( ADC_vect ) {
  println8bit(ADCH);
  sendChar('\n');
  sendChar('\r');
}
