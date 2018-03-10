#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	
#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

#define DDR_Recv DDRB
#define PIN_Recv PINB
#define BIT_Recv _BV(0)

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
  
  
  TCCR0B = 3;//488.28125 Hz PWM
  TIMSK0 = _BV(OCIE0A) + _BV(TOIE0);
  
  DDRA = _BV(0);
  sei();
  
	loop();
}

ISR(TIM0_COMPA_vect){
  PORTA = _BV(0);
}

ISR(TIM0_OVF_vect){
  PORTA = 0;
}


void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

void loop() {
	for(;;)
	{
		uint8_t val = SerialRead();
    //OCR0A = 254;//ÂÌµÆÁÁµÄ±ÈÀý
		//if(val!=0){
			OCR0A = val;
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
	DDR_Recv &= ~BIT_Recv;
}


//#define PIN_Recv_val (PIN_Recv&BIT_Recv)
#define PIN_Recv_val (!(PIN_Recv&BIT_Recv))
#define timing 200

uint8_t SerialRead()
{
	//cli();
	uint8_t val = 0;
	TCNT1 = 0;TIFR1 |= _BV(TOV1);
	while(PIN_Recv_val);
	TCCR1B = 3;
	TCNT1 = 0;
	while(TCNT1<100);TCNT1 = 0;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		while(TCNT1<timing);TCNT1 = 0;
		val>>=1;val |= (PIN_Recv_val?0x80:0);
	}
	TCNT1 = 0;
	while(TCNT1<200);TCNT1 = 0;
	//sei();
	return val;
}
