#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

//1 daylight analog in
//2 switch digital in
//3 daylight power out
//4 switch power out
//B0 lamp out1
//B1 lamp out2







#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

void ClockInit();
uint16_t ARead(uint8_t pin);

int main(void) {
	ClockInit();
	sei();

	DDRA = 0;PORTA = 0;//all input
	DDRB = 0;PORTB = 0;//all input
	
	uint8_t IsDaylight = 0;
  uint8_t DaylightCount = 0;
	PRR = _BV(PRADC) | _BV(PRUSI);
  MCUCR |= _BV(SE);
  MCUCR |= _BV(SM1);
  ACSR |= _BV(ACD);
  WDTCSR |= _BV(WDCE);
  WDTCSR |= _BV(WDP1);//0 0 1 0 8K cycles 0.0625 s
  WDTCSR |= _BV(WDIE);//0 1 Running Interrupt

  while(1)
  {
    DaylightCount++;
    if(DaylightCount==32)//2s check daylight
    {
      DDRA |= _BV(3);
      uint16_t val = ARead(1);
      DDRA &= ~_BV(3);
      if(val>512)
      {
        IsDaylight = 1;
      }
      else
      {
        IsDaylight = 0;
      }
      DaylightCount = 0;
    }
    if(IsDaylight)
    {
      DDRA |= _BV(4);
      uint8_t doorOpen = PINA & _BV(2);
      DDRA &= ~_BV(4);
      if(doorOpen)
      {
        DDRB |= _BV(0);
        DDRB |= _BV(1);

        PORTA |= _BV(0);
        TCCR1A = 0;
        TCCR1B = 2;//  1/8
        TCCR1C = 0;
        TIMSK1 = 0;
        TCNT1 = 0;//timer reset
        TIFR1 |= _BV(TOV1);//overflow flg reset
        while(currTick<10)
        {
          ;
        }
        PORTA &= ~_BV(0);

        
        
        DDRB &= ~_BV(0);
        DDRB &= ~_BV(1);
      }
    }
    asm volatile("sleep");
  }
}

void ClockInit() {
	CLKPR = _BV(CLKPCE);//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 0;//8Mhz
	CLKPR = _BV(CLKPS3);//1 0 0 0 1/256  31.25khz
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