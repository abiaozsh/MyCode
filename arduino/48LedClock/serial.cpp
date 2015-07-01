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


/////////////////////////////////////////////////////////////////////////////////////////
void TimeInit();
void TimeWait();
void writeOut(uint8_t value);
uint8_t readIn();
void ReadTime();
void WriteTime();
/////////////////////////////////////////////////////////////////////////////////////////

void ClockInit();
void loop();
void Timer0Init();
void Timer1Init();
void SerialInit();
void SerialSend(uint8_t val);
void IncTimer();

int main(void) {
	ClockInit();
	DDRA = _BV(0);//pin0
	SerialInit();
	Timer0Init();
	Timer1Init();
	TimeInit();
	
	
	
	loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

void TimeInit()
{
	DDR_ce |= BIT_ce;
	DDR_sclk |= BIT_sclk;
}

volatile uint8_t temp;
void TimeWait()
{
	for(uint8_t i=0;i<100;i++)
	{
		temp++;
	}
}

uint8_t lastSecond = 0;

void loop() {
	for(;;)
	{
		// put your main code here, to run repeatedly: 
		if(lastSecond!=second)
		{
			lastSecond=second;
			PINA=_BV(0);
		}
		//delay
		for(uint16_t i=0;i<10000;i++)
		{
			IncTimer();
		}
		
		IncTimer();SerialSend('0'+days);
		IncTimer();SerialSend(',');
		IncTimer();SerialSend('0'+figure1);
		IncTimer();SerialSend('0'+figure2);
		IncTimer();SerialSend(':');
		IncTimer();SerialSend('0'+figure3);
		IncTimer();SerialSend('0'+figure4);
		IncTimer();SerialSend(':');
		IncTimer();SerialSend('0'+figure5);
		IncTimer();SerialSend('0'+figure6);
		IncTimer();SerialSend('.');
		IncTimer();SerialSend('0'+dimeSeconds);
		IncTimer();SerialSend('\r');
		IncTimer();SerialSend('\n');
		
		
		
	}
}



void Timer1Init() {
	TCCR1A = 0;
	TCCR1B = 1;
	TCCR1C = 0;
	TIMSK1 = 0;
}

void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}

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





