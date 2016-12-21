#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

volatile uint8_t FStart = 0;

void adj();

void wait(unsigned int ticks)
{
	unsigned int i;
	for(i=0;i<ticks;i++)
	{
		asm volatile("nop");
	}
}

int main(void) {
	//初始化时钟：1MHz -> 8MHz
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	CLKPR = 6;//1/1 //8MHz
	//0 0 0 0 0 1
	//0 0 0 1 1 2
	//0 0 1 0 2 4
	//0 0 1 1 3 8
	//0 1 0 0 4 16
	//0 1 0 1 5 32
	//0 1 1 0 6 64
	//0 1 1 1 7 128
	//1 0 0 0 8 256 
	
	
	
	
	DDRA = 255;
	PORTA = 0;
	DDRB = 15;
	PORTB = 0;
	PORTB |= _BV(3);
	//DDRA &= ~ _BV(7);
	//DDRA &= ~ _BV(5);
	while(1){
	wait(10000);
	PORTA &= ~ _BV(5);
	wait(10000);
	PORTA |= _BV(5);
	
	}
	
	
	TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(COM0B0) | _BV(WGM00);
	TCCR0B = 1;
	// _BV(WGM13) | _BV(WGM12) | _BV(WGM11) | _BV(WGM10)
	// WGM0  210
    //       001 PWM, Phase Correct 0xFF TOP BOTTOM	
	OCR0A = 54;
	OCR0B = 54;
	
//CS02 CS01 CS00 Description
//0 0 0 No clock source (Timer/Counter stopped)
//0 0 1 clkI/O/(No prescaling)
//0 1 0 clkI/O/8 (From prescaler)
//0 1 1 clkI/O/64 (From prescaler)
//1 0 0 clkI/O/256 (From prescaler)
//1 0 1 clkI/O/1024 (From prescaler)

	while(1){
	
	TCCR0A |= (_BV(COM0A1) | _BV(COM0B1) | _BV(COM0B0));
	
	while(PINA & _BV(5)){}
  
  TCCR0A &= ~(_BV(COM0A1) | _BV(COM0B1) | _BV(COM0B0));
  
	while(!(PINA & _BV(5))){}
  }


	/*
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(COM1B0) | _BV(WGM11);// ;
	TCCR1B = _BV(WGM13) | _BV(WGM12) | 3;
	
//CS12 CS11 CS10 Description
//0 0 0 0 No clock source (Timer/Counter stopped).
//1 0 0 1 clkI/O/1 (No prescaling)
//2 0 1 0 clkI/O/8 (From prescaler)
//3 0 1 1 clkI/O/64 (From prescaler)
//4 1 0 0 clkI/O/256 (From prescaler)
//5 1 0 1 clkI/O/1024 (From prescaler)
	
	// _BV(WGM13) | _BV(WGM12) | _BV(WGM11) | _BV(WGM10)
	// WGM1 3  0
	//14    1110 Fast PWM ICR1 TOP TOP
	
	OCR1A = 120;
	OCR1B = 120;
	
	ICR1 = 1000;
	while(1){
	
	TCCR1A |= (_BV(COM1A1) | _BV(COM1B1) | _BV(COM1B0));
	
	while(PINA & _BV(7)){}
  
  TCCR1A &= ~(_BV(COM1A1) | _BV(COM1B1) | _BV(COM1B0));
  
	while(!(PINA & _BV(7))){}
  }
  */
  
  
//  //初始化输入
//  GIMSK |= _BV(PCIE0);
//  PCMSK0 |= _BV(PCINT6);//CLK
//  sei();

  //主循环
  for(;;) 
  {
	
  }
}

ISR(TIM1_COMPA_vect){
 // PORT6O = PWR_OFF[Step];PWROff;//CmdPWROff;
}

ISR(PCINT0_vect){//先送高，后送低
}
