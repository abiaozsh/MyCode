#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define IRPIN (PINA & _BV(6))

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

#define LONGON  PORTB |= _BV(0)
#define LONGOFF PORTB &=~_BV(0)
#define AUTOON  PORTB |= _BV(1)
#define AUTOOFF PORTB &=~_BV(1)

void SetClockHigh();
void SetClockLow();
uint8_t GetIR();
  
volatile uint8_t status = 0;//off


// 1 2 3 4 switch in
// 5 IRin

int main(void)
{
  SetClockLow();
  sei();
  GIMSK |= _BV(PCIE0);
  PCMSK0 |= _BV(PCINT6);
  
  //8Mhz = 0.000000125 s
  //0.000032
  //0.032768s/tick
  TCCR1A = 0;
  //0.032768s/tick
  //total 2147.483648s
  TCCR1B = 5;//  1 0 1 clkI/O/1024 (From prescaler)
  TCCR1C = 0;
  TIMSK1 = 0;

  DDRA = _BV(0);
  DDRB = 3;
  //PORTA |= 0x1E;//DEBUG
  
  TCNT1 = 0;TIFR1 |= _BV(TOV1);
  
  uint8_t lastVal = PINA&0x1E;//(1 2 3 4)bit
  for (;;) 
  {
    uint8_t val = PINA&0x1E;
    PINA |= 1;
    if(val!=lastVal)
    {
      while(currTick<15);//0.5s
      if(currTick>60)//3s 单击
      {
        if(status==0)
        {
          status = 1;
          AUTOON;
          LONGOFF;
        }
        else
        {
          status = 0;
          AUTOOFF;
          LONGOFF;
        }
      }
      else//双击
      {
        status = 2;
        LONGON;
        AUTOOFF;
      }
      TCNT1 = 0;TIFR1 |= _BV(TOV1);
      lastVal = val;
      
    }
  }
}
uint8_t GetIR(){
  uint8_t data = 0;
  uint8_t datahi = 0;
  uint8_t dataidx = 0;
  uint8_t byteidx = 0;
  TCCR0A = 0;
  TCCR0B = 4;//0 1 1 clkI/O/64 (From prescaler)
  //while(IRPIN);//等待低电平 下降沿
  while(!IRPIN);
  while(IRPIN);
  uint8_t OLDflg = IRPIN;
  TCNT0 = 0;
  TIMSK0 = 0;
  TIFR0 |= _BV(TOV0);
  while((!(TIFR0 & _BV(TOV0))) && (byteidx<4))
  {
    if(OLDflg != IRPIN)
    {
      if(IRPIN)
      {
      }
      else
      {
        //35~105 70
        data>>=1;
        if(TCNT0<35)
        {
          data|=0x80;
        }
        dataidx++;
        if(dataidx==8)
        {
          dataidx=0;
          switch(byteidx)
          {
            case 0:
              if(data!=0xFF)return 0xFF;
              break;
            case 1:
              if(data!=0x00)return 0xFF;
              break;
            case 2:
              datahi = ~data;
              break;
          }
          byteidx++;
        }
      }
      TCNT0 = 0;
      OLDflg = IRPIN;
    }
  }
  if(data!=datahi)
  {
    return 0xFF;
  }
  return data;
}

void SetClockHigh() {
	CLKPR = _BV(CLKPCE);//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	CLKPR = 0;//8Mhz
}
void SetClockLow() {
	CLKPR = _BV(CLKPCE);//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	CLKPR = _BV(CLKPS3);//1 0 0 0 1/256  31.25khz
}

ISR(PCINT0_vect){
  SetClockHigh();
  uint8_t val = GetIR();

  if(val == 0x16)
  {
    status = 0;
    AUTOOFF;
    LONGOFF;
  }
  else if(val == 0x19)
  {
    status = 1;
    AUTOON;
    LONGOFF;
  }
  else if(val == 0x0D)
  {
    status = 2;
    LONGON;
    AUTOOFF;
  }
  SetClockLow();
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