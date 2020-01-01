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

PROGMEM prog_uint32_t num10s[] = {
1000000000,
100000000,
10000000,
1000000,
100000,
10000,
1000,
100,
10,
1,
};

void SendInt(uint32_t val)
{
	uint32_t num = val;
	for(uint8_t idx = 0; idx < 10 ; idx++)
	{
		uint8_t outNum = 0;
		uint32_t CmpNum = pgm_read_dword_near(num10s + idx);
		for(uint8_t i = 0; i < 10 ; i++)
		{
			if(num>=CmpNum)
			{
				num -= CmpNum;
				outNum++;
			}
			else
			{
				break;
			}
		}
		SerialSend('0' + outNum);
	}
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

//默认高电平，低电平有效
#define HIGH_RANGE (!(PINC & _BV(3)))
#define MID_RANGE  (!(PINC & _BV(4)))
#define LOW_RANGE  (!(PINC & _BV(5)))

//L:G1 H:G20
#define G20  (!(!(PINB & _BV(5))))

//电压/电流 通过数据口获得

#define CAL_V_LOW_RANGE_G20    1.0
#define CAL_V_LOW_RANGE_G1     1.0
#define CAL_V_MID_RANGE_G20    1.0
#define CAL_V_MID_RANGE_G1     1.0
#define CAL_V_HIGH_RANGE_G20   1.0
#define CAL_V_HIGH_RANGE_G1    1.0

#define CAL_A_LOW_RANGE_G20    1.0
#define CAL_A_LOW_RANGE_G1     1.0
#define CAL_A_MID_RANGE_G20    1.0
#define CAL_A_MID_RANGE_G1     1.0
#define CAL_A_HIGH_RANGE_G20   1.0
#define CAL_A_HIGH_RANGE_G1    1.0




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
    
    uint8_t v_a = 0;//0V 1A 第31bit
    uint8_t sign = 0;//最高位符号，其余位为0   第32bit

    while(EN);
    while(!EN);
    for(uint8_t i=0;i<32;i++){
      while(!CLK && EN);
      if(DATA){
        val_req |= mask;
      }
      while(CLK && EN);
      mask<<=1;
    }

    sign = (val_req & 0x80000000) != 0;
    v_a =  (val_req & 0x40000000) != 0;
    val_req &= 0x00FFFFFF;

    uint8_t ovf = 0;
    if(sign){
      if(val_req==524288){
        ovf = 1;
      }
    }else{
      if(val_req==524287){
        ovf = 1;
      }
    }
    SendInt(val_req);
    
    SerialSend(' ');
    SerialSend('0'+LOW_RANGE);
    SerialSend('0'+MID_RANGE);
    SerialSend('0'+HIGH_RANGE);
    SerialSend('0'+G20);
    SerialSend('\r');
    SerialSend('\n');
    
    float val = (float)val_req; //max +- 512k / 1.1v

    uint8_t digi0=0;
    uint8_t digi1=0;
    uint8_t digi2=0;
    uint8_t digi3=0;
    uint8_t digi0Dot=0;
    uint8_t digi1Dot=0;
    uint8_t digi2Dot=0;
    uint8_t digi3Dot=0;
    
    val = val / (512.0*1024.0); //v 1~0~-1
    if(v_a){
      //A
        
      if(LOW_RANGE){
        if(G20){
          //10OHM 1.1V to/20*1000  max 5.5mA
          val = val * 1.1 * CAL_A_LOW_RANGE_G20 * 0.05 * 1000 / 10;
          //5.5mA
          if(val>=1){
            //x.xxx mA
            digi0Dot = 1;
          }else{
            //xxxx uA
            val = val * 10;
          }
        }else{
          //10OHM 1.1V  max 0.11A
          val = val * 1.1 * CAL_V_LOW_RANGE_G1 / 10;
          //0.110A
          if(val>=0.1){
            //xxxx mA
            val = val * 10;
          }else{
            //xxx.x mA
            val = val * 100;
            digi2Dot = 1;
          }
        }
      }else if(MID_RANGE){
        if(G20){
          //1 OHM 1.1V to/20*1000  max 55mA
          val = val * 1.1 * CAL_A_MID_RANGE_G20 * 0.05 * 1000;
          //55mA
          if(val>=10){
            //xx.xx mA
            val = val * 0.1;
            digi1Dot = 1;
          }else{
            //x.xxx mA
            digi0Dot = 1;
          }
        }else{
          //1 OHM 1.1V max 1.1A
          val = val * 1.1 * CAL_V_MID_RANGE_G1;
          //1.1A
          if(val>=1){
            //x.xxx A
            digi0Dot = 1;
          }else{
            //xxxx mA
            val = val * 10;
          }
        }
      }else if(HIGH_RANGE){
        if(G20){
          //0.05 OHM 1.1V to*20  max 1.1A
          val = val * 1.1 * CAL_A_HIGH_RANGE_G20 * 0.05 / 0.05;
          // 1.1A
          if(val>=1){
            //x.xxx A
            digi0Dot = 1;
          }else{
            //xxxx mA
            val = val * 10;
          }
        }else{
          //0.05 OHM 1.1V max 22A
          val = val * 1.1 * CAL_V_HIGH_RANGE_G1 / 0.05;
          //22A
          if(val>=10){
            //xx.xx A
            val = val / 10;
            digi1Dot = 1;
          }else{
            //x.xxx A
            digi0Dot = 1;
          }
        }
      }
    }else{
      //V
      if(LOW_RANGE){
        if(G20){
          float cal = 1;
          //max 0.055V to/20
          //max 55mv to *1000
          val = val * 1.1 * CAL_V_LOW_RANGE_G20 / 1 * 0.05 * 1000;
          if(val>=10){
            //xx.xx mV
            val = val * 0.1;
            digi1Dot = 1;
          }else{
            //x.xxx mV
            digi0Dot = 1;
          }
        }else{
          //max 1.1V   *1
          val = val * 1.1 * CAL_V_LOW_RANGE_G1 / 1;
          if(val>=1){
            //x.xxx V
            digi0Dot = 1;
          }else{
            //xxxx mV
            val = val * 10;
            //digi-1Dot = 1;
          }
        }
      }else if(MID_RANGE){
        if(G20){
          //max 0.55V  to*10/20
          val = val * 1.1 * CAL_V_MID_RANGE_G20 * 10 * 0.05;
          if(val>=0.1){
            //xxx.x mV
            val = val * 10;
            digi2Dot = 1;
          }else{
            //xx.xx mV
            val = val * 100;
            digi1Dot = 1;
          }
        }else{
          //max 11V to *10
          val = val * 1.1 * CAL_V_MID_RANGE_G1 * 10;
          if(val>=10){
            //xx.xx V
            val = val * 0.1;
            digi1Dot = 1;
          }else{
            //x.xxx V
            digi0Dot = 1;
          }
        }
      }else if(HIGH_RANGE){
        if(G20){
          //max 5.5V  to*100/20
          val = val * 1.1 * CAL_V_MID_RANGE_G20 * 100 * 0.05;
          if(val>=1){
            //x.xxx V
            digi1Dot = 1;
          }else{
            //xxxx mV
            val = val * 0.1;
            //digi-1Dot = 1;
          }
        }else{
          //max 110V to *100
          val = val * 1.1 * CAL_V_MID_RANGE_G1 * 100;
          if(val>=100){
            //xxx.x V
            val = val * 0.01;
            digi2Dot = 1;
          }else{
            //xx.xx V
            val = val * 0.1;
            digi1Dot = 1;
          }
        }
      }
    }
    
    if(!ovf){
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
    }else{
      digi0 = 9;
      digi1 = 9;
      digi2 = 9;
      digi3 = 9;
    }
          
          
          
      //if(sign){
      //  SerialSend('-');  
		  //}else{
      //  SerialSend('+');
		  //}
      //SerialSend('0'+digi0);
		  //if(digi0Dot){
			//  SerialSend('.');
		  //}
      //SerialSend('0'+digi1);
		  //if(digi1Dot){
			//  SerialSend('.');
		  //}
		  //SerialSend('0'+digi2);
		  //if(digi2Dot){
			//  SerialSend('.');
		  //}
		  //SerialSend('0'+digi3);
		  //if(digi3Dot){
			//  SerialSend('.');
		  //}
      //
		  //SerialSend('\r');
		  //SerialSend('\n');
          

    _digi0    =digi0;
    _digi0Dot =digi0Dot;
    _digi1    =digi1;
    _digi1Dot =digi1Dot;
    _digi2    =digi2;
    _digi2Dot =digi2Dot;
    _digi3    =digi3;
    _digi3Dot =digi3Dot;
    _sign     =sign;
  }
}

volatile uint8_t cur_digit = 0;

ISR(TIMER1_COMPA_vect){
  TCNT1 = 0;
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
