#include "FastSerial.h"
#include <avr/io.h>
#include <stdio.h>

void initSerial(unsigned int baudRateSetting) {
  
  // Set baudrate
  UBRR0H = (baudRateSetting >> 8);
  UBRR0L = baudRateSetting;

  // set serial data packet format
  UCSR0C = 0b00000110;  // 8 data bits, 1 stop bit
  
  UCSR0B = (1<<TXEN0);  // enable transmission
// UCSR0B = (1<<RXEN0);  // enable recieve
}

void println16bit(unsigned int val) {
  // format value
  char buff[6];
  sprintf(buff, "%u", val);

  // print string
  int i = 0;
  while( buff[i] ) {
    while (!( UCSR0A & (1<<UDRE0))); // Wait for empty transmit buffer
    UDR0 =buff[i++]; 
  }

  // print newline
  while (!( UCSR0A & (1<<UDRE0))); // Wait for empty transmit buffer
  UDR0 = '\n';
}

void sendChar(char val) {
  while (!( UCSR0A & (1<<UDRE0))); // Wait for empty transmit buffer
  UDR0 = val;
}
