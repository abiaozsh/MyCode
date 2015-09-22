#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#define IRPIN (PINA & _BV(1))

#define OCRON  DDRA |= _BV(6);
#define OCROFF DDRA &= ~_BV(6);


void SetClockHigh();
void SetClockLow();
uint8_t GetIR();

uint16_t val;
uint16_t eepromval;
uint16_t Top;
uint8_t status = 0;//0 operating 1 setting Top

int main(void) {
  SetClockLow();
  Top = 1000;
  //8 1000 PWM, Phase & Freq. Correct ICR1 BOTTOM BOTTOM
  ICR1 = 10000; //8000000/10000 = 800HZ
  TCCR1A |= _BV(COM1A1); //COM1A0 //WGM11 WGM10
  TCCR1B = 1;
  TCCR1B |= _BV(WGM13); //WGM12
  
  OCR1A = 0;
  OCRON;
  eepromval = eeprom_read_word(0);
  val = eepromval;
  if(val>Top)
  {
    val = Top;
  }
  TCNT1 = 0;
  
  while(1)
  {
    uint8_t indata = GetIR();
    
    switch(indata)
    {
      case 0x45: //off
        val = 0;
        break;
      case 0x47: //on
        val = Top;
        break;
      case 0x44: //<<
      {
        uint16_t dif = Top/10;
        if(val>dif)
        {
          val-=dif;
        }
        else
        {
          val = 0;
        }
        break;
      }
      case 0x40: //>>
      {
        uint16_t dif = Top/10;
        if(Top-val>dif)
        {
          val+=dif;
        }
        else
        {
          val = Top;
        }
        break;
      }
      case 0x07: //<
      {
        uint16_t dif = Top/100;
        if(val>dif)
        {
          val-=dif;
        }
        else
        {
          val = 0;
        }
        break;
      }
      case 0x15: //>
      {
        uint16_t dif = Top/100;
        if(Top-val>dif)
        {
          val+=dif;
        }
        else
        {
          val = Top;
        }
        break;
      }
      case 0x0C: //1
      {
        uint32_t tmp = Top*1/10;
        val = (uint16_t)tmp;
        break;
      }
      case 0x18: //2
      {
        uint32_t tmp = Top*2/10;
        val = (uint16_t)tmp;
        break;
      }
      case 0x5E: //3
      {
        uint32_t tmp = Top*3/10;
        val = (uint16_t)tmp;
        break;
      }
      case 0x08: //4
      {
        uint32_t tmp = Top*4/10;
        val = (uint16_t)tmp;
        break;
      }
      case 0x1C: //5
      {
        uint32_t tmp = Top*5/10;
        val = (uint16_t)tmp;
        break;
      }
      case 0x5A: //6
      {
        uint32_t tmp = Top*6/10;
        val = (uint16_t)tmp;
        break;
      }
      case 0x42: //7
      {
        uint32_t tmp = Top*7/10;
        val = (uint16_t)tmp;
        break;
      }
      case 0x52: //8
      {
        uint32_t tmp = Top*8/10;
        val = (uint16_t)tmp;
        break;
      }
      case 0x4A: //9
      {
        uint32_t tmp = Top*9/10;
        val = (uint16_t)tmp;
        break;
      }
      case 0x09: //(EQ)save
      {
        if(val != eepromval)
        {
          eepromval = val;
          eeprom_write_word(0,eepromval);
        }
      }
    }
    
    if(val==0)
    {
      //OCROFF;
      OCR1A = 0;
      SetClockLow();
    }
    else
    {
      SetClockHigh();
      OCR1A = val;
      //OCRON;
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
  while(IRPIN);//等待低电平 下降沿
  SetClockHigh();
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