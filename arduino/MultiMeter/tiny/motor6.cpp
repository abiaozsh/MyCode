#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//4+8 reset disabled
//
//a0 vref - cap to GND
// a3-a3 calib? 100100*1  100101*20
// a3-a2 Vin    110000*1  110001*20  010000 010001
// a3-a4 Ain    010010*1  010011*20   110010 110011


//a6 high gain low gain //TODO auto
//a7 A/V

//b0 clk
//b1 dat
//b2 en
//b3 free

void wait(uint16_t time){
	TCNT1 = 0;
	while(TCNT1<time);
	return;
}

volatile uint8_t send_req;
volatile int32_t val_req;

#define ENH PORTB |= _BV(2);PORTB &=~_BV(3);
#define ENL PORTB &=~_BV(2);PORTB |= _BV(3);

#define CLKH PORTB |= _BV(0)
#define CLKL PORTB &=~_BV(0)

#define DATAH PORTB |= _BV(1)
#define DATAL PORTB &=~_BV(1)

//L:G1 H:G20  PINA & _BV(7)
#define G20 0
//L:V H:A  PINA & _BV(6)
#define V_A 0
//L:normal H:20A   PINA & _BV(5)
#define A20 0

int main(void) {
  //初始化时钟：1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  
  
  //初始化定时器 1/8
  TCCR1B = 2;//  1/8	1MHz 1us

  
  
  DIDR0 |= _BV(2);
  DIDR0 |= _BV(3);
  DIDR0 |= _BV(4);
  
  ADCSRA |= _BV(ADEN);
  ADCSRA |= _BV(ADSC);
  ADCSRA |= _BV(ADATE);
  ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
  ADCSRA |= _BV(ADIE);
  
  ADCSRB |= _BV(BIN);
  ADCSRB |= _BV(ADLAR);

  //打开输出端口

  //初始化输出端口
  DDRB |= _BV(0);
  DDRB |= _BV(1);
  DDRB |= _BV(2);
  DDRB |= _BV(3);
  sei();

  
  //zeroCalib();//offset
  // calib? N3 P3  0x24 10 0100*1  0x25 10 0101*20
  //ADMUX = 0x80+0x25;//REFS1 Internal 1.1V voltage reference


  uint8_t ADMUXcurr = 0;
  
  ADMUX = 0x00+0x00;
  
  while(true){
    if(send_req){
      uint8_t v_a = V_A;
      
//      uint8_t ADMUXset;
//      if(v_a){
//        //A
//        //P1 N3 0x0E 00 1110*1 0x0F 00 1111*20
//        if(A20){
//          if(G20){
//            ADMUXset = 0x80+0x0E;
//          }else{
//            ADMUXset = 0x80+0x0F;
//          }
//        }else{
//          //P4 N3  0x32 11 0010*1   0x33 11 0011*20
//          if(G20){
//            ADMUXset = 0x80+0x33;
//          }else{
//            ADMUXset = 0x80+0x32;
//          }
//        }
//      }else{
//        //V
//        //P2 N3  0x10 01 0000*1  0x11 01 0001*20
//        if(G20){
//          ADMUXset = 0x80+0x11;
//        }else{
//          ADMUXset = 0x80+0x10;
//        }
//      }
//      
//      if(ADMUXset!=ADMUXcurr){
//        ADMUXcurr = ADMUXset;
//        ADMUX = ADMUXset;
//      }
      
      ENH;//EN
      uint8_t sign;
      uint32_t sendval;
      if(val_req<0){
        sendval = -val_req;
        sign = 1;
      }else{
        sendval = val_req;
        sign = 0;
      }
      if(sign){
        sendval|=0x80000000;
      }
      if(v_a){
        sendval|=0x40000000;
      }

      uint32_t mask = 1;
      for(uint8_t i=0;i<32;i++){
        
        if(sendval & mask){
          DATAH;
        }else{
          DATAL;
        }
        mask = mask << 1;
      
        wait(50);
        CLKH;
        wait(50);
        CLKL;
      }
      
      send_req = 0;
      ENL;//EN
    }
    ////////////////////
    //ADCSRA |= _BV(ADSC);//??
    //set_sleep_mode(SLEEP_MODE_ADC);
    //sleep_mode();
    /////////////////////
  }
}


uint16_t count;
int32_t val;

ISR(ADC_vect){
  count++;
  
  //if(ADC&0x200){
  //  val += int16_t(0xFC00 | ADC);//max +- 512
  //}else{
  //  val += ADC;
  //}
  val = ((int16_t)ADC)>>6;
  //val = ((uint16_t)ADC)>>6;
 
  //1024
  if(count>=256){//max +- 512k / 1.1v
    count=0;
    val_req=val;
    val=0;
    send_req = 1;
  }
}
