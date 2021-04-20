#ifndef _ADC_H_
#define _ADC_H_

// As of right now this only inits reading from A0
// Sets up auto conversion and interupt on completion
// Use the ISR( ADC_vect ) to run code on converions  
void initADC();

// Will enable the ADC, turn on auto trigger, and then start a conversion
void startADC();

// Will turn off auto trigger to stop future conversions
void pauseADC();

// Hard disables the ADC will currupt current conversion
void stopADC();

#endif /* _ADC_H_ */
