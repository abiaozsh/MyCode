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
    LOW;
    TCNT1 = 0;//timer reset
    while(TCNT1<(10));
    HIGH;
    while(TCNT1<(20));
    LOW;
    while(TCNT1<(30));
    HIGH;
    while(TCNT1<(40));
    LOW;
    while(TCNT1<(90));
    HIGH;
    while(TCNT1<(140));
    LOW;
    
    while(TCNT1<(5000));

    }
	return 0;
}
