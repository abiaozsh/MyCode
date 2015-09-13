#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

void ClockInit();

int main(void) {
	ClockInit();
	sei();

	DDRA = 0;PORTA = 0;//all input
	DDRB = 0;PORTB = 0;//all input
	
	
	PRR = _BV(PRADC) | _BV(PRUSI);
  MCUCR |= _BV(SE);
  MCUCR |= _BV(SM1);
  ACSR |= _BV(ACD);

  WDTCSR |= _BV(WDCE);
 //WDP3 WDP2 WDP1 WDP0
//Number of WDT Oscillator
//Cycles
//Typical Time-out at
//VCC = 5.0V
//0 0 0 0 2K cycles 16 ms
//0 0 0 1 4K cycles 32 ms
//WDTCSR |= _BV(WDP1);//0 0 1 0 8K cycles 64 ms
//WDTCSR |= _BV(WDP1);WDTCSR |= _BV(WDP0);//0 0 1 1 16K cycles 0.125 s
//0 1 0 0 32K cycles 0.25 s
WDTCSR |= _BV(WDP2);WDTCSR |= _BV(WDP0);//0 1 0 1 64K cycles 0.5 s
//0 1 1 0 128K cycles 1.0 s
//0 1 1 1 256K cycles 2.0 s
//1 0 0 0 512K cycles 4.0 s
//1 0 0 1 1024K cycles 8.0 s
 
  
  
  //WDE WDIE Watchdog Timer State Action on Time-out
//0 0 Stopped None
WDTCSR |= _BV(WDIE);//0 1 Running Interrupt
//1 0 Running Reset
//1 1 Running Interrupt  


	while(1)
	{
	DDRA |= _BV(0);
	//DDRA |= _BV(1);
	
	PORTA |= _BV(0);
  // COM1A1 COM1A0 COM1B1 COM1B0 每 每 WGM11 WGM10
  TCCR1A = 0;
  //ICNC1 ICES1 每 WGM13 WGM12 CS12 CS11 CS10
  //0 0 1 clkI/O/1 (No prescaling)
  //0 1 0 clkI/O/8 (From prescaler)
  //0 1 1 clkI/O/64 (From prescaler)
  //1 0 0 clkI/O/256 (From prescaler)
  //1 0 1 clkI/O/1024 (From prescaler)

  //TCCR1B = 1;//  1/1
  TCCR1B = 2;//  1/8
  //TCCR1B = 3;//  1/64

  //FOC1A FOC1B 每 每 每 每 每 每
  TCCR1C = 0;
  //每 每 ICIE1 每 每 OCIE1B OCIE1A TOIE1
  TIMSK1 = 0;
		TCNT1 = 0;//timer reset
	TIFR1 |= _BV(TOV1);//overflow flg reset
	while(currTick<10)
	{
		;
	}

	
	PORTA &= ~_BV(0);
	
	DDRA &= ~_BV(0);
	//DDRA &= ~_BV(1);
	asm volatile("sleep");
  }
}

void ClockInit() {
	CLKPR = _BV(CLKPCE);//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 0;//8Mhz
	CLKPR = _BV(CLKPS3);//1 0 0 0 1/256  31.25khz
}

ISR(WDT_vect){return;}

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