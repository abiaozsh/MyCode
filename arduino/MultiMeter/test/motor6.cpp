#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>



//8000000 / 115200 = 69.4
//8000000 / 115200 * 1.5 = 104.16666666666666666666666666667
#define TCCR0_Value_115200 1
#define CNT_Value_115200 69
#define CNT_1_5_115200 96


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
#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(0)
#define DDR_Recv DDRB
#define PIN_Recv PINB
#define BIT_Recv _BV(1)



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
  TCCR0B = TCCR0_Value;

}
void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}

void SerialSend(uint8_t val){
	cli();
	TCCR0B = TCCR0_Value;
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



//4+8 reset disabled
//
//a0 vref - cap to GND
// a3-a3 calib? 100100*1  100101*20
// a3-a2 Vin    110000*1  110001*20  010000 010001
// a3-a4 Ain    010010*1  010011*20   110010 110011

//a1 1/50

//a5 high gain low gain //TODO auto
//a6 high val low val
//a7 A/V

//b0 b1 b2 2*595 8 seg + 4 digit + sign
//b3 free

/*
seg:
0.001
0.010
0.100
1.000
11.00
111.0
1111
*/

volatile uint8_t send_req;

volatile int32_t val_req;

void wait(uint16_t time){
	TCNT1 = 0;
	while(TCNT1<time);
	return;
}
volatile int16_t aval;

uint8_t conv[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

int main(void) {
  //初始化时钟：1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  
  SerialInit();
  TimerInit();
  
  //初始化定时器 1/8
  TCCR1B = 2;//  1/8	1MHz 1us
  //TIMSK1 |= _BV(OCIE1A);
  
  OCR1A = 5000;
  
  // a3-a3 calib? 0x24 10 0100*1  0x25 10 0101*20
  ADMUX = 0x80+0x25;//REFS1 Internal 1.1V voltage reference
  
  DIDR0 |= _BV(2);
  DIDR0 |= _BV(3);
  DIDR0 |= _BV(4);
  
  ADCSRA |= _BV(ADEN);
  ADCSRA |= _BV(ADSC);
  ADCSRA |= _BV(ADATE);
  ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
  ADCSRA |= _BV(ADIE);
  
  ADCSRB = _BV(BIN);
  
  //打开输出端口

  //初始化输出端口
//  DDRB |= _BV(0);
//  DDRB |= _BV(1);
//  DDRB |= _BV(2);
  
  sei();

  //zeroCalib();//offset
  
  ////////////
  // a3-a2 Vin    0x10 01 0000*1  0x11 01 0001*20
  ADMUX = 0x80+0x10;
  DDRB|=_BV(3);
  while(true){
    int32_t val = val_req;//aval;

	
    if(val>=0){
      SerialSend('+');
    }else{
      val=-val;
      SerialSend('-');
    }

    SendInt(val);
    SerialSend('\r');
    SerialSend('\n');
	
	  for(uint16_t i=0;i<100;i++){
      wait(1000);
	  }
	  PINB|=_BV(3);
  }
  /////////////////
    
}


uint16_t count;
int32_t val;

ISR(ADC_vect){
  count++;
  if(ADC&0x200){
	val += int16_t(0xFC00 | ADC);//max +- 512
  }else{
	val += ADC;
  }
  if(count>=1024){//max +- 512k / 1.1v
    count=0;
    val_req=val;
    val=0;
    send_req = 1;
  }
}
