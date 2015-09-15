#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CUR_TIMING TIMING__8M_TCCR1B_1_115200
#define TCCR1B_Value 1

PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1_115200[] = {   69,  138,  208,  277,  347,  416,  486,  555,  625,  694};

#define DDR_Send DDRA
#define PORT_Send PORTA
#define BIT_Send _BV(7)
#define Send_INIT DDR_Send &= ~BIT_Send;PORT_Send &= ~BIT_Send;
#define Send_HI DDR_Send &= ~BIT_Send;
#define Send_LO DDR_Send |= BIT_Send;
void SerialSend(uint8_t val);
void SendInt(uint32_t val);
void SerialInit(){
	TCCR1A = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
	Send_INIT;
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

void SendInt(uint32_t val){
	uint32_t num = val;
	uint8_t idx;
	for(idx = 0; idx < 10 ; idx++)
	{
		uint8_t outNum = 0;
		uint32_t CmpNum = pgm_read_dword_near(num10s + idx);
		uint8_t i;
		for(i = 0; i < 10 ; i++)
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
		SerialSend('0' + outNum);
	}
}

void SerialSend(uint8_t val){
	cli();
	TCCR1B = TCCR1B_Value;
	TCNT1 = 0;
	uint16_t timing;
	prog_uint16_t* pTiming = CUR_TIMING;
	Send_LO;timing = pgm_read_word_near(pTiming++);while(TCNT1<timing);//startbit
	uint8_t chkbit = 0x01;
	uint8_t i;
	for(i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit){Send_HI;}else{Send_LO;}chkbit<<=1;timing = pgm_read_word_near(pTiming++);while(TCNT1<timing);
	}
	Send_HI;timing = pgm_read_word_near(pTiming);while(TCNT1<timing);
	sei();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//gnd
#define ledV    0 //led v--  out
#define senseIN 1 //sense in
#define ledIN   2 //led in
#define senseV  3 //sense v--
#define voltIN  4 //volt sense
#define voltV   5 //volt sense v--
//
//vcc
//b0
//b1
//rst
//b2
//a7 serial out
//a6





#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

void ClockInit();
uint16_t ARead(uint8_t pin);

int main(void) {
	ClockInit();
	//sei();
  
  SerialInit();
  SerialSend('a');SerialSend('a');SerialSend('a');SerialSend('a');SerialSend('a');SerialSend('a');SerialSend('a');SerialSend('a');SerialSend('a');
  while(1)
  {
    {
      DDRA |= _BV(senseV);
      uint16_t val = ARead(senseIN);
      DDRA &= ~_BV(senseV);
      SendInt(val);
      SerialSend(' ');SerialSend(' ');
    }
      
    {
      DDRA |= _BV(ledV);
      uint16_t val = ARead(ledIN);
      DDRA &= ~_BV(ledV);
      SendInt(val);
      SerialSend(' ');SerialSend(' ');
    }

    {
      DDRA |= _BV(voltV);
      uint16_t val = ARead(voltIN);
      DDRA &= ~_BV(voltV);
      SendInt(val);
      SerialSend(' ');SerialSend(' ');
    }
    SerialSend('\r');SerialSend('\n');
//  TCCR1A = 0;
//  TCCR1B = 5;//  1/64 31.25khz 488hz
//  TCCR1C = 0;
//  TIMSK1 = 0;
//    TCNT1 = 0;TIFR1 |= _BV(TOV1);//overflow flg reset
//    while(currTick<30)//~1s
//    {
//      ;
//    }
  }
}

void ClockInit() {
	CLKPR = _BV(CLKPCE);//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	CLKPR = 0;//8Mhz
	//CLKPR = _BV(CLKPS3);//1 0 0 0 1/256  31.25khz
}

uint16_t ARead(uint8_t pin)
{
return 12;
  ADCSRA |= _BV(ADEN);
  ADMUX = pin;

	ADCSRA |= _BV(ADSC);

	while (bit_is_set(ADCSRA, ADSC));

  ADCSRA &= ~_BV(ADEN);
	return ADC;
}

/*
ISR(INT0_vect){return;}
ISR(PCINT0_vect){return;}
ISR(PCINT1_vect){return;}
ISR(WDT_vect){return;}
ISR(TIMER1_CAPT_vect){return;}
ISR(TIM1_COMPA_vect){return;}
ISR(TIM1_COMPB_vect){return;}
ISR(TIM1_OVF_vect){return;}
ISR(TIM0_COMPA_vect){return;}
ISR(TIM0_COMPB_vect){return;}
ISR(TIM0_OVF_vect){return;}
ISR(ANA_COMP_vect){return;}
ISR(ADC_vect){return;}
ISR(EE_RDY_vect){return;}
ISR(USI_START_vect){return;}
ISR(USI_OVF_vect){return;}
*/