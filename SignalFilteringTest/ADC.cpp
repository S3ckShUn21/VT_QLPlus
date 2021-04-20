#include "ADC.h"
#include <avr/io.h>

// As of right now this only inits reading from A0
// Sets up auto conversion and interupt on completion
// Use the ISR( ADC_vect ) to run code on completion
void initADC() {
  ADCSRA = 0 ; // clear ADCSRA register
  ADCSRB = 0 ; // clear ADCSRB register

  ADMUX = 0x00; // set ADC to read from A0
  ADMUX |= (1 << REFS0); // set reference voltage to internal 5V
  ADMUX |= (1 << ADLAR); // set the adc buffer to left align the reading
  

  // sample rate =  [ADC clock ] / [ prescaler ] / [ conversion clock cycles ]
  // ADC takes 13 cycles
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // prescaler = 64
  // sample rate = 19.231kHz

  
  ADCSRA |= (1 << ADATE) ; // enable ADC auto trigger conversion
  ADCSRA |= (1 << ADIE) ; // enable interrupt when measurement complete

  ADCSRA |= (1 << ADEN) ; // enable ADC
  ADCSRA |= (1 << ADSC) ; // start ADC measurements
}

// Will enable the ADC, turn on auto trigger, and then start a conversion
void startADC() {
  ADCSRA |= (1 << ADATE) ; // enable ADC auto trigger conversion
  ADCSRA |= (1 << ADEN) ; // enable ADC
  ADCSRA |= (1 << ADSC) ; // start ADC measurements
}

// Will turn off auto trigger to stop future conversions
void pauseADC() {
  ADCSRA &= ~(1 << ADATE) ; // disable ADC auto trigger conversion
}

// Hard disables the ADC will currupt current conversion
void stopADC(){
  ADCSRA &= ~(1 << ADATE) ; // disable ADC auto trigger conversion
  ADCSRA &= ~(1 << ADEN) ; // disable ADC
}
