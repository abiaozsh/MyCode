#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


//1000000 / 9600 = 104.16666666666666666666666666667
//1000000 / 9600 = 156.25
#define TCCR0_Value_9600 2
#define CNT_Value_9600 104
#define CNT_1_5_9600 156


#define TCCR0_Value TCCR0_Value_9600
#define CNT_Value CNT_Value_9600
#define CNT_1_5 CNT_1_5_9600


//pd0 rxd		usb txd
//pd1 txd		usb rxd
#define DDR_Send DDRD
#define PORT_Send PORTD
#define BIT_Send _BV(1)
#define DDR_Recv DDRD
#define PIN_Recv PIND
#define BIT_Recv _BV(0)


void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}
void TimerInit()
{
  // COM1A1 COM1A0 COM1B1 COM1B0 – – WGM11 WGM10
//CS02 CS01 CS00 Description
//0   0 0 0 No clock source (Timer/Counter stopped)
//1 8Mhz      0 0 1 clkI/O/(No prescaling)
//2 1Mhz      0 1 0 clkI/O/8 (From prescaler)
//3 1/8Mhz    0 1 1 clkI/O/64 (From prescaler)
//4 1/32Mhz   1 0 0 clkI/O/256 (From prescaler)
//5 1/128Mhz  1 0 1 clkI/O/1024 (From prescaler)
  TCCR0 = TCCR0_Value;

}
void SerialSend(uint8_t val){
	cli();
	TCCR0 = TCCR0_Value;
	TCNT0 = 0;
	PORT_Send &= ~BIT_Send;
  while(TCNT0<CNT_Value);TCNT0 = 0;//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit)
    {
      PORT_Send |= BIT_Send;
    }else{
      PORT_Send &= ~BIT_Send;
    }
    chkbit<<=1;
    while(TCNT0<CNT_Value);TCNT0 = 0;
	}
	PORT_Send |= BIT_Send;
  while(TCNT0<CNT_Value);TCNT0 = 0;//stopbit
	sei();
}


/*
11.11 V
1.111 V
1111. mV
111.1 mV

11.11 A
1.111 A
111.1 mA
1111 uA
*/

volatile uint8_t _digi0;
volatile uint8_t _digi0Dot;
volatile uint8_t _digi1;
volatile uint8_t _digi1Dot;
volatile uint8_t _digi2;
volatile uint8_t _digi2Dot;
volatile uint8_t _digi3;
volatile uint8_t _digi3Dot;
volatile uint8_t _sign;

#define EN   (PINC & _BV(0))
#define DATA (PINC & _BV(1))
#define CLK  (PINC & _BV(2))
//PC3 4 5 衰减等级

//默认高电平
#define HIGH (PINC & _BV(5))
#define MID  (PINC & _BV(4))
#define LOW  (PINC & _BV(3))

//L:G1 H:G20
#define G20  (PINB & _BV(5))

//电压/电流 通过数据口获得

void wait(uint8_t ticks)
{
	TCNT0 = 0;//timer reset
	while(TCNT0<ticks);
}
volatile uint8_t dummy;

int main()
{
	TCCR1A = 0;
	TCCR1B = 3;
	TCNT1 = 0;
  TIMSK |= _BV(OCIE1A);
  OCR1A = 100;
  SerialInit();
  TimerInit();

  sei();

//      A11-B2  
//F10-B3      B07-D2
//      G05-B7
//E01-B0      C04-D5
//      D02-D7      DP03-D6
  DDRB |= _BV(2);//A11
  DDRD |= _BV(2);//B07
  DDRD |= _BV(5);//C04
  DDRD |= _BV(7);//D02
  DDRB |= _BV(0);//E01
  DDRB |= _BV(3);//F10
  DDRB |= _BV(7);//G05
  DDRD |= _BV(6);//DP03

  DDRB |= _BV(1);//digit1
  DDRD |= _BV(4);//digit2
  DDRD |= _BV(3);//digit3
  DDRB |= _BV(6);//digit4
  DDRB |= _BV(4);//sign
  
  uint8_t casev = 0;
	while (true) {
      int32_t val_req = 0;
      int32_t mask = 1;
	  
	  uint8_t V_A = 0;//0V 1A 第31bit
	  uint8_t sign = 0;//最高位符号，其余位为0   第32bit

    while(EN);
      while(!EN);
	  //	  V_A
	  //	  _sign
    
      for(uint8_t i=0;i<32;i++){
        while(!CLK && EN);
        if(DATA){
          val_req |= mask;
        }
        while(CLK && EN);
        mask<<=1;
      }
      
      sign = (val_req & 0x80000000) != 0;
      V_A =  (val_req & 0x40000000) != 0;
      val_req &= 0x00FFFFFF;
      
      
      float val = (float)val_req; //max +- 512k / 1.1v

		uint8_t digi0=0;
		uint8_t digi1=0;
		uint8_t digi2=0;
		uint8_t digi3=0;
		uint8_t digi0Dot=0;
		uint8_t digi1Dot=0;
		uint8_t digi2Dot=0;
		uint8_t digi3Dot=0;
		
      //a6 high gain low gain
      //a7 V/A 轮流监测
//      if(PINA|=_BV(7)){//V
        // a3-a2 Vin    0x20 01 0000*1  0x21 01 0001*20
//        if(PINA|=_BV(6)){//high gain 20x
        
//        }else{//low gain 1x

          val = val / (512.0*1024.0) * 1.1; //v
          
          //a1 1/50  max 55V  max 1.1V
          if(false){//PINA|=_BV(1)
            val = val * (50.0);
          }
          if(val>10){
          //xx.xx
            digi1Dot = 1;
            val = val * 0.1;
          }else if(val>1){
          //x.xxx
            digi0Dot = 1;
          }else{
          //xxxx
            val = val * 10;
          }
          digi0 = (uint8_t)val;
          val -= digi0;
          val = val * 10;
          digi1 = (uint8_t)val;
          val -= digi1;
          val = val * 10;
          digi2 = (uint8_t)val;
          val -= digi2;
          val = val * 10;
          digi3 = (uint8_t)val;
          
      if(sign){
        SerialSend('-');  
		  }else{
        SerialSend('+');
		  }
      SerialSend('0'+digi0);
		  if(digi0Dot){
			  SerialSend('.');
		  }
      SerialSend('0'+digi1);
		  if(digi1Dot){
			  SerialSend('.');
		  }
		  SerialSend('0'+digi2);
		  if(digi2Dot){
			  SerialSend('.');
		  }
		  SerialSend('0'+digi3);
		  if(digi3Dot){
			  SerialSend('.');
		  }

		  SerialSend('\r');
		  SerialSend('\n');
          
//        }
//      }else{//A
//        // a3-a4 Ain    0x32 11 0010*1  0x33 11 0011*20
//        if(PINA|=_BV(6)){//high gain 20x
//        
//        }else{//low gain 1x
//          
//        }
//      }

    _digi0    =digi0;
		_digi0Dot =digi0Dot;
		_digi1    =digi1;
		_digi1Dot =digi1Dot;
		_digi2    =digi2;
		_digi2Dot =digi2Dot;
		_digi3    =digi3;
		_digi3Dot =digi3Dot;
		_sign     =sign;


      //段位转换
      
    }

    
		//for(uint16_t i=0;i<100;i++)
		//{
		//	wait(100);//1000ms
		//}
	
        
	return 0;
}

volatile uint8_t cur_digit = 0;

ISR(TIMER1_COMPA_vect){
TCNT1=0;
  PORTB |= _BV(2);//A11
  PORTD |= _BV(2);//B07
  PORTD |= _BV(5);//C04
  PORTD |= _BV(7);//D02
  PORTB |= _BV(0);//E01
  PORTB |= _BV(3);//F10
  PORTB |= _BV(7);//G05
  PORTD |= _BV(6);//DP03

  PORTB&=~_BV(1);//digit1
  PORTD&=~_BV(4);//digit2
  PORTD&=~_BV(3);//digit3
  PORTB&=~_BV(6);//digit4
  
  PORTB |= _BV(4);//sign
  
  uint8_t cur_digit_val = 0;
  uint8_t cur_digit_dot = 0;
  if(cur_digit==0){
	PORTB |= _BV(1);
	cur_digit_val = _digi0;
	cur_digit_dot = _digi0Dot;
  }
  else if(cur_digit==1){
	PORTD |= _BV(4);
	cur_digit_val = _digi1;
	cur_digit_dot = _digi1Dot;
  }
  else if(cur_digit==2){
    PORTD |= _BV(3);
	cur_digit_val = _digi2;
	cur_digit_dot = _digi2Dot;
  }
  else if(cur_digit==3){
    PORTB |= _BV(6);
	cur_digit_val = _digi3;
	cur_digit_dot = _digi3Dot;
  }
  
  if(_sign){
    PORTB&=~_BV(4);
  }
  
  uint8_t segA=0;
  uint8_t segB=0;
  uint8_t segC=0;
  uint8_t segD=0;
  uint8_t segE=0;
  uint8_t segF=0;
  uint8_t segG=0;

//      A11-B2  
//F10-B3      B07-D2
//      G05-B7
//E01-B0      C04-D5
//      D02-D7      DP03-D6
  if(cur_digit_val==0){
	  segA=1;segB=1;segC=1;segD=1;segE=1;segF=1;
  }
  if(cur_digit_val==1){
	  segB=1;segC=1;
  }
  if(cur_digit_val==2){
	  segA=1;segB=1;segG=1;segE=1;segD=1;
  }
  if(cur_digit_val==3){
	  segA=1;segB=1;segG=1;segC=1;segD=1;
  }
  if(cur_digit_val==4){
	  segF=1;segG=1;segB=1;segC=1;
  }
  if(cur_digit_val==5){
	  segA=1;segF=1;segG=1;segC=1;segD=1;
  }
  if(cur_digit_val==6){
	  segA=1;segF=1;segG=1;segC=1;segD=1;segE=1;
  }
  if(cur_digit_val==7){
	  segA=1;segB=1;segC=1;
  }
  if(cur_digit_val==8){
	  segA=1;segB=1;segC=1;segD=1;segE=1;segF=1;segG=1;
  }
  if(cur_digit_val==9){
	  segA=1;segB=1;segC=1;segD=1;segF=1;segG=1;
  }

  if(segA){
	PORTB &= ~_BV(2);//A11
  }
  if(segB){
	PORTD &= ~_BV(2);//B07
  }
  if(segC){
	PORTD &= ~_BV(5);//C04
  }
  if(segD){
	PORTD &= ~_BV(7);//D02
  }
  if(segE){
	PORTB &= ~_BV(0);//E01
  }
  if(segF){
	PORTB &= ~_BV(3);//F10
  }
  if(segG){
	PORTB &= ~_BV(7);//G05
  }

  if(cur_digit_dot){
	PORTD &= ~_BV(6);//DP03
  }
  
  cur_digit++;
  if(cur_digit==4){
    cur_digit=0;
  }
}
