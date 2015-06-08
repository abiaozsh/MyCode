#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "motor6.h"

	
#define CUR_TIMING TIMING__8M_TCCR1B_1_115200
#define TCCR1B_Value 1

PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1_115200[] = {   69,  138,  208,  277,  347,  416,  486,  555,  625,  694};

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

#define DDR_Send DDRA
#define PORT_Send PORTA
#define BIT_Send _BV(2)
#define DDR_Recv DDRA
#define PIN_Recv PINA
#define BIT_Recv _BV(1)


volatile uint8_t temp;

void ClockInit();
void TimerInit();
void SerialInit();
void AnalogInit();
void loop();
void SerialSend(uint8_t val);
uint8_t SerialRead();
uint8_t SerialRead(uint16_t timeout, uint8_t* timoutParam);
void SendInt(uint32_t val);
uint32_t ReadInt();


int main(void) {
	ClockInit();//初始化时钟：1MHz -> 8MHz
	AnalogInit();//初始化模拟输入
	SerialInit();
	TimerInit();


	//主循环
	loop();
}


void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

void AnalogInit() {
	ADMUX = 6;//A7 rpm
	ADCSRB = 0;//_BV(ADLAR);
	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
}

void TimerInit() {
	TCCR1A = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
}
void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}

void loop() {
	for(;;) {
		uint16_t aread1 = ADC;
		sbi(ADCSRA, ADSC);
		SendInt(aread1);
		SerialSend('\r');SerialSend('\n');

		for(uint32_t i = 0;i<500000;i++)
		{
			temp++;
		}
		
		uint16_t TargetRPM = 12300;
		ADMUX = 7;
		ADCSRB = _BV(ADLAR);
		ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2);// 1/2
		while(ADCSRA & ADSC)
		{
		}
		uint8_t aread2 = ADCH;
		SendInt(aread2);
		SerialSend('\r');SerialSend('\n');
		aread2 &= 127;
		AnalogInit();
		uint16_t temp = (((uint32_t)TargetRPM)*aread2)>>7;
		SendInt(temp);
		SerialSend('\r');SerialSend('\n');
		SendInt('e');
		SerialSend('\r');SerialSend('\n');
	}
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

void SerialSend(uint8_t val)
{
	cli();
	TCCR1B = TCCR1B_Value;
	TCNT1 = 0;
	uint16_t timing;
	prog_uint16_t* pTiming = CUR_TIMING;
	PORT_Send &= ~BIT_Send;timing = pgm_read_word_near(pTiming++);while(TCNT1<timing);//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit){PORT_Send |= BIT_Send;}else{PORT_Send &= ~BIT_Send;}chkbit<<=1;timing = pgm_read_word_near(pTiming++);while(TCNT1<timing);
	}
	PORT_Send |= BIT_Send;timing = pgm_read_word_near(pTiming);while(TCNT1<timing);
	sei();
}

uint32_t ReadInt()
{
	uint32_t num = 0;
	uint8_t buff[10];
	uint8_t i;
	for(i=0;i<10;i++)
	{
		uint8_t data = SerialRead();
		if(data >= '0' && data <= '9')
		{
			buff[i] = data;
		}
		else
		{
			break;
		}
	}
	for(uint8_t j=0;j<i;j++)
	{
		num *= 10;
		num += buff[j] - '0';
	}
	return num;
}

uint8_t SerialRead()
{
	uint8_t timoutParam = 0;
	return SerialRead(65535, &timoutParam);
}

uint8_t SerialRead(uint16_t timeout, uint8_t* timoutParam)
{
	cli();
	uint8_t val = 0;
	TCCR1B = *timoutParam;//timeout param 1~5 : 1/(1~1024) max (0.008192 s ~ 8.388608 s)
	TCNT1 = 0;TIFR1 |= _BV(TOV1);
	while(PIN_Recv&BIT_Recv){
		if(((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)>=timeout)
		{
			*timoutParam = 0;
			return 0;
		}
	}
	TCCR1B = TCCR1B_Value;
	TCNT1 = 0;
	uint16_t timing;
	prog_uint16_t* pTiming = CUR_TIMING;

	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		timing = pgm_read_word_near(pTiming++);while(TCNT1<timing);
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);
	}
	sei();
	return val;
}
