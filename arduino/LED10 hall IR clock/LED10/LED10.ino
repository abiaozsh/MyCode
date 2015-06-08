#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define currTick0 ((TIFR0 & _BV(TOV0))?0x0FF:TCNT0)
#define currTick1 ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)
#define currTick3 TCNT3
#define ResetT0 TCNT0 = 0;TIFR0 |= _BV(TOV0);
#define ResetT1 TCNT1 = 0;TIFR1 |= _BV(TOV1);

byte days = 0;
byte hour = 0;
byte miniute = 0;
byte second = 0;
unsigned int tick = 0;//15625 tick/s

byte figure0;//days
byte figure1;//hours
byte figure2;
byte figure3;
byte figure4;
byte figure5;
byte figure6;
byte dimeSeconds=0;

uint8_t BITS10[10];
volatile uint8_t* PORTS10[10];
volatile uint8_t* DDRS10[10];
volatile uint8_t* PINS10[10];

void initArray()
{
	unsigned int i;
	i = 0;
	BITS10[i] = _BV(1);PORTS10[i] = &PORTD;DDRS10[i] = &DDRD;PINS10[i] = &PIND;i++;//2, D1
	BITS10[i] = _BV(0);PORTS10[i] = &PORTD;DDRS10[i] = &DDRD;PINS10[i] = &PIND;i++;//3, D0
	BITS10[i] = _BV(4);PORTS10[i] = &PORTD;DDRS10[i] = &DDRD;PINS10[i] = &PIND;i++;//4, D4
	BITS10[i] = _BV(6);PORTS10[i] = &PORTC;DDRS10[i] = &DDRC;PINS10[i] = &PINC;i++;//5, C6
	BITS10[i] = _BV(7);PORTS10[i] = &PORTD;DDRS10[i] = &DDRD;PINS10[i] = &PIND;i++;//6, D7
	BITS10[i] = _BV(6);PORTS10[i] = &PORTE;DDRS10[i] = &DDRE;PINS10[i] = &PINE;i++;//7, E6
	BITS10[i] = _BV(4);PORTS10[i] = &PORTB;DDRS10[i] = &DDRB;PINS10[i] = &PINB;i++;//8, B4
	BITS10[i] = _BV(5);PORTS10[i] = &PORTB;DDRS10[i] = &DDRB;PINS10[i] = &PINB;i++;//9, B5
	BITS10[i] = _BV(6);PORTS10[i] = &PORTB;DDRS10[i] = &DDRB;PINS10[i] = &PINB;i++;//10,B6
	BITS10[i] = _BV(2);PORTS10[i] = &PORTB;DDRS10[i] = &DDRB;PINS10[i] = &PINB;i++;//16 B2
}

void rstPin()
{
	DDRD = 0;
	DDRC = 0;
	DDRE = 0;
	DDRB = 0;
	DDRF = 0;
	PORTD = 0;
	PORTC = 0;
	PORTE = 0;
	PORTB = 0;
	PORTF = 0;
}

void initDDR()
{
  for(uint8_t i=0;i<10;i++)  
  {
    *DDRS10[i]  |= BITS10[i];
  }
  DDRF = _BV(4) | _BV(5) | _BV(6);
}

//TCNT0
void Timer0Init()
{
	TCCR0A = 0;
	TCCR0B = 0;
	//CS02 CS01 CS00 Description
	//0 0 0 No clock source (Timer/Counter stopped)
	//0 0 1 clkI/O/(No prescaling)
	//0 1 0 clkI/O/8 (From prescaler)
	//0 1 1 clkI/O/64 (From prescaler)
	//1 0 0 clkI/O/256 (From prescaler)
	//1 0 1 clkI/O/1024 (From prescaler)
	//1 1 0 External clock source on T0 pin. Clock on falling edge.
	//1 1 1 External clock source on T0 pin. Clock on rising edge.
	TCCR0B = 2; // 1/8
	//TCCR0B = 3; // 1/64
	//TCCR0B = 4; // 1/256
	TIMSK0 = 0;
	TCNT0 = 0;TIFR0 |= _BV(TOV0);//timer reset //overflow flg reset
}

//TCNT0
void Timer1Init()
{
	TCCR1A = 0;
	TCCR1B = 0;
	//CS02 CS01 CS00 Description
	//0 0 0 No clock source (Timer/Counter stopped)
	//1 0 0 1 clkI/O/(No prescaling)
	//2 0 1 0 clkI/O/8 (From prescaler)
	//3 0 1 1 clkI/O/64 (From prescaler)
	//4 1 0 0 clkI/O/256 (From prescaler)
	//5 1 0 1 clkI/O/1024 (From prescaler)
	//1 1 0 External clock source on T0 pin. Clock on falling edge.
	//1 1 1 External clock source on T0 pin. Clock on rising edge.
	//TCCR1B = 2; // 1/8
	TCCR1B = 3;
	TIMSK1 = 0;
	TCNT1 = 0;TIFR1 |= _BV(TOV1);//timer reset //overflow flg reset
}

//1tick = 1/15625s
//15625tick/s
void Timer3Init()
{
	TCCR3A = 0;
	TCCR3B = 0;
	//CS02 CS01 CS00 Description
	//0 0 0 No clock source (Timer/Counter stopped)
	//0 0 1 clkI/O/(No prescaling)
	//0 1 0 clkI/O/8 (From prescaler)
	//0 1 1 clkI/O/64 (From prescaler)
	//1 0 0 clkI/O/256 (From prescaler)
	//1 0 1 clkI/O/1024 (From prescaler)
	//1 1 0 External clock source on T0 pin. Clock on falling edge.
	//1 1 1 External clock source on T0 pin. Clock on rising edge.
	TCCR3B = 5; // 1/1024
	TIMSK3 = 0;
}

