#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	
#define CUR_TIMING TIMING__8M_TCCR1B_1___9600
#define TCCR1B_Value 1
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1_115200[] = {   69,  138,  208,  277,  347,  416,  486,  555,  625,  694};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1___9600[] = {  833, 1666, 2500, 3333, 4166, 5000, 5833, 6666, 7500, 8333};

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
	sei();
	loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

volatile int32_t lastValue = 0;
volatile int32_t currValue = 0;
volatile uint16_t cnt = 0;
volatile uint8_t flg = 0;

ISR(ADC_vect){
  uint16_t v = ADC;
  if(v>=512){
    v = 1024 - v;
    currValue -= v;
  }
  else
  {
    currValue += v;
  }
  cnt++;
  if(cnt==1024){
    lastValue = currValue;
    currValue = 0;
    flg = 1;
		cnt = 0;
		DDRB ^= _BV(3);
  }
}


void loop() {
	ADCSRA = _BV(ADATE) | _BV(ADIE) | _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);//ADATE ADIF 
	DIDR0 |= _BV(ADC0D) | _BV(ADC1D);
  
  //ADC0 (PA0) ADC1 (PA1)  Gain 1x Gain 20x  001000 001001
  //REFS1(1) REFS0(0) Internal 1.1V voltage reference
  ADMUX = 0;
  ADMUX |= _BV(REFS1);
  ADMUX |= 0x09;//Gain 20x 001001
  //ADMUX |= 0x08;//Gain 1x 001000

  ADCSRB = 0;
  ADCSRB |= _BV(BIN);
  
  ADCSRA |= _BV(ADSC);
  
	for(;;)
	{
    while(!flg);
    flg = 0;
/*    
0.01Ω
 10A 0.1v 1W
 30A 0.3V 9w

1=1.1/20 /1024 *100 = 0.00537109375A

1023 = 1.1/20  *100 = 5.5A    
*/
    //int32_t CURR = lastValue/1024 * 1.1/20 /1024 *100 A *1000 ma
    
    int32_t CURR = lastValue*5500;
    CURR = CURR >> 19;//    /1024/1024    A  ma     *2
    
    if(CURR<0){
      CURR = - CURR;
      SerialSend('-');
    }
    else
    {
      SerialSend(' ');
    }
    
    
    SendInt(CURR,10);SerialSend(' ');

		SerialSend('\r');
		SerialSend('\n');

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
			SerialSend('0' + outNum);
		}
	}
}

void SerialSend(uint8_t val){
	//cli();
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
	//sei();
}
