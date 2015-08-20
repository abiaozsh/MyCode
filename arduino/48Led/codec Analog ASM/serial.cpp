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


#define getV(pos,bit) (((AltBuff[pos]-DT)&0x80)?0:bit)
//#define getV(pos,bit) (((DT-AltBuff[pos])&0x80)>>bit)

//#define getV(pos,bit) ((AltBuff[pos]>=60)?bit:0)
volatile uint8_t Count256;
volatile uint8_t CurrentDT;

volatile uint8_t data0;
volatile uint8_t data1;
volatile uint8_t data2;
volatile uint8_t data3;
volatile uint8_t data4;
volatile uint8_t data5;
volatile uint8_t data6;
volatile uint8_t data7;

volatile uint8_t* volatile AltBuff;//正在写入
volatile uint8_t buff1[48];//乱序 0~5 6~11。。。。

PROGMEM prog_uint8_t idx[] = {0,1,2,3,4,5,6,7,8,9};//48

void SerialSend(uint8_t val){
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

void SendInt(uint32_t val){
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


#define proc(num)   \
{                                 \
    __asm__ __volatile__                         \
    (                                            \
      "ldi r18,0\n\t" \
      /*r17<-AltBuff[i] ; AltBuff[i]-DT store carry*/ \
      "ld r17,X+\n\t" "cp r17,r16\n\t" "ror r18\n\t" \
      "ld r17,X+\n\t" "cp r17,r16\n\t" "ror r18\n\t" \
      "ld r17,X+\n\t" "cp r17,r16\n\t" "ror r18\n\t" \
      "ld r17,X+\n\t" "cp r17,r16\n\t" "ror r18\n\t" \
      "ld r17,X+\n\t" "cp r17,r16\n\t" "ror r18\n\t" \
      "ld r17,X+\n\t" "cp r17,r16\n\t" "ror r18\n\t" \
      "sts %0,r18\n\t" \
      ::"m" (num)\
    );                                           \
}


int main(void) {

	UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
	
	uint8_t time = TCNT0;
	TIMSK0 = 0;
	cli();
	while(true)
	{
    for(uint8_t i=0;i<48;i++)
    {
      uint8_t temp = AltBuff[pgm_read_byte_near(idx+i)];
      temp++;
      AltBuff[pgm_read_byte_near(idx+i)] = temp;
    }
    
  //26 27 x
  //28 29 y
  //30 31 z

	asm volatile("push r26");//16 17
	asm volatile("push r27");
	asm volatile("push r16");
	asm volatile("push r17");
	asm volatile("push r18");

	asm volatile("lds r26,AltBuff");
	asm volatile("lds r27,(AltBuff)+1");
	asm volatile("lds r16,CurrentDT");
  
  proc(data7);
  proc(data6);
  proc(data5);
  proc(data4);
  proc(data3);
  proc(data2);
  proc(data1);
  proc(data0);
  
	asm volatile("pop r18");
	asm volatile("pop r17");
	asm volatile("pop r16");
	asm volatile("pop r27");
	asm volatile("pop r26");


		SendInt(time);
		SerialSend('\r');
		SerialSend('\n');
		for(long i=0;i<300000;i++)
		{
		volatile long vv=0;vv++;
		}
	}
}
