#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	
#define CUR_TIMING TIMING__8M_TCCR1B_1_115200
#define TCCR1B_Value 1

PROGMEM prog_uint16_t TIMING__8M_TCCR1B_3____300[] = {  416,  833, 1250, 1666, 2083, 2500, 2916, 3333, 3750, 4166};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_2___1200[] = {  833, 1666, 2500, 3333, 4166, 5000, 5833, 6666, 7500, 8333};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_2___2400[] = {  416,  833, 1250, 1666, 2083, 2500, 2916, 3333, 3750, 4166};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_2___4800[] = {  208,  416,  625,  833, 1041, 1250, 1458, 1666, 1875, 2083};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1___9600[] = {  833, 1666, 2500, 3333, 4166, 5000, 5833, 6666, 7500, 8333};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1__14400[] = {  555, 1111, 1666, 2222, 2777, 3333, 3888, 4444, 5000, 5555};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1__19200[] = {  416,  833, 1250, 1666, 2083, 2500, 2916, 3333, 3750, 4166};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1__28800[] = {  277,  555,  833, 1111, 1388, 1666, 1944, 2222, 2500, 2777};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1__38400[] = {  208,  416,  625,  833, 1041, 1250, 1458, 1666, 1875, 2083};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1__57600[] = {  138,  277,  416,  555,  694,  833,  972, 1111, 1250, 1388};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1_115200[] = {   69,  138,  208,  277,  347,  416,  486,  555,  625,  694};

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

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
void SendInt(uint32_t val);
uint8_t SerialRead();
uint8_t SerialRead(uint16_t timeout, uint8_t* timoutParam);
uint32_t ReadInt();

int main(void) {
	ClockInit();
	SerialInit();
	TimerInit();
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
		// put your main code here, to run repeatedly: 
//		for(int i=0;i<=8;i++)
//		{
//		buff[i]=SerialRead();
//		}
//		for(int i=0;i<8;i++)
//		{
//		SerialSend(buff[i]);
//		}
//
//		for(uint8_t v = '0';v<'9';v++)
//		{
//		SerialSend(v);
//		}
//		SerialSend('\r');
//		SerialSend('\n');
		SerialSend('a');
		SerialSend('\r');
		SerialSend('\n');
		uint32_t val1 = ReadInt();
		SerialSend('b');
		SerialSend('\r');
		SerialSend('\n');
		uint32_t val2 = ReadInt();
		uint32_t vala = val1/val2;
		uint32_t valb = val1%val2;
		SendInt(vala);
		SerialSend(',');
		SendInt(valb);
		
		SerialSend('\r');
		SerialSend('\n');
		uint16_t TargetRPM = val1;
		uint16_t Power;
		uint16_t Tick;
		uint16_t rpm = 12345;
		{
			TCCR1A = 0;
			TCCR1B = 2;//  1/8	1MHz 1us
			TCCR1C = 0;
			TIMSK1 = 0;
			TCNT1 = 0;TIFR1 |= _BV(TOV1);
			
			uint32_t temp = TargetRPM;  // TargetRPM/rpm *65536  ->0~65536
			temp = temp << 16;//*65536
			temp = temp / rpm;
			temp = 65536 - temp;//65536~~>0 (0 为转速非常接近)
			temp = rpm * temp;
			temp = temp >> 10;// 数字越大越柔和    /1024
			if(temp>rpm)
			{
				Power = rpm;
			}
			else
			{
				Power = temp;
			}
			Tick = currTick;
		}
		SendInt(Power);
		SerialSend(',');
		SendInt(Tick);

		SerialSend('\r');
		SerialSend('\n');
	}
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

void SerialSend(uint8_t val){
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
