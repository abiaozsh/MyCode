#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

void ClockInit();
uint16_t ARead(uint8_t pin);


int main(void) {
  ClockInit();
  sei();

  DDRA = 0;PORTA = 0;//all input
  DDRB = 0;PORTB = 0;//all input

  //8 1000 PWM, Phase & Freq. Correct ICR1 BOTTOM BOTTOM
  ICR1 = 0x7FFF; //8000000/32768 = 244.140625HZ
  TCCR1A |= _BV(COM1A1); //COM1A0 //WGM11 WGM10
  TCCR1B = 1;
  TCCR1B |= _BV(WGM13); //WGM12
  
  DDRA |= _BV(6);//A6out
  OCR1A = 1;
  TCNT1 = 0;
  
  while(1)
  {
	uint16_t v = ARead(7);
	if(v>100)
	{
		OCR1A = v;
	}
	else
	{
		OCR1A = 0;
	}
  }
}

void ClockInit() {
	CLKPR = _BV(CLKPCE);//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	CLKPR = 0;//8Mhz
	//CLKPR = _BV(CLKPS3);//1 0 0 0 1/256  31.25khz
}

uint16_t ARead(uint8_t pin)
{
  ADCSRA |= _BV(ADEN);
  ADMUX = pin;

  ADCSRA |= _BV(ADSC);

  while (bit_is_set(ADCSRA, ADSC));

  ADCSRA &= ~_BV(ADEN);
  return ADC;
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