#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

PROGMEM prog_uint8_t data[] = {
224,7,
16,8,
12,48,
4,32,
2,64,
1,64,
1,128,
1,128,
1,128,
1,128,
1,64,
2,64,
4,32,
12,48,
48,12,
192,3,

1,129,
2,66,
4,36,
8,24,
16,24,
32,36,
64,66,
129,129,
130,1,
68,2,
40,4,
16,8,
40,16,
68,32,
130,64,
1,129,
};

int main(void) {
  //初始化时钟：1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  //初始化定时器 1/8
  TCCR0A = 0;//  1/8	1MHz 1us
  TCCR0B = 2;//0 1 0 clkI/O/8 (From prescaler)
  TCNT0 = 0;
  TIMSK0 = _BV(TOIE0);
  
  
  
  //打开输出端口
  DDRA |= _BV(0);
  
  
  sei();
  //主循环
  for(;;) 
  {
  }
}
volatile uint16_t totalidx = 0;
volatile uint8_t rowCount = 0;
volatile uint8_t colCount = 0;
uint8_t bitExt[] = {1,2,4,8,16,32,64,128};
volatile uint8_t value = 0;

ISR(TIM0_OVF_vect){
  PORTA = value;
  
  rowCount++;
  if(rowCount==8){
    rowCount = 0;
    colCount++;
    if(colCount==32)
    {
      colCount = 0;
	  totalidx+=32;
	  if(totalidx==64)
	  {
	  totalidx=0;
	  }
    }
  }
  
  uint8_t v = pgm_read_byte_near(data+totalidx+colCount);
  v = v & bitExt[rowCount];
  if(v){
    value = _BV(0);
  }
  else
  {
    value = 0;
  }
}
