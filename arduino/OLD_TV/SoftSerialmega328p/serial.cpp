#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

PROGMEM prog_uint8_t data[] = {
224,7,16,8,12,48,4,32,2,64,1,64,1,128,1,128,1,128,1,128,1,64,2,64,4,32,12,48,48,12,192,3,
1,129,2,66,4,36,8,24,16,24,32,36,64,66,129,129,130,1,68,2,40,4,16,8,40,16,68,32,130,64,1,129,
};

//target:30fps
//30*16*16=dps
//7680*n = 2000000Mhz
//260.41666666666666666666666666667

//fps*16*16*n = 2000000hz
//n = 2000000/fps/256
//n = 2000000/30/256  260.41666666666666666666666666667


//fps*16*16*n = 250000hz
//n = 250000/fps/256
//n = 250000/30/256 32.552083333333333333333333333333

//2000000 / n / 256 = 

//fps*16*16*n = 16000000hz
//n = 16000000/fps/256
//n = 16000000/30/256 2083.3333333333333333333333333333

//2000000 / n / 256 = 


int main(void) {
  //初始化定时器 1/8
  TCCR1A = 0;
  TCCR1B = 1|_BV(WGM12);//0 1 0 clkI/O/8 (From prescaler)  //  1/8	2MHz 0.5us
  TCNT1 = 0;
  OCR1A = 2083;
  TIMSK1 |= _BV(OCIE1A);//TOIE1
  
  //打开输出端口
  DDRB |= _BV(0);
  DDRB |= _BV(1);
  
  DDRD = 0;
  PCICR |= _BV(PCIE2);
  PCMSK2 |= _BV(PCINT23);
  PORTD |= _BV(7);//PD7 上拉
  
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
volatile uint8_t value1 = 0;
volatile uint8_t value2 = 0;
//TIMER1_OVF_vect  TIMER1_COMPA_vect
ISR(TIMER1_COMPA_vect){
  PORTB = value1|value2;
  
  rowCount++;
  if(rowCount==8){
    rowCount = 0;
    colCount++;
    if(colCount==32)
    {
      value1 = 0;
      return;
//      colCount = 0;
//      totalidx+=32;
//      if(totalidx==64)
//      {
//        totalidx=0;
//      }
    }
  }
  
  uint8_t v = pgm_read_byte_near(data+totalidx+colCount);
  v = v & bitExt[rowCount];
  if(v){
    value1 = _BV(0);
  }
  else
  {
    value1 = 0;
  }
}

uint8_t rpmCount = 0;
ISR(PCINT2_vect){
  rpmCount++;
  if(rpmCount==36){
    value2 ^= 2;
    rpmCount = 0;
    totalidx=0;
    //if(totalidx==2)
    rowCount = 0;
    colCount = 0;

    
  }
}