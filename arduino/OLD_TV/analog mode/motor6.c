#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

int main(void) {
  //初始化时钟：1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  //初始化定时器 1/8
  TCCR0A = 0;//  1/8	1MHz 1us
  TCCR0B = 2;//0 1 0 clkI/O/8 (From prescaler)
  TIMSK0 = _BV(TOIE0);
  
  //打开输出端口
  DDRA |= _BV(0);
  
  sei();
  //主循环
  for(;;) 
  {
  }
}

ISR(TIM0_OVF_vect){
	PINA |= _BV(0);
}
