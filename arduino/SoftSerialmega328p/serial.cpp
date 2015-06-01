#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	
#define CUR_TIMING TIMING_16M_TCCR1B_1_115200
#define TCCR1B_Value 1
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_3____300[] = {  833, 1666, 2500, 3333, 4166, 5000, 5833, 6666, 7500, 8333};
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_2___1200[] = { 1666, 3333, 5000, 6666, 8333,10000,11666,13333,15000,16666};
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_2___2400[] = {  833, 1666, 2500, 3333, 4166, 5000, 5833, 6666, 7500, 8333};
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_2___4800[] = {  416,  833, 1250, 1666, 2083, 2500, 2916, 3333, 3750, 4166};
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_2___9600[] = {  208,  416,  625,  833, 1041, 1250, 1458, 1666, 1875, 2083};
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_1__14400[] = { 1111, 2222, 3333, 4444, 5555, 6666, 7777, 8888,10000,11111};
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_1__19200[] = {  833, 1666, 2500, 3333, 4166, 5000, 5833, 6666, 7500, 8333};
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_1__28800[] = {  555, 1111, 1666, 2222, 2777, 3333, 3888, 4444, 5000, 5555};
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_1__38400[] = {  416,  833, 1250, 1666, 2083, 2500, 2916, 3333, 3750, 4166};
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_1__57600[] = {  277,  555,  833, 1111, 1388, 1666, 1944, 2222, 2500, 2777};
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_1_115200[] = {  138,  277,  416,  555,  694,  833,  972, 1111, 1250, 1388};


#define DDR_Send DDRD
#define PORT_Send PORTD
#define BIT_Send _BV(1)
#define DDR_Recv DDRD
#define PIN_Recv PIND
#define BIT_Recv _BV(0)

void loop();
void TimerInit();
void SerialInit();
void SerialSend(uint8_t val);
uint8_t SerialRead();

int main(void) {
	SerialInit();
	TimerInit();
	for(uint8_t v = '0';v<'9';v++)
	{
		SerialSend(v);
	}
	for(;;)
	{
		loop();
	}
}

uint8_t buff[8];
void loop() {
  // put your main code here, to run repeatedly: 
  for(int i=0;i<=8;i++)
  {
	buff[i]=SerialRead();
  }
  for(int i=0;i<8;i++)
  {
    SerialSend(buff[i]);
  }

  for(uint8_t v = '0';v<'9';v++)
  {
    SerialSend(v);
  }
  SerialSend('\r');
  SerialSend('\n');
}


void TimerInit() {
	TCCR1A = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
}
void SerialInit(){
	UCSR0B = 0;//not forget turnoff usart0 on mega328p
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
	PORT_Send &= ~BIT_Send;timing = pgm_read_word_near(CUR_TIMING);while(TCNT1<timing);//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 1;i<=8;i++)
	{
		if(val&chkbit){PORT_Send |= BIT_Send;}else{PORT_Send &= ~BIT_Send;}chkbit<<=1;timing = pgm_read_word_near(CUR_TIMING + i);while(TCNT1<timing);
	}
	PORT_Send |= BIT_Send;timing = pgm_read_word_near(CUR_TIMING + 9);while(TCNT1<timing);
	sei();
}

uint8_t SerialRead()
{
	cli();
	TCCR1B = TCCR1B_Value;
	uint8_t val = 0;
	while(PIN_Recv&BIT_Recv){}
	TCNT1 = 0;
	uint16_t timing;
	timing = pgm_read_word_near(CUR_TIMING);while(TCNT1<timing);//startbit

	for(uint8_t i = 1;i<=8;i++)
	{
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);timing = pgm_read_word_near(CUR_TIMING + i);while(TCNT1<timing);
	}
	sei();
	return val;
}

