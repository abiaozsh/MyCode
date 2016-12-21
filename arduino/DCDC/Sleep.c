#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

volatile uint8_t FStart = 0;

void adj();

void wait(unsigned int ticks)
{
  TCCR1A = 0;
  TCNT1 = 0;
  TCCR1B = 2;
	while(TCNT1<ticks)
	{
		//asm volatile("nop");
	}
}

int main(void) {
	//³õÊ¼»¯Ê±ÖÓ£º1MHz -> 8MHz
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	CLKPR = 1;//1/1 //8MHz
	//0 0 0 0 0 1
	//0 0 0 1 1 2
	//0 0 1 0 2 4
	//0 0 1 1 3 8
	//0 1 0 0 4 16
	//0 1 0 1 5 32
	//0 1 1 0 6 64
	//0 1 1 1 7 128
	//1 0 0 0 8 256 
	
	
	DDRA = 255;
	PORTA = 0;
	DDRB = 15;
	PORTB = 0;
  
	DDRA &= ~ _BV(7);//sleep button
  
	//DDRA &= ~ _BV(5);
  
	while(1){
    wait(1000);
    DDRB &= ~ _BV(3);
    wait(1000);
    DDRB |= _BV(3);
    
    if(!(PINA |= _BV(7))){
      MCUCR |= _BV(SE) | _BV(SM1);
      PRR |= _BV(PRTIM1) | _BV(PRTIM0) | _BV(PRUSI) | _BV(PRADC);
    }
	}
}

ISR(PCINT0_vect){
  PRR &= ~(_BV(PRTIM1) | _BV(PRTIM0) | _BV(PRUSI) | _BV(PRADC));
}
