#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define CUR_TIMING TIMING_16M_TCCR1B_1_115200
#define TCCR1B_Value 1

PROGMEM prog_uint16_t TIMING_16M_TCCR1B_1_115200[] = {  138,  277,  416,  555,  694,  833,  972, 1111, 1250, 1388};

#define DDR_Send DDRD
#define PORT_Send PORTD
#define BIT_Send _BV(1)
#define DDR_Recv DDRD
#define PIN_Recv PIND
#define BIT_Recv _BV(0)



//d0,d1 serial
//d2~d7 6 io 74hc595
#define DDR_595_6 DDRD
#define PORT_595_6 PORTD

//b0 b1 b2 clk + st + oe 74hc595
#define DDR_CLK_ON   DDRB  |=  _BV(1)
#define PORT_CLK_ON  PORTB |=  _BV(1)
#define PORT_CLK_OFF PORTB &= ~_BV(1)

#define DDR_OE_ON   DDRB  |=  _BV(0)
#define PORT_OE_ON  PORTB |=  _BV(0)
#define PORT_OE_OFF PORTB &= ~_BV(0)

#define DDR_PNP1_ON   DDRC  |=  _BV(3)
#define PORT_PNP1_ON  PORTC |=  _BV(3)
#define PORT_PNP1_OFF PORTC &= ~_BV(3)

#define DDR_PNP2_ON   DDRC  |=  _BV(2)
#define PORT_PNP2_ON  PORTC |=  _BV(2)
#define PORT_PNP2_OFF PORTC &= ~_BV(2)

#define DDR_LED1_ON   DDRB  |=  _BV(4)
#define PORT_LED1_ON  PORTB |=  _BV(4)
#define PORT_LED1_OFF PORTB &= ~_BV(4)

#define DDR_LED2_ON   DDRB  |=  _BV(5)
#define PORT_LED2_ON  PORTB |=  _BV(5)
#define PORT_LED2_OFF PORTB &= ~_BV(5)


PROGMEM  prog_uint8_t  DitherTable[] = {
0xff,0x7f,0xbf,0x3f,0xdf,0x5f,0x9f,0x1f,0xef,0x6f,0xaf,0x2f,0xcf,0x4f,0x8f,0x0f,
0xf7,0x77,0xb7,0x37,0xd7,0x57,0x97,0x17,0xe7,0x67,0xa7,0x27,0xc7,0x47,0x87,0x07,
0xfb,0x7b,0xbb,0x3b,0xdb,0x5b,0x9b,0x1b,0xeb,0x6b,0xab,0x2b,0xcb,0x4b,0x8b,0x0b,
0xf3,0x73,0xb3,0x33,0xd3,0x53,0x93,0x13,0xe3,0x63,0xa3,0x23,0xc3,0x43,0x83,0x03,
0xfd,0x7d,0xbd,0x3d,0xdd,0x5d,0x9d,0x1d,0xed,0x6d,0xad,0x2d,0xcd,0x4d,0x8d,0x0d,
0xf5,0x75,0xb5,0x35,0xd5,0x55,0x95,0x15,0xe5,0x65,0xa5,0x25,0xc5,0x45,0x85,0x05,
0xf9,0x79,0xb9,0x39,0xd9,0x59,0x99,0x19,0xe9,0x69,0xa9,0x29,0xc9,0x49,0x89,0x09,
0xf1,0x71,0xb1,0x31,0xd1,0x51,0x91,0x11,0xe1,0x61,0xa1,0x21,0xc1,0x41,0x81,0x01,
0xfe,0x7e,0xbe,0x3e,0xde,0x5e,0x9e,0x1e,0xee,0x6e,0xae,0x2e,0xce,0x4e,0x8e,0x0e,
0xf6,0x76,0xb6,0x36,0xd6,0x56,0x96,0x16,0xe6,0x66,0xa6,0x26,0xc6,0x46,0x86,0x06,
0xfa,0x7a,0xba,0x3a,0xda,0x5a,0x9a,0x1a,0xea,0x6a,0xaa,0x2a,0xca,0x4a,0x8a,0x0a,
0xf2,0x72,0xb2,0x32,0xd2,0x52,0x92,0x12,0xe2,0x62,0xa2,0x22,0xc2,0x42,0x82,0x02,
0xfc,0x7c,0xbc,0x3c,0xdc,0x5c,0x9c,0x1c,0xec,0x6c,0xac,0x2c,0xcc,0x4c,0x8c,0x0c,
0xf4,0x74,0xb4,0x34,0xd4,0x54,0x94,0x14,0xe4,0x64,0xa4,0x24,0xc4,0x44,0x84,0x04,
0xf8,0x78,0xb8,0x38,0xd8,0x58,0x98,0x18,0xe8,0x68,0xa8,0x28,0xc8,0x48,0x88,0x08,
0xf0,0x70,0xb0,0x30,0xd0,0x50,0x90,0x10,0xe0,0x60,0xa0,0x20,0xc0,0x40,0x80,0x00
};

#define getV(pos,bit) ((AltBuff[pos]>pgm_read_byte_near(DitherTable + Count256))?bit:0)
//#define getV(pos,bit) ((AltBuff[pos]>=60)?bit:0)
volatile uint8_t Count256;


volatile uint8_t* volatile currBuff;//正在写入
volatile uint8_t* volatile AltBuff;//显示用
volatile uint8_t buff0[48];
volatile uint8_t buff1[48];


void SendLine();
void Conv();

int main(void) {
cli();
	DDR_595_6 = 0xFC;//B11111100;
	DDR_CLK_ON;
	DDR_OE_ON;
	DDR_PNP1_ON;
	//DDR_LED1_ON;
	//DDR_LED2_ON;
	
  //刷新定时器初始化
  TCCR0A = 0;//Initial Value 0 0 0 0 0 0 0 0
  TCCR0B = 2;
  TCNT0 = 0;
  OCR0A = 255;//数字越大越暗（match以后开OE，定时器超时关OE）
  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);

	TCCR1A = 0;
	TCCR1B = 5;//1/1024 (16000000/1024=15625)tick/s
	TCCR1C = 0;
	TIMSK1 = 0;
	
  //UCSR0A = 0;
	//UCSR0B = _BV(RXEN0);
  
  PORT_PNP1_OFF;
  PORT_OE_OFF;
  
  AltBuff = buff0;
  for(uint8_t i = 0;i<48;i++)
  {
    buff0[i] = i*4;
  }
  //buff0[1] = 1;
  //buff0[2] = 2;
  //buff0[3] = 3;
  sei();
	while(true)
	{
	  for(uint8_t i = 0;i<47;i++)
	  {
		buff1[i] = buff0[i+1];
	  }
		buff1[47] = buff0[0];
		AltBuff = buff1;
		for(long i=0;i<300;i++)
		{
		volatile int vv=0;vv++;
		}
		
		
	  for(uint8_t i = 0;i<47;i++)
	  {
		buff0[i] = buff1[i+1];
	  }
		buff0[47] = buff1[0];
		AltBuff = buff0;
		for(long i=0;i<300;i++)
		{
		volatile int vv=0;vv++;
		}
  	}
}

void SerialSend(uint8_t val)
{
	cli();
	TCCR1B = TCCR1B_Value;
	TCNT1 = 0;
	uint16_t timing;
	PORT_Send &= ~BIT_Send;timing = pgm_read_word_near(CUR_TIMING);while(TCNT1<timing);//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 1;i<=8;i++)
	{
		if(val&chkbit){PORT_Send |= BIT_Send;}else{PORT_Send &= ~BIT_Send;}chkbit<<=1;timing = pgm_read_word_near(CUR_TIMING + i);while(TCNT1<timing);
	}
	PORT_Send |= BIT_Send;timing = pgm_read_word_near(CUR_TIMING + 9);while(TCNT1<timing);
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

ISR(TIMER0_OVF_vect){
  PORT_PNP1_OFF;//关闭输出,开始传输
 uint8_t data[8];
  data[7] = getV(5*8+7,_BV(7)) | getV(4*8+0,_BV(6)) | getV(3*8+7,_BV(5)) | getV(2*8+0,_BV(4)) | getV(1*8+7,_BV(3)) | getV(0*8+0,_BV(2)) |0 |0;
  data[6] = getV(5*8+5,_BV(7)) | getV(4*8+2,_BV(6)) | getV(3*8+5,_BV(5)) | getV(2*8+2,_BV(4)) | getV(1*8+5,_BV(3)) | getV(0*8+2,_BV(2)) |0 |0;
  data[5] = getV(5*8+3,_BV(7)) | getV(4*8+4,_BV(6)) | getV(3*8+3,_BV(5)) | getV(2*8+4,_BV(4)) | getV(1*8+3,_BV(3)) | getV(0*8+4,_BV(2)) |0 |0;
  data[4] = getV(5*8+1,_BV(7)) | getV(4*8+6,_BV(6)) | getV(3*8+1,_BV(5)) | getV(2*8+6,_BV(4)) | getV(1*8+1,_BV(3)) | getV(0*8+6,_BV(2)) |0 |0;
  data[3] = getV(4*8+7,_BV(7)) | getV(5*8+0,_BV(6)) | getV(2*8+7,_BV(5)) | getV(3*8+0,_BV(4)) | getV(0*8+7,_BV(3)) | getV(1*8+0,_BV(2)) |0 |0;
  data[2] = getV(4*8+5,_BV(7)) | getV(5*8+2,_BV(6)) | getV(2*8+5,_BV(5)) | getV(3*8+2,_BV(4)) | getV(0*8+5,_BV(3)) | getV(1*8+2,_BV(2)) |0 |0;
  data[1] = getV(4*8+3,_BV(7)) | getV(5*8+4,_BV(6)) | getV(2*8+3,_BV(5)) | getV(3*8+4,_BV(4)) | getV(0*8+3,_BV(3)) | getV(1*8+4,_BV(2)) |0 |0;
  data[0] = getV(4*8+1,_BV(7)) | getV(5*8+6,_BV(6)) | getV(2*8+1,_BV(5)) | getV(3*8+6,_BV(4)) | getV(0*8+1,_BV(3)) | getV(1*8+6,_BV(2)) |0 |0;
  Count256++;
  
	PORT_CLK_OFF;
	volatile uint8_t* idx = data;
	PORT_595_6 = *idx++;PORT_CLK_ON;PORT_CLK_OFF;
	PORT_595_6 = *idx++;PORT_CLK_ON;PORT_CLK_OFF;
	PORT_595_6 = *idx++;PORT_CLK_ON;PORT_CLK_OFF;
	PORT_595_6 = *idx++;PORT_CLK_ON;PORT_CLK_OFF;
	PORT_595_6 = *idx++;PORT_CLK_ON;PORT_CLK_OFF;
	PORT_595_6 = *idx++;PORT_CLK_ON;PORT_CLK_OFF;
	PORT_595_6 = *idx++;PORT_CLK_ON;PORT_CLK_OFF;
	PORT_595_6 = *idx++;PORT_CLK_ON;PORT_CLK_OFF;
	PORT_CLK_ON; //shift clock up
	PORT_CLK_OFF; //shift clock down
  /*
	UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
	
	uint8_t time = TCNT0;
	TIMSK0 = 0;
	cli();
	while(true)
	{
		SendInt(time);
		SerialSend('\r');
		SerialSend('\n');
		for(long i=0;i<300000;i++)
		{
		volatile long vv=0;vv++;
		}
	}*/
}

ISR(TIMER0_COMPA_vect){
  PORT_PNP1_ON;//打开输出
}
