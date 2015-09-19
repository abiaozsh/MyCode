#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

//gnd
#define ledV    0 //led v--  out
#define senseIN 1 //sense in
#define ledIN   2 //led in
#define senseV  3 //sense v--
#define voltIN  4 //volt sense
#define voltV   5 //volt sense v--
//
//vcc
//b0
//b1
//rst
//b2
//a7 serial out
//a6

volatile uint16_t lightCount = 0;
volatile uint8_t curLamp = 1;


#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

void wait(uint16_t length)
{
  TCCR1A = 0;
  TCCR1B = 1;
  TCCR1C = 0;
  TIMSK1 = 0;
    TCNT1 = 0;TIFR1 |= _BV(TOV1);//overflow flg reset
    while(currTick<length)
    {
      ;
    }

}

void ClockInit();
uint8_t ARead(uint8_t pin);

int main(void) {
  ClockInit();
  sei();

  DDRA = 0;PORTA = 0;//all input
  DDRB = 0;PORTB = 0;//all input

  uint8_t IsNight = 0;
  uint8_t DaylightCount = 31;
  //PRR = _BV(PRADC) | _BV(PRUSI);
  MCUCR |= _BV(SE);
  MCUCR |= _BV(SM1);
 // ACSR |= _BV(ACD);
  WDTCSR |= _BV(WDCE);
  WDTCSR |= _BV(WDP1);//0 0 1 0 8K cycles 0.0625 s
  WDTCSR |= _BV(WDIE);//0 1 Running Interrupt

  TCCR0B = 2;
  OCR0A = 20;
  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);
  
  while(1)
  {
    DaylightCount++;
    if(DaylightCount==32)//2s check daylight
    {
      DDRA |= _BV(senseV);
      uint8_t val = ARead(senseIN);
      DDRA &= ~_BV(senseV);
      if(val>128)//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      {
        IsNight = 1;
      }
      else
      {
        IsNight = 0;
      }
      DaylightCount = 0;
    }
    if(IsNight)
    {
      DDRA |= _BV(ledV);
	  uint8_t doorOpen = ARead(ledIN);
      DDRA &= ~_BV(ledV);
      if(doorOpen>128)//doorOpen>600
      {
        DDRB |= _BV(0);
        DDRB |= _BV(1);

        uint8_t volt;
        lightCount = 0;
        while(lightCount<100)//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        {
          DDRA |= _BV(voltV);
          volt = ARead(voltIN);
          DDRA &= ~_BV(voltV);
          if(volt<128)//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
          {
          }
        }
        
        DDRB &= ~_BV(0);
        DDRB &= ~_BV(1);
      }
    }
    asm volatile("sleep");
  }
}

void ClockInit() {
	//CLKPR = _BV(CLKPCE);//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 0;//8Mhz
	//CLKPR = _BV(CLKPS3);//1 0 0 0 1/256  31.25khz
}

uint8_t ARead(uint8_t pin)
{
//  wait(100);
  ADCSRA = 7;
  ADCSRB |= _BV(ADLAR);
  ADMUX = pin;
  ADCSRA |= _BV(ADEN);

  ADCSRA |= _BV(ADSC);

  while (bit_is_set(ADCSRA, ADSC));
  uint8_t ret = ADCH;
  ADCSRA &= ~_BV(ADEN);
  return ret;
}

ISR(WDT_vect){return;}

ISR(TIM0_COMPA_vect){
  PORTB &= ~_BV(0);
  PORTB &= ~_BV(1);
}

ISR(TIM0_OVF_vect){
  PORTB |= curLamp;
  curLamp ^= 3;
  lightCount++;
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