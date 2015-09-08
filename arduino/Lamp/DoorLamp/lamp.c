#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

void ClockInit();
//void TimerInit();


int main(void) {
	ClockInit();//��ʼ��ʱ�ӣ�1MHz -> 8MHz
	//TimerInit();//��ʼ����ʱ�� 1/8

	DDRA = 0;PORTA = 0;//all input
	DDRB = 0;PORTB = 0;//all input

	DDRA |= _BV(0);
	
	TCCR1A = 0;
	TCCR1B = 2;
	TIMSK1 = _BV(TOIE1);
	
//  TCCR0A = 0;
//  TCCR0B = 2;
//  TCNT0 = 0;
//  OCR0A = 128;//����Խ��Խ����match�Ժ�OE����ʱ����ʱ��OE��
//  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);
	sei();
	
	
	//��ѭ��
	while(1)
	{
		asm volatile("sleep");
	}
}

void ClockInit() {
	CLKPR = _BV(CLKPCE);//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = _BV(CLKPS3);//1/1 //8MHz
}

ISR(TIM1_OVF_vect){
	PORTA |= _BV(0);
	while(TCNT1<1000);
	PORTA &= ~_BV(0);
}
