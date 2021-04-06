#define BAUDRATE_115200 8 // Based on 16MHz clock
                          // pg 165 Atmega328p datasheet

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


void initADC() {
  ADCSRA = 0 ; // clear ADCSRA register
  ADCSRB = 0 ; // clear ADCSRB register

  ADMUX = 0x00; // set ADC to read from A0
  ADMUX |= (1 << REFS0); // set reference voltage to internal 5V
// ADMUX |= (1 << ADLAR); // set the adc buffer to left align the reading
  

  // sample rate =  [ADC clock ] / [ prescaler ] / [ conversion clock cycles ]
  // ADC takes 13 cycles
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // prescaler = 64
  // sample rate = 19.231kHz

  
  ADCSRA |= (1 << ADATE) ; // enable ADC auto trigger conversion
  ADCSRA |= (1 << ADIE) ; // enable interrupt when measurement complete

  ADCSRA |= (1 << ADEN) ; // enable ADC
  ADCSRA |= (1 << ADSC) ; // start ADC measurements
}

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

void sendChar(byte val) {
  while (!( UCSR0A & (1<<UDRE0))); // Wait for empty transmit buffer
  UDR0 = val;
}
