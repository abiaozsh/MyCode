#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define blinkDDR DDRB |=  _BV(1);
#define blinkH PORTB |=  _BV(1);
#define blinkL PORTB &= ~_BV(1);

void TimerInit()
{
  // COM1A1 COM1A0 COM1B1 COM1B0 ¨C ¨C WGM11 WGM10
  TCCR1A = 0;
  //ICNC1 ICES1 ¨C WGM13 WGM12 CS12 CS11 CS10
  
  
  //0 0 1 clkI/O/1 (No prescaling)
  //0 1 0 clkI/O/8 (From prescaler)
  //0 1 1 clkI/O/64 (From prescaler)
  //1 0 0 clkI/O/256 (From prescaler)
  //1 0 1 clkI/O/1024 (From prescaler)
  //TCCR1B = 1;//  1/1
  TCCR1B = 2;//  1/8 //1Mhz
  //TCCR1B = 3;//  1/64

  
}

void wait(uint16_t ticks)
{
	TCNT1 = 0;//timer reset
	while(TCNT1<ticks)
	{
		;
	}
}

int main()
{
	TimerInit(); //do later

	blinkDDR;
	
	for (;;) {
		blinkH;
		for(uint16_t i=0;i<2000;i++)
		{
			wait(100);//1000ms
		}
		blinkL;
		for(uint16_t i=0;i<2000;i++)
		{
			wait(100);//1000ms
		}
	}
        
	return 0;
}
