#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

void ClockInit();

int main(void) {
	ClockInit();

	DDRA = 0;PORTA = 0;//all input
	DDRB = 0;PORTB = 0;//all input

	DDRA |= _BV(0);
	
	TCCR0A = 0;
	TCCR0B = 3;
	TIMSK0 = _BV(TOIE1);
	
	sei();
	
	PRR = _BV(PRADC) | _BV(PRUSI);
  MCUCR |= _BV(SE);
  ACSR |= _BV(ACD);
  
	//Ö÷Ñ­»·
	while(1)
	{
		asm volatile("sleep");
	}
}

void ClockInit() {
	CLKPR = _BV(CLKPCE);//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 0;//8Mhz
	CLKPR = _BV(CLKPS3);//1 0 0 0 1/256  31.25khz
}

ISR(TIM0_OVF_vect){
	PINA |= _BV(0);
}

/*
ISR(INT0_vect){return;}
ISR(PCINT0_vect){return;}
ISR(PCINT1_vect){return;}
ISR(WDT_vect){return;}
ISR(TIMER1_CAPT_vect){return;}
ISR(TIM1_COMPA_vect){return;}
ISR(TIM1_COMPB_vect){return;}
ISR(TIM1_OVF_vect){return;}
ISR(TIM0_COMPA_vect){return;}
ISR(TIM0_COMPB_vect){return;}
ISR(TIM0_OVF_vect){return;}
ISR(ANA_COMP_vect){return;}
ISR(ADC_vect){return;}
ISR(EE_RDY_vect){return;}
ISR(USI_START_vect){return;}
ISR(USI_OVF_vect){return;}
*/