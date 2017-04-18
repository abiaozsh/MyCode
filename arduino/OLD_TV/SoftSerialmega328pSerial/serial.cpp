#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

uint8_t data1[32];
uint8_t data2[32];

uint8_t* buff1;
uint8_t* buff2;
uint8_t buffidx = 0;

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

//时钟信号输入： 10(B2)
//10 PB2
//	SS (SPI Bus Master Slave select)
//	OC1B (Timer/Counter1 Output Compare Match B Output)
//	PCINT2 (Pin Change Interrupt 2)

//同步按钮 2(D2)
//2  PD2
//	INT0 (External Interrupt 0 Input)
//	PCINT18 (Pin Change Interrupt 18)

//同步模拟输入 A0
//A0 PC0
//	ADC0 (ADC Input Channel 0)
//	PCINT8 (Pin Change Interrupt 8)

//输出 D7

int main(void) {
  //初始化定时器 1/8
  TCCR1A = 0;
  TCCR1B = 1|_BV(WGM12);//0 1 0 clkI/O/8 (From prescaler)  //  1/8	2MHz 0.5us
  TCNT1 = 0;
  OCR1A = 2083;
  TIMSK1 |= _BV(OCIE1A);//TOIE1
  
  ADMUX = _BV(REFS0) | 0 | _BV(ADLAR);
  ADCSRA = _BV(ADEN)|_BV(ADATE)|_BV(ADSC)|_BV(ADIE)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0);
  
  //打开输出端口
  DDRD |= _BV(7);

  DDRB = 0;
  PCICR |= _BV(PCIE0);
  PCICR |= _BV(PCIE2);
  PCMSK0 |= _BV(PCINT2);
  PCMSK2 |= _BV(PCINT18);
  PORTB |= _BV(7);//PD7 上拉
  
  buff1 = data1;
  buff2 = data2;
  
  UCSR0A = _BV(U2X0);//U2Xn=1
  UCSR0B = _BV(RXCIE0) | _BV(RXEN0);
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
  UBRR0 = 7;//250000
  
  
  sei();
  //主循环
  for(;;) 
  {
  }

}

volatile uint8_t rowCount = 0;
volatile uint8_t colCount = 0;
uint8_t bitExt[] = {1,2,4,8,16,32,64,128};
volatile uint8_t value1 = 0;
//TIMER1_OVF_vect  TIMER1_COMPA_vect
ISR(TIMER1_COMPA_vect){
  PORTD &= ~_BV(7);
  PORTD |= value1;
  
  rowCount++;
  if(rowCount==8){
    rowCount = 0;
    colCount++;
    if(colCount==32)
    {
      value1 = 0;
    }
  }
  
  uint8_t v = buff1[colCount];
  v = v & bitExt[rowCount];
  if(v){
    value1 = _BV(7);
  }
  else
  {
    value1 = 0;
  }
}

volatile uint8_t rpmCount = 0;
//时钟输入
ISR(PCINT0_vect){
  rpmCount++;
  if(rpmCount==36){
    rpmCount = 0;
    rowCount = 0;
    colCount = 0;

    //新帧开始
    uint8_t* tmp = buff1;
    buff1 = buff2;
    buff2 = tmp;
  }
}

//按钮输入
ISR(PCINT2_vect){
  if(PIND & _BV(2)){
  }
}

uint8_t adcOldval = 0;
ISR(ADC_vect){
  uint8_t tmp = ADCH>>2;
  if(tmp>adcOldval){
    rpmCount--;
  }
  if(tmp<adcOldval){
    rpmCount++;
  }
  adcOldval = tmp;
}
 

ISR(USART_RX_vect){
  uint8_t val = UDR0;

  if(buffidx==32)
  {
    buffidx=0;
  }
  buff2[buffidx] = val;
  
  buffidx++;
}
