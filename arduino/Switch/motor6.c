#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define STAOn   DDRB |= _BV(3) ;/**/
#define STAOff  DDRB &= ~_BV(3);/**/
uint8_t i = 0;

#define CUR_TIMING TIMING__8M_TCCR1B_1___9600
#define TCCR1B_Value 1
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1___9600[] = {  833, 1666, 2500, 3333, 4166, 5000, 5833, 6666, 7500, 8333};

#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(1)
#define DDR_Recv DDRB
#define PIN_Recv PINB
#define BIT_Recv _BV(0)

#define CMD_ON1 1
#define CMD_ON2 2
#define CMD_ON3 3
#define CMD_ON4 4
#define CMD_OFF1 5
#define CMD_OFF2 6
#define CMD_OFF3 7
#define CMD_OFF4 8
#define CMD_TIM1_1S 9
#define CMD_TIM1_10S 10
#define CMD_TIM1_1M 11
#define CMD_TIM1_1H 12
#define CMD_TIM1_2H 13
#define CMD_TIM1_3H 14
#define CMD_TIM1_4H 15
#define CMD_TIM1_5H 16

    
void loop();
void TimerInit();
void SerialInit();
uint8_t SerialRead();

volatile uint32_t timer;

int main(void) {
	CLKPR = 128;CLKPR = 0;
  SerialInit();
  TimerInit();
  MCUCR |= _BV(PUD);
  DDRA = 0x0F;
  timer = 0;
	sei();
  loop();
}

void loop() {
	for(;;)
	{
    uint8_t data = SerialRead();
    switch(data){
      case CMD_ON1:
        PORTA |= _BV(0);STAOn;
        break;
      case CMD_ON2:
        PORTA |= _BV(1);STAOn;
        break;
      case CMD_ON3:
        PORTA |= _BV(2);STAOn;
        break;
      case CMD_ON4:
        PORTA |= _BV(3);STAOn;
        break;
      case CMD_OFF1:
        PORTA &= ~_BV(0);STAOff;
        break;
      case CMD_OFF2:
        PORTA &= ~_BV(1);STAOff;
        break;
      case CMD_OFF3:
        PORTA &= ~_BV(2);STAOff;
        break;
      case CMD_OFF4:
        PORTA &= ~_BV(3);STAOff;
        break;
      case CMD_TIM1_1S:
        PORTA |= _BV(0);STAOn;timer = 30;
        break;
      case CMD_TIM1_10S:
        PORTA |= _BV(0);STAOn;timer = 305;
        break;
      case CMD_TIM1_1M:
        PORTA |= _BV(0);STAOn;timer = 1831;
        break;
      case CMD_TIM1_1H:
        PORTA |= _BV(0);STAOn;timer = 109863;
        break;
      case CMD_TIM1_2H:
        PORTA |= _BV(0);STAOn;timer = 219726;
        break;
      case CMD_TIM1_3H:
        PORTA |= _BV(0);STAOn;timer = 329589;
        break;
      case CMD_TIM1_4H:
        PORTA |= _BV(0);STAOn;timer = 439453;
        break;
      case CMD_TIM1_5H:
        PORTA |= _BV(0);STAOn;timer = 549316;
        break;
    }
	}
}

void TimerInit() {
	TCCR1A = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
  
  TCCR0A = 0;
  TCCR0B = 5;//1 0 1 clkI/O/1024 (From prescaler)  8000000/1024 7812.5/256 =  30.517578125/s
  TIMSK0 = _BV(TOIE0);
}

ISR(TIM0_OVF_vect){
  if(timer!=0){
    timer--;
    if(timer==0){
      PORTA &= ~_BV(0);STAOff;
    }
  }
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

