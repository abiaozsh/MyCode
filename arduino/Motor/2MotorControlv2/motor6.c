#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
//С����
#define STAOn   DDRB |= _BV(3) ;/**/
#define STAOff  DDRB &= ~_BV(3);/**/

#define ROTOn   PORTB |= _BV(2) ;/**/
#define ROTOff  PORTB &= ~_BV(2);/**/
uint8_t i = 0;

#define CUR_TIMING TIMING__8M_TCCR1B_1_115200
#define TCCR1B_Value 1
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1_115200[] = {   69,  138,  208,  277,  347,  416,  486,  555,  625,  694};

#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(0)
#define DDR_Recv DDRB
#define PIN_Recv PINB
#define BIT_Recv _BV(1)

void loop();
void TimerInit();
void SerialInit();
uint8_t SerialRead();

int main(void) {
	CLKPR = 128;CLKPR = 0;
  SerialInit();
  TimerInit();
  MCUCR |= _BV(PUD);
  PORTA = 0x95;//10010101 LHHLHLHL
  DDRB |= _BV(2) ;
  loop();
}

void loop() {
	for(;;)
	{
    uint8_t data = SerialRead();
	
		if(data == (0x04 + 0x08 + 0x10 + 0x20)){STAOn;ROTOn;continue;}
		if(data == (0x80 + 0x40 + 0x01 + 0x02)){STAOff;ROTOff;continue;}

		if((data & (0x04 + 0x08)) == (0x04 + 0x08))continue;
		if((data & (0x10 + 0x20)) == (0x10 + 0x20))continue;
		if((data & (0x80 + 0x40)) == (0x80 + 0x40))continue;
		if((data & (0x01 + 0x02)) == (0x01 + 0x02))continue;

    DDRA = data;
    i=~i;
    if(i)
    {
      STAOn;
    }
    else
    {
      STAOff;
    }
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

uint8_t SerialRead()
{
	TCCR1B = TCCR1B_Value;
	uint8_t val = 0;
	while(PIN_Recv&BIT_Recv){}
	TCNT1 = 0;
	uint16_t timing;
	timing = pgm_read_word_near(CUR_TIMING);while(TCNT1<timing);//startbit

  uint8_t i;
	for(i = 1;i<=8;i++)
	{
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);timing = pgm_read_word_near(CUR_TIMING + i);while(TCNT1<timing);
	}
	return val;
}
