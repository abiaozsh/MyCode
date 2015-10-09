#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define LEDInit PORTB &= ~_BV(3);
#define LEDOn   DDRB |= _BV(3);
#define LEDOff  DDRB &= ~_BV(3);

int main(void) {

  LEDInit;
  
  uint32_t i;
  for(;;) {
    LEDOn;
    for(i = 0;i<100000;i++)
    {
      asm volatile("nop");
    }
    LEDOff;
    for(i = 0;i<100000;i++)
    {
      asm volatile("nop");
    }
  }
}
