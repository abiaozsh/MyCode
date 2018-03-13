#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define DDR_Recv DDRB
#define PIN_Recv PINB
#define BIT_Recv _BV(0)

void loop();
uint8_t SerialRead();

int main(void) {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
  
  
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Recv &= ~BIT_Recv;
  
  TCCR1B = 3;//488.28125 Hz PWM
  TIMSK0 = _BV(OCIE1A) + _BV(TOIE1);
  
  DDRA = _BV(0);
  sei();
  
	loop();
}

ISR(TIM1_COMPA_vect){
  PORTA = _BV(0);
}

ISR(TIM1_OVF_vect){
  PORTA = 0;
}


void loop() {
	for(;;)
	{
		uint8_t val = SerialRead();
    //OCR0A = 254;//ÂÌµÆÁÁµÄ±ÈÀý
		//if(val!=0){
			OCR1A = val<<8;
		//}
	}
}


//#define PIN_Recv_val (PIN_Recv&BIT_Recv)
#define PIN_Recv_val (!(PIN_Recv&BIT_Recv))
#define timing 200

uint8_t SerialRead()
{
	//cli();
	uint8_t val = 0;
	while(PIN_Recv_val);
	TCCR0B = 3;
	TCNT0 = 0;
	while(TCNT0<100);TCNT0 = 0;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		while(TCNT0<timing);TCNT0 = 0;
		val>>=1;val |= (PIN_Recv_val?0x80:0);
	}
	while(TCNT0<timing);TCNT0 = 0;
	//sei();
	return val;
}
