#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

//2 daylight power out
//3 switch power out
//4 daylight analog in
//5 switch in
//B0 lamp out

#define PowerOn DDRA |= _BV(3)
#define PowerOn DDRA |= _BV(3)

void ClockInit();

int main(void) {
  ClockInit();

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

uint16_t daylightCount = 0;

void ClockInit() {
	CLKPR = _BV(CLKPCE);//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 0;//8Mhz
	CLKPR = _BV(CLKPS3);//1 0 0 0 1/256  31.25khz
}

ISR(TIM0_OVF_vect){
  daylightCount++;
  if(daylightCount)
	PINA |= _BV(0);
}

uint16_t ARead(uint8_t pin)
{
  ADCSRA |= _BV(ADEN);
  ADMUX = pin;

	sbi(ADCSRA, ADSC);

	while (bit_is_set(ADCSRA, ADSC));

  ADCSRA &= ~_BV(ADEN);
	return ADC;
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