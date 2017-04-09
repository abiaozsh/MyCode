#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

int main(void) {
  //��ʼ��ʱ�ӣ�1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  //��ʼ����ʱ�� 1/8
  TCCR0A = 0;//  1/8	1MHz 1us
  TCCR0B = 2;//0 1 0 clkI/O/8 (From prescaler)
  TIMSK0 = _BV(TOIE0);
  
  //������˿�
  DDRA |= _BV(0);
  
  sei();
  //��ѭ��
  for(;;) 
  {
  }
}

ISR(TIM0_OVF_vect){
	PINA |= _BV(0);
}
