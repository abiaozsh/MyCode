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

#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(0)
#define DDR_Recv DDRB
#define PIN_Recv PINB
#define BIT_Recv _BV(1)

void ClockInit();
void loop();
void TimerInit();
void SerialInit();
void SerialSend(uint8_t val);
void SendInt(uint32_t val, uint8_t digits);
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
void convb(uint8_t name){
	uint16_t i;
	int32_t val = 0;
	ADCSRA |= _BV(ADSC);while(ADCSRA & _BV(ADSC));
	ADCSRA |= _BV(ADSC);while(ADCSRA & _BV(ADSC));
	for(i=0;i<1024;i++){
		ADCSRA |= _BV(ADSC);while(ADCSRA & _BV(ADSC));
		uint16_t v = ADC;
		if(v>=512){
			v = 1024 - v;
			val -= v;
		}
		else
		{
			val += v;
		}
		
	}
	val = val >> 6;
	
	SerialSend(name);SerialSend(':');
	if(val<0){
		val = - val;
		SerialSend('-');
	}
	else
	{
		SerialSend(' ');
	}
	SendInt(val,10);SerialSend(' ');
}
void loop() {
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);//ADATE ADIF ADIE
	DIDR0 |= _BV(ADC3D) | _BV(ADC2D);
	for(;;)
	{

		ADCSRB = 0;
		ADCSRB |= _BV(BIN);
		ADMUX = 0;
		ADMUX |= _BV(REFS1);
		ADMUX |= 0x11;//010000 010001  0x11
		convb('d');

		ADCSRB = 0;
		ADCSRB |= _BV(BIN);
		ADMUX = 0;
		ADMUX |= 0x11;//010000 010001  0x11
		convb('e');

		SerialSend('\r');
		SerialSend('\n');

		
		
		
		//uint32_t i = 0;
		//for(i=0;i<50000;i++)
		//{
		//	asm volatile("nop");
		//}
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

void SendInt(uint32_t val, uint8_t digits){
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
		if(10-idx<=digits){
			SerialSend('0' + outNum);\
		}
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
