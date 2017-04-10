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
  TCNT0 = 0;
  TIMSK0 = _BV(TOIE0);
  
  
  
  //打开输出端口
  DDRA |= _BV(0);
  
  
data[0] = 224;data[1] = 7;
data[2] = 16;data[3] = 8;
data[4] = 12;data[5] = 48;
data[6] = 4;data[7] = 32;
data[8] = 2;data[9] = 64;
data[10] = 1;data[11] = 64;
data[12] = 1;data[13] = 128;
data[14] = 1;data[15] = 128;
data[16] = 1;data[17] = 128;
data[18] = 1;data[19] = 128;
data[20] = 1;data[21] = 64;
data[22] = 2;data[23] = 64;
data[24] = 4;data[25] = 32;
data[26] = 12;data[27] = 48;
data[28] = 48;data[29] = 12;
data[30] = 192;data[31] = 3;
 /*
data[0] = 1;data[1] = 129;
data[2] = 2;data[3] = 66;
data[4] = 4;data[5] = 36;
data[6] = 8;data[7] = 24;
data[8] = 16;data[9] = 24;
data[10] = 32;data[11] = 36;
data[12] = 64;data[13] = 66;
data[14] = 129;data[15] = 129;
data[16] = 130;data[17] = 1;
data[18] = 68;data[19] = 2;
data[20] = 40;data[21] = 4;
data[22] = 16;data[23] = 8;
data[24] = 40;data[25] = 16;
data[26] = 68;data[27] = 32;
data[28] = 130;data[29] = 64;
data[30] = 1;data[31] = 129;
  */
  sei();
  //主循环
  for(;;) 
  {
  }
}

volatile uint8_t rowCount = 0;
volatile uint8_t colCount = 0;
uint8_t bitExt[] = {1,2,4,8,16,32,64,128};
uint8_t data[32];
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
    }
  }
  
  uint8_t v = data[colCount];
  v = v & bitExt[rowCount];
  if(v){
    value = _BV(0);
  }
  else
  {
    value = 0;
  }
}
