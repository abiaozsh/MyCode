#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	
#define CUR_TIMING TIMING__8M_TCCR0B_1_115200
#define TCCR0B_Value 1

PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2___9600[] = {104,208, 56,160,  8,113,217, 65,169, 17};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2__14400[] = { 69,138,208, 21, 91,160,230, 43,113,182};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_1_115200[] = { 69,138,208, 21, 91,160,230, 43,113,182};

#define DDR_Send DDRA
#define PORT_Send PORTA
#define BIT_Send _BV(2)
#define DDR_Recv DDRA
#define PIN_Recv PINA
#define BIT_Recv _BV(1)

void ClockInit();
void loop();
void TimerInit();
void SerialInit();
void SerialSend(uint8_t val);
//uint8_t SerialRead();
//uint8_t SerialRead(uint16_t timeout, uint8_t* timoutParam);
void SendInt(uint32_t val);
//uint32_t ReadInt();

int main(void) {
	loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

uint8_t buff[8];
void loop() {
	for(;;)
	{
    
      //初始化定时器 1/8
    TCCR1B = 2;//  1/8	1MHz 1us
    TIMSK1 |= _BV(OCIE1A);
    volatile uint16_t val = 12000;
    uint32_t temp = val;
    uint8_t aread = 123;
    TCNT1 = 0;//TIFR1 |= _BV(TOV1);timer reset //overflow flg reset
    temp*=aread;
    temp>>=8;
    uint16_t time = TCNT1;
    volatile uint16_t vvv = (uint16_t)temp;
    
    
	ClockInit();
	SerialInit();
	TimerInit();

    SendInt(time);
    
		SerialSend('\r');
		SerialSend('\n');
    
    uint32_t i = 0;
    for(i=0;i<100000;i++)
    {
      asm volatile("nop");
      
    }

	}
}


void TimerInit() {
	TCCR0A = 0;
	TIMSK0 = 0;
}

void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
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
	TCCR0B = TCCR0B_Value;
	TCNT0 = 0;
	uint8_t timing;
	prog_uint8_t* pTiming = CUR_TIMING;
	PORT_Send &= ~BIT_Send;timing = pgm_read_byte_near(pTiming++);while(timing-TCNT0>0);//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit){PORT_Send |= BIT_Send;}else{PORT_Send &= ~BIT_Send;}chkbit<<=1;timing = pgm_read_byte_near(pTiming++);while(timing-TCNT0>0);
	}
	PORT_Send |= BIT_Send;timing = pgm_read_byte_near(pTiming);while(timing-TCNT0>0);
	sei();
}

/*
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
	TCNT0 = 0;TIFR1 |= _BV(TOV1);
	while(PIN_Recv&BIT_Recv){
		if(((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)>=timeout)
		{
			*timoutParam = 0;
			return 0;
		}
	}
	TCCR0B = TCCR0B_Value;
	TCNT1 = 0;
	uint16_t timing;
	prog_uint16_t* pTiming = CUR_TIMING;

	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		timing = pgm_read_word_near(pTiming++);while(timing-TCNT0<128);
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);
	}
	sei();
	return val;
}
*/