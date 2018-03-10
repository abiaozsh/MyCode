#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	
#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(0)

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
  
  //ADMUX = POWER_IN;//A7 power
  DIDR0 |= _BV(0);//1
  ADCSRA = _BV(ADEN) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);// | _BV(ADIE) | _BV(ADATE) _BV(ADSC) | 
  ADCSRB = _BV(ADLAR);
  
  
	loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

uint8_t aread(uint8_t pin){
  ADMUX = pin;
  ADCSRA |= _BV(ADSC);while(ADCSRA & _BV(ADSC));
  return ADCH;
}

void wait(uint16_t timing){
  TCNT1 = 0;
  while(TCNT1<timing);
}

void loop() {
	for(;;)
	{
      wait(5000);
      uint8_t val = aread(0);
      SerialSend(val);
	}
}


void TimerInit() {
	TCCR1A = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
}

//#define PORT_Send_ON PORT_Send |= BIT_Send
//#define PORT_Send_OFF PORT_Send &= ~BIT_Send
#define timing 200
#define PORT_Send_ON PORT_Send &= ~BIT_Send
#define PORT_Send_OFF PORT_Send |= BIT_Send

void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	PORT_Send_ON;
	TCCR1B = 3;
}

void SerialSend(uint8_t val){
	TCCR1B = 3;
	TCNT1 = 0;
	PORT_Send_OFF;while(TCNT1<timing);TCNT1=0;//startbit
	uint8_t chkbit = 0x01;
	//低bit先发送
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit){PORT_Send_ON;}else{PORT_Send_OFF;}chkbit<<=1;
		while(TCNT1<timing);TCNT1=0;
	}
	PORT_Send_ON;while(TCNT1<timing);TCNT1=0;
}


void xSerialSend(uint16_t val){
	TCCR1B = 3;
	TCNT1 = 0;
	PORT_Send_OFF;while(TCNT1<timing);TCNT1=0;//startbit
	uint8_t chkbit = 0x01;
	//低bit先发送
	for(uint8_t i = 10 ; i > 0 ; i--)
	{
		if(val&chkbit){PORT_Send_ON;}else{PORT_Send_OFF;}chkbit<<=1;
		while(TCNT1<timing);TCNT1=0;
	}
	PORT_Send_ON;while(TCNT1<timing);TCNT1=0;
}
