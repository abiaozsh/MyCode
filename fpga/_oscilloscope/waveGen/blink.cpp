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
    while(TCNT1<60000);
    HIGH;TCNT1 = 0;
    while(TCNT1<60000);
    LOW;TCNT1 = 0;
    
    while(TCNT1<30000);
    HIGH;TCNT1 = 0;
    while(TCNT1<30000);
    LOW;TCNT1 = 0;
    
    while(TCNT1<10000);
    HIGH;TCNT1 = 0;
    while(TCNT1<10000);
    LOW;TCNT1 = 0;
    
    while(TCNT1<5000);
    HIGH;TCNT1 = 0;
    while(TCNT1<5000);
    LOW;TCNT1 = 0;
    
    while(TCNT1<2000);
    HIGH;TCNT1 = 0;
    while(TCNT1<2000);
    LOW;TCNT1 = 0;
    
    while(TCNT1<1000);
    HIGH;TCNT1 = 0;
    while(TCNT1<1000);
    LOW;TCNT1 = 0;
    
    while(TCNT1<100);
    HIGH;TCNT1 = 0;
    while(TCNT1<100);
    LOW;TCNT1 = 0;
    
    while(TCNT1<10);
    HIGH;TCNT1 = 0;
    while(TCNT1<10);
    LOW;TCNT1 = 0;
    
    while(TCNT1<5);
    HIGH;TCNT1 = 0;
    while(TCNT1<5);
    LOW;TCNT1 = 0;
    
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
  }
	return 0;
}
