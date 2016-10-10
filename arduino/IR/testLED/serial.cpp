#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define IRPIN (PINB & _BV(4))
#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

                  //     2      3      4      5      6      7      8      9     10     11
uint8_t indexD[] = {_BV(2),_BV(3),_BV(4),_BV(5),_BV(6),_BV(7),     0,     0,     0,     0};
uint8_t indexB[] = {     0,     0,     0,     0,     0,     0,_BV(0),_BV(1),_BV(2),_BV(3)};
uint8_t indexC[] = {_BV(0),_BV(1),_BV(2)};

uint8_t index3 = 0;
uint8_t index30 = 0;
uint8_t index10 = 0;

volatile uint8_t dataC;

volatile uint8_t buff[30];

//占用timer0
/*
45 46 47
44 40 43
07 15 09
16 19 0D
0C 18 5E
08 1C 5A
42 52 4A
*/
uint8_t GetIR() {
  uint8_t data = 0;
  uint8_t datahi = 0;
  uint8_t dataidx = 0;
  uint8_t byteidx = 0;
  TCCR0A = 0;
  TCCR0B = 4;//1 0 0 clkI/O/256 (From prescaler)
  while(IRPIN);//等待低电平 下降沿
  DDRB|=_BV(5);
  PORTB|=_BV(5);
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
        if(TCNT0<70)
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
  DDRB&=~_BV(5);
  PORTB&=~_BV(5);
  if(data!=datahi)
  {
    return 0xFF;
  }
  return data;
}

int main(void) {
  DDRD = _BV(2)|_BV(3)|_BV(4)|_BV(5)|_BV(6)|_BV(7);
  DDRB = _BV(0)|_BV(1)|_BV(2)|_BV(3);
  DDRC = _BV(0)|_BV(1)|_BV(2);
  
  //TCCR1A = 0;// | _BV(WGM11) | _BV(WGM10);// ; 
  //TCCR1B = 1;//0 0 1 clkI/O/1 (No prescaling)    16000000 / 8 / 512 / 30
  //2 0 0 1 0 PWM, Phase Correct, 9-bit 0x01FF TOP BOTTOM
  //WGM11
  
  
  TCCR1A = _BV(WGM11);
  TCCR1B = 1 | _BV(WGM13) | _BV(WGM12);
  ICR1 = 0x500;
  
  TIMSK1 = _BV(TOIE1) | _BV(OCIE1A) | _BV(OCIE1B);
  TCNT1 = 0;

  sei();
  
  {
    buff[ 0] = 255;
    buff[10] = 255;
    buff[20] = 255;

    buff[ 1] = 0;
    buff[11] = 0;
    buff[21] = 0;
    
    buff[ 2] = 0;
    buff[12] = 0;
    buff[22] = 0;
    
    buff[ 3] = 0;
    buff[13] = 0;
    buff[23] = 0;
    
    buff[ 4] = 0;
    buff[14] = 0;
    buff[24] = 0;
    
    buff[ 5] = 0;
    buff[15] = 0;
    buff[25] = 0;

    buff[ 6] = 0;
    buff[16] = 0;
    buff[26] = 0;
    
    buff[ 7] = 0;
    buff[17] = 0;
    buff[27] = 0;
    
    buff[ 8] = 0;
    buff[18] = 0;
    buff[28] = 0;
    
    buff[ 9] = 0;
    buff[19] = 0;
    buff[29] = 0;

  }
  
  uint8_t idx = 0;
  uint8_t color = 0;
  while(true){
    uint8_t val = GetIR();
    if(val==0)continue;
    
    uint8_t n = 0;
    
    if(val==0x45)
    {
      n = 0;
    }
    if(val==0x46)
    {
      n = 1;
    }
    if(val==0x47)
    {
      n = 2;
    }
    if(val==0x44)
    {
      n = 64;
    }
    if(val==0x40)
    {
      n = 128;
    }
    if(val==0x43)
    {
      n = 255;
    }
    
    switch(val)
    {
      case 0x16:idx = 0;break;
      case 0x0C:idx = 1;break;
      case 0x18:idx = 2;break;
      case 0x5E:idx = 3;break;
      case 0x08:idx = 4;break;
      case 0x1C:idx = 5;break;
      case 0x5A:idx = 6;break;
      case 0x42:idx = 7;break;
      case 0x52:idx = 8;break;
      case 0x4A:idx = 9;break;
    }
    switch(val)
    {
      case 0x07:color =  0;break;
      case 0x15:color = 10;break;
      case 0x09:color = 20;break;
    }

    buff[idx+color] = n;
  }
}

ISR(TIMER1_OVF_vect){
  //off
  PORTD |=  _BV(2)|_BV(3)|_BV(4)|_BV(5)|_BV(6)|_BV(7);
  PORTB |=  _BV(0)|_BV(1)|_BV(2)|_BV(3);
  //on
  PORTD &= ~indexD[index10];
  PORTB &= ~indexB[index10];
  dataC = indexC[index3];

  index3++;
  index30+=10;
  if(index3==3){
    index3 = 0;
    index30 = 0;
    index10++;
    if(index10==10){
      index10 = 0;
    }
  }
  //Update of OCR1x at BOTTOM
  OCR1AH = 3;
  OCR1AL = buff[index30+index10];
  OCR1BH = 2;
  OCR1BL = 0xFE;

}
//open
ISR(TIMER1_COMPB_vect){
  PORTC |=  dataC;
}
//close
ISR(TIMER1_COMPA_vect){
  PORTC &= ~dataC;
}




/* Interrupt Vectors */
/* Interrupt Vector 0 is the reset vector. */
//#define INT0_vect         _VECTOR(1)   /* External Interrupt Request 0 */
//#define INT1_vect         _VECTOR(2)   /* External Interrupt Request 1 */
//#define PCINT0_vect       _VECTOR(3)   /* Pin Change Interrupt Request 0 */
//#define PCINT1_vect       _VECTOR(4)   /* Pin Change Interrupt Request 0 */
//#define PCINT2_vect       _VECTOR(5)   /* Pin Change Interrupt Request 1 */
//#define WDT_vect          _VECTOR(6)   /* Watchdog Time-out Interrupt */
//#define TIMER2_COMPA_vect _VECTOR(7)   /* Timer/Counter2 Compare Match A */
//#define TIMER2_COMPB_vect _VECTOR(8)   /* Timer/Counter2 Compare Match A */
//#define TIMER2_OVF_vect   _VECTOR(9)   /* Timer/Counter2 Overflow */
//#define TIMER1_CAPT_vect  _VECTOR(10)  /* Timer/Counter1 Capture Event */
//#define TIMER1_COMPA_vect _VECTOR(11)  /* Timer/Counter1 Compare Match A */
//#define TIMER1_COMPB_vect _VECTOR(12)  /* Timer/Counter1 Compare Match B */ 
//#define TIMER1_OVF_vect   _VECTOR(13)  /* Timer/Counter1 Overflow */
//#define TIMER0_COMPA_vect _VECTOR(14)  /* TimerCounter0 Compare Match A */
//#define TIMER0_COMPB_vect _VECTOR(15)  /* TimerCounter0 Compare Match B */
//#define TIMER0_OVF_vect   _VECTOR(16)  /* Timer/Couner0 Overflow */
//#define SPI_STC_vect      _VECTOR(17)  /* SPI Serial Transfer Complete */
//#define USART_RX_vect     _VECTOR(18)  /* USART Rx Complete */
//#define USART_UDRE_vect   _VECTOR(19)  /* USART, Data Register Empty */
//#define USART_TX_vect     _VECTOR(20)  /* USART Tx Complete */
//#define ADC_vect          _VECTOR(21)  /* ADC Conversion Complete */
//#define EE_READY_vect     _VECTOR(22)  /* EEPROM Ready */
//#define ANALOG_COMP_vect  _VECTOR(23)  /* Analog Comparator */
//#define TWI_vect          _VECTOR(24)  /* Two-wire Serial Interface */
//#define SPM_READY_vect    _VECTOR(25)  /* Store Program Memory Read */
