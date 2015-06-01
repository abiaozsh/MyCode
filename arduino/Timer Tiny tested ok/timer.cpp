#include <avr/io.h>
volatile int temp;
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define dwPH PORTA |=  _BV(0);
#define dwPL PORTA &= ~_BV(0);
#define dwPT PINA  |=  _BV(0);
//Three I/O memory address locations are allocated for each port, one each for the Data Register
//– PORTx, Data Direction Register – DDRx, and the Port Input Pins – PINx. The Port Input Pins
//I/O location is read only, while the Data Register and the Data Direction Register are read/write.
//However, writing a logic one to a bit in the PINx Register, will result in a toggle in the corresponding
//bit in the Data Register. In addition, the Pull-up Disable – PUD bit in MCUCR disables the
//pull-up function for all pins in all ports when set.


#define drA3 (PINC & _BV(3))

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)


#define dwP13H PORTB |=  _BV(5);
#define dwP13L PORTB &= ~_BV(5);

void ClockInit()
{
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1
}

void TimerInit()
{
  // COM1A1 COM1A0 COM1B1 COM1B0 – – WGM11 WGM10
  TCCR1A = 0;
  //ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
  //0 0 1 clkI/O/1 (No prescaling)
  //0 1 0 clkI/O/8 (From prescaler)
  //0 1 1 clkI/O/64 (From prescaler)
  //1 0 0 clkI/O/256 (From prescaler)
  //1 0 1 clkI/O/1024 (From prescaler)

  //TCCR1B = 1;//  1/1
  TCCR1B = 2;//  1/8
  //TCCR1B = 3;//  1/64

  //FOC1A FOC1B – – – – – –
  TCCR1C = 0;
  //– – ICIE1 – – OCIE1B OCIE1A TOIE1
  TIMSK1 = 0;
  
}

void wait(unsigned int ticks)
{
	TCNT1 = 0;//timer reset
	TIFR1 |= _BV(TOV1);//overflow flg reset
	while(currTick<ticks)
	{
		;
	}
}

int main(void)
{
	ClockInit(); //do first
	TimerInit(); //do later

	DDRA |= _BV(0);//pMode(0, OUTPUT);    
	dwPH;
	
	for (;;) {
		dwPT;
		for(int i=0;i<1000;i++)
		{
			wait(100);
		}
		dwPT;
		for(int i=0;i<1000;i++)
		{
			wait(100);
		}
	}
        
	return 0;
}
