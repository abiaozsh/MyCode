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
	CLKPR = 1;//1/1 //8MHz
	//0 0 0 0 0 1
	//0 0 0 1 1 2
	//0 0 1 0 2 4
	//0 0 1 1 3 8
	//0 1 0 0 4 16
	//0 1 0 1 5 32
	//0 1 1 0 6 64
	//0 1 1 1 7 128
	//1 0 0 0 8 256 
	sei();
	
	
	DDRA = 255;
	PORTA = 0;
	DDRB = 15;
	PORTB = 0;
	PORTB |= _BV(3);
	//DDRA &= ~ _BV(7);
	DDRA &= ~ _BV(5);
	
	DDRA &= ~ _BV(4);
	DIDR0 &= ~ _BV(4);
	ADMUX = 4;
	ADCSRB |= _BV(ADLAR);

	ADCSRA = 7;
	ADCSRA |= _BV(ADIE);
	ADCSRA |= _BV(ADATE);
	ADCSRA |= _BV(ADEN);
	ADCSRA |= _BV(ADSC);
	
	//上OC0A B2 1.3/5*256 = 67
	//下OC0B A7 3.7/5*256 = 189
	
	OCR0A = 128;
	OCR0B = 128;
	TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1) | _BV(WGM00);
	TCCR0B = 1;
	// _BV(WGM13) | _BV(WGM12) | _BV(WGM11) | _BV(WGM10)
	// WGM0  210
    //       001 PWM, Phase Correct 0xFF TOP BOTTOM	
	
//CS02 CS01 CS00 Description
//0 0 0 No clock source (Timer/Counter stopped)
//1 0 0 1 clkI/O/(No prescaling)
//2 0 1 0 clkI/O/8 (From prescaler)
//3 0 1 1 clkI/O/64 (From prescaler)
//4 1 0 0 clkI/O/256 (From prescaler)
//5 1 0 1 clkI/O/1024 (From prescaler)
 	//DDRB &= ~ _BV(2);
	//DDRA &= ~ _BV(7);

	while(1){
	
//	//TCCR0A |= (_BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1));
// 	DDRB &= ~ _BV(2);
//	DDRA &= ~ _BV(7);
//	
//	while(PINA & _BV(5)){}
//  
//	//TCCR0A &= ~(_BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1));
// 	DDRB |= _BV(2);
//	DDRA |= _BV(7);
//
//	while(!(PINA & _BV(5))){}
  }
  
  
//  //初始化输入
//  GIMSK |= _BV(PCIE0);
//  PCMSK0 |= _BV(PCINT6);//CLK
//  sei();

  //主循环
  for(;;) 
  {
	
  }
}
ISR(ADC_vect){
	uint8_t val = ADCH;
	OCR0A = val;
	OCR0B = val;
}
ISR(TIM1_COMPA_vect){
 // PORT6O = PWR_OFF[Step];PWROff;//CmdPWROff;
}

ISR(PCINT0_vect){//先送高，后送低
}
