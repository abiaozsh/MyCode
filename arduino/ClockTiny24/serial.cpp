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

uint8_t days = 0;
uint8_t hour = 0;
uint8_t miniute = 0;
uint8_t second = 0;
uint16_t tick = 0;//15625 tick/s

uint8_t figure0;//days
uint8_t figure1;//hours
uint8_t figure2;
uint8_t figure3;
uint8_t figure4;
uint8_t figure5;
uint8_t figure6;
uint8_t dimeSeconds=0;


#define DDR_Send DDRA
#define PORT_Send PORTA
#define BIT_Send _BV(2)
#define DDR_Recv DDRA
#define PIN_Recv PINA
#define BIT_Recv _BV(1)

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
	loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
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


//0.016384 Ãë
void Timer0Init() {
	TCCR0A = 0;
	TCCR0B = 5;
	TIMSK0 = 0;
}

void Timer1Init() {
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

uint8_t lastTick = 0;//15625 tick/s
void IncTimer()
{
	uint8_t buffTick = TCNT0;
	uint8_t diffTick = buffTick - lastTick;
	lastTick = buffTick;
	tick+=diffTick;
	if(tick>=15625)
	{
		tick -= 15625;
		second+=2;
		if(second==60)
		{
			second = 0;
			miniute++;
			if(miniute==60)
			{
				miniute = 0;
				hour++;
				if(hour==24)
				{
					hour=0;
					days++;
				}
			}
		}
	}

	figure1 = hour / 10;
	figure2 = hour % 10;

	figure3 = miniute / 10;
	figure4 = miniute % 10;

	figure5 = second / 10;
	figure6 = second % 10;

	uint32_t temp = tick;
	dimeSeconds = (temp*10737)>>24;//0~9
}
