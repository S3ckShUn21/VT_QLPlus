#include "ADC.h"
#include "FastSerial.h"

volatile unsigned int ADCReading = 0;
volatile unsigned int counter = 0;

void setup() {
  initADC();
  initSerial(BAUDRATE_115200);
}

void loop() {
  if( ADCReading ) {
    println16bit(ADCReading);
    ADCReading = 0;
  }
}

ISR( ADC_vect ) {
  ADCReading = ADCL;
  ADCReading |= (ADCH << 8);
}
