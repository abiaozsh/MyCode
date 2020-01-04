#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define PIN 2

#define HIGH PORTC |=_BV(PIN)
#define LOW PORTC &=~_BV(PIN)

int main()
{
  TCCR1B = 1;
  DDRC |= _BV(PIN);
  
  while(1){
    LOW;TCNT1 = 0;
    while(TCNT1<100);

    HIGH;TCNT1 = 0;
    while(TCNT1<10);
    LOW;TCNT1 = 0;
    while(TCNT1<10);

    HIGH;TCNT1 = 0;
    while(TCNT1<5);
    LOW;TCNT1 = 0;
    while(TCNT1<5);

    for(uint8_t i=0;i<10;i++){
      HIGH;
      LOW;
      HIGH;
      LOW;
    }
    for(uint8_t i=0;i<10;i++){
      HIGH;
      LOW;
      HIGH;
      LOW;
    }
    for(uint8_t i=0;i<10;i++){
      HIGH;
      LOW;
      HIGH;
      LOW;
      HIGH;
      LOW;
      HIGH;
      LOW;
      HIGH;
      LOW;
      HIGH;
      LOW;
      HIGH;
      LOW;
      HIGH;
      LOW;
      HIGH;
      LOW;
      HIGH;
      LOW;
      HIGH;
      LOW;
      HIGH;
      LOW;
    }
    
    HIGH;TCNT1 = 0;
    while(TCNT1<100);

  }
	return 0;
}
