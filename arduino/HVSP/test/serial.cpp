#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define DDR_OE_ON    DDRB  |=  _BV(3)
#define DDR_OE_OFF   DDRB  &= ~_BV(3)
#define PORT_OE_ON   PORTB |=  _BV(3)
#define PORT_OE_OFF  PORTB &= ~_BV(3)


void wait(uint32_t n){
	{
		uint32_t i = 0;
		for(i=0;i<n;i++)
		{
			asm volatile("nop");
		}
	}
  
}

int main(void) {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
	PORT_OE_OFF;

	for(;;)
	{
		DDR_OE_ON;
		wait(100000);
		DDR_OE_OFF;
		wait(100000);
	}
}
