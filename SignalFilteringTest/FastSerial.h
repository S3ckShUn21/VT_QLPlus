#ifndef _FAST_SERIAL_H_
#define _FAST_SERIAL_H_

//////////////////////////////////////
//
// Baudrates based on 16MHz clock
//  pg 165 Atmega328p datasheet
//
//////////////////////////////////////

#define BAUDRATE_9600         103
#define BAUDRATE_38400        25
#define BAUDRATE_115200       8

//////////////////////////////////////
//
// Functions
//
//////////////////////////////////////

// Sets up serial
// Currently only allows transmit
void initSerial(unsigned int baudRateSetting);

// Will print a 16bit unsigned value as a string
// End the string with '\n'
void println16bit(unsigned int val);

// Sends a single char
void sendChar(char val);

#endif /* _FAST_SERIAL_H_ */
