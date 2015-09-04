#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRA = 0x99;
	DDRA |= _BV(0);//pMode(0, OUTPUT);    
	for (;;) {
		PORTA |= _BV(0);
		PORTA &= ~_BV(0);
	}
}

ISR(INT0_vect			){return;}
ISR(PCINT0_vect			){return;}
ISR(PCINT1_vect			){return;}
ISR(WDT_vect			){return;}
ISR(TIMER1_CAPT_vect	){return;}
ISR(TIM1_COMPA_vect		){return;}
ISR(TIM1_COMPB_vect		){return;}
ISR(TIM1_OVF_vect		){return;}
ISR(TIM0_COMPA_vect		){return;}
ISR(TIM0_COMPB_vect		){return;}
ISR(TIM0_OVF_vect		){return;}
ISR(ANA_COMP_vect		){return;}
ISR(ADC_vect			){return;}
ISR(EE_RDY_vect			){return;}
ISR(USI_START_vect		){return;}
ISR(USI_OVF_vect		){return;}
