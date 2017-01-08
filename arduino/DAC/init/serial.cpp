#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	

#define DDR_DAT_ON   DDRA  |=  _BV(3)
#define PORT_DAT_ON  PORTA |=  _BV(3)
#define PORT_DAT_OFF PORTA &= ~_BV(3)

#define DDR_CLK_ON   DDRA  |=  _BV(5)
#define PORT_CLK_ON  PORTA |=  _BV(5)
#define PORT_CLK_OFF PORTA &= ~_BV(5)

#define DDR_STR_ON   DDRA  |=  _BV(2)
#define PORT_STR_ON  PORTA |=  _BV(2)
#define PORT_STR_OFF PORTA &= ~_BV(2)

#define DDR_OE_ON    DDRA  |=  _BV(4)
#define PORT_OE_ON   PORTA |=  _BV(4)
#define PORT_OE_OFF  PORTA &= ~_BV(4)


#define CUR_TIMING TIMING__8M_TCCR1B_1_115200
#define TCCR1B_Value 1

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


void SendByte(uint8_t data){
  for(uint8_t i=0;i<8;i++)
  {
    if(data&1)
    {
      PORT_DAT_ON;
    }
    else
    {
      PORT_DAT_OFF;
    }
    data>>=1;
    PORT_CLK_ON; //shift clock up
    PORT_CLK_OFF; //shift clock down
  }
}
void sendData(uint8_t ByteA, uint8_t ByteB){
  SendByte(ByteA);
  SendByte(ByteB);
  PORT_STR_ON;
  PORT_STR_OFF;
}

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


void conv(){
	uint8_t i;
	uint32_t val = 0;
	ADCSRA |= _BV(ADSC);while(ADCSRA & _BV(ADSC));
	ADCSRA |= _BV(ADSC);while(ADCSRA & _BV(ADSC));
	for(i=0;i<64;i++){
		ADCSRA |= _BV(ADSC);while(ADCSRA & _BV(ADSC));
		uint16_t v = ADC;
		val += v;
	}
	val = val >> 6;
	
	SerialSend('v');SerialSend(':');SendInt(val,6);SerialSend('\t');
	
}

void loop() {
	DDR_DAT_ON;
	DDR_CLK_ON;
	DDR_STR_ON;
	DDR_OE_ON ;
	PORT_OE_OFF;

	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);//ADATE ADIF ADIE
	DIDR0 |= _BV(ADC6D);
	ADMUX = 6;
	ADCSRB = 0;
	{
		uint32_t i = 0;
		for(i=0;i<1000000;i++)
		{
			asm volatile("nop");
		}
	}
	{
		uint16_t i = 0;
		uint16_t j = 0;
		for(i=0;i<256;i++)
		{
			for(j=0;j<256;j++)
			{
				sendData(i,j);
				SerialSend('i');SerialSend(':');SendInt(i,4);SerialSend('\t');
				SerialSend('j');SerialSend(':');SendInt(j,4);SerialSend('\t');
				conv();
				
				SerialSend('\r');
				SerialSend('\n');
			}
		}
	}
	
	for(;;){}
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
