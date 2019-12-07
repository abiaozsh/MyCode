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

#define currTick ((TIFR & _BV(TOV1))?0x0FFFF:TCNT1)

//pd0 rxd		usb txd
//pd1 txd		usb rxd
#define DDR_Send DDRD
#define PORT_Send PORTD
#define BIT_Send _BV(1)
#define DDR_Recv DDRD
#define PIN_Recv PIND
#define BIT_Recv _BV(0)

void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}
void TimerInit()
{
  // COM1A1 COM1A0 COM1B1 COM1B0 ¨C ¨C WGM11 WGM10
  TCCR1A = 0;
  //ICNC1 ICES1 ¨C WGM13 WGM12 CS12 CS11 CS10

  //0 0 1 clkI/O/1 (No prescaling)
  //0 1 0 clkI/O/8 (From prescaler)
  //0 1 1 clkI/O/64 (From prescaler)
  //1 0 0 clkI/O/256 (From prescaler)
  //1 0 1 clkI/O/1024 (From prescaler)
  //TCCR1B = 1;//  1/1
  //TCCR1B = 2;//  1/8 //1Mhz
  //TCCR1B = 3;//  1/64
  
  TCCR1B = TCCR1B_Value;
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

//uint8_t SerialRead()
//{
//	uint8_t timoutParam = 0;
//	return SerialRead(65535, &timoutParam);
//}
uint8_t SerialRead()//uint16_t timeout, uint8_t* timoutParam
{
	cli();
	uint8_t val = 0;
	//TCCR1B = *timoutParam;//timeout param 1~5 : 1/(1~1024) max (0.008192 s ~ 8.388608 s)
	//TCNT1 = 0;TIFR |= _BV(TOV1);
	while(PIN_Recv&BIT_Recv){
		//if(((TIFR & _BV(TOV1))?0x0FFFF:TCNT1)>=timeout)
		//{
		//	*timoutParam = 0;
		//	return 0;
		//}
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


#define blinkDDR DDRC |=  _BV(0);
#define blinkH PORTC |=  _BV(0);
#define blinkL PORTC &= ~_BV(0);


void wait(uint16_t ticks)
{
	TCNT1 = 0;//timer reset
	while(TCNT1<(ticks<<3))
	{
		;
	}
}

int main()
{
	TimerInit(); //do later

	blinkDDR;
	
	for (;;) {
    uint8_t c = SerialRead();
    if(c=='1'){
      for(uint16_t i=0;i<1000;i++)
      {
        wait(1000);//1000ms
      }
      blinkH;
    }
    if(c=='0'){
      blinkL;
    }
	}
        
	return 0;
}
