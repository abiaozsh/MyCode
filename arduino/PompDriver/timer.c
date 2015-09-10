#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

#define LONGON  PORTB |= _BV(0)
#define LONGOFF PORTB &=~_BV(0)
#define AUTOON  PORTB |= _BV(1)
#define AUTOOFF PORTB &=~_BV(1)

int main(void)
{
	CLKPR = _BV(CLKPCE);//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 0;//8Mhz
	CLKPR = _BV(CLKPS3);//1 0 0 0 1/256  31.25khz
//8Mhz = 0.000000125 s
//0.000032
//0.032768s/tick
  TCCR1A = 0;
  //0.032768s/tick
  //total 2147.483648s
  TCCR1B = 5;//  1 0 1 clkI/O/1024 (From prescaler)
  TCCR1C = 0;
  TIMSK1 = 0;

  DDRA = 0;
  DDRB = 3;
  
  TCNT1 = 0;TIFR1 |= _BV(TOV1);
  
  uint8_t status = 0;//off
  
  uint8_t lastVal = PINA&0x1E;
  for (;;) {
    uint8_t val = PINA&0x1E;
    
    if(val!=lastVal)
    {
      if(currTick>10)//0.3s 单击
      {
        if(status==0)
        {
          status = 1;
          AUTOON;
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
      }
      TCNT1 = 0;TIFR1 |= _BV(TOV1);
      lastVal = val;
      
    }
  }
}
