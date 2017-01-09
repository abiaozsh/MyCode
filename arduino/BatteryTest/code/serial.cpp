#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

PROGMEM prog_uint8_t Voltage[] = {   
0,0,//0,0V 
2,48,//1,0.0390625V 
0,50,//2,0.078125V 
32,114,//3,0.1171875V 
72,18,//4,0.15625V 
66,114,//5,0.1953125V 
0,84,//6,0.234375V 
0,22,//7,0.2734375V 
2,118,//8,0.3125V 
4,2,//9,0.3515625V 
4,50,//10,0.390625V 
0,76,//11,0.4296875V 
0,14,//12,0.46875V 
2,94,//13,0.5078125V 
1,114,//14,0.546875V 
4,22,//15,0.5859375V 
6,122,//16,0.625V 
0,132,//17,0.6640625V 
0,180,//18,0.703125V 
8,214,//19,0.7421875V 
4,14,//20,0.78125V 
6,94,//21,0.8203125V 
4,242,//22,0.859375V 
0,156,//23,0.8984375V 
2,252,//24,0.9375V 
0,254,//25,0.9765625V 
4,180,//26,1.015625V 
12,214,//27,1.0546875V 
37,122,//28,1.09375V 
3,200,//29,1.1328125V 
1,198,//30,1.171875V 
4,156,//31,1.2109375V 
6,252,//32,1.25V 
4,254,//33,1.2890625V 
15,242,//34,1.328125V 
9,220,//35,1.3671875V 
1,222,//36,1.40625V 
7,196,//37,1.4453125V 
5,198,//38,1.484375V 
5,246,//39,1.5234375V 
20,86,//40,1.5625V 
60,54,//41,1.6015625V 
5,140,//42,1.640625V 
13,220,//43,1.6796875V 
5,222,//44,1.71875V 
20,78,//45,1.7578125V 
20,126,//46,1.796875V 
30,242,//47,1.8359375V 
16,220,//48,1.875V 
17,130,//49,1.9140625V 
17,178,//50,1.953125V 
20,244,//51,1.9921875V 
60,150,//52,2.03125V 
62,246,//53,2.0703125V 
0,81,//54,2.109375V 
8,19,//55,2.1484375V 
2,115,//56,2.1875V 
21,130,//57,2.2265625V 
21,178,//58,2.265625V 
0,69,//59,2.3046875V 
8,7,//60,2.34375V 
0,55,//61,2.3828125V 
4,81,//62,2.421875V 
12,19,//63,2.4609375V 
6,115,//64,2.5V 
8,129,//65,2.5390625V 
0,31,//66,2.578125V 
2,127,//67,2.6171875V 
12,7,//68,2.65625V 
4,55,//69,2.6953125V 
38,119,//70,2.734375V 
0,149,//71,2.7734375V 
2,245,//72,2.8125V 
0,247,//73,2.8515625V 
4,31,//74,2.890625V 
6,127,//75,2.9296875V 
0,141,//76,2.96875V 
2,221,//77,3.0078125V 
0,223,//78,3.046875V 
4,149,//79,3.0859375V 
6,245,//80,3.125V 
4,247,//81,3.1640625V 
76,199,//82,3.203125V 
9,213,//83,3.2421875V 
3,151,//84,3.28125V 
4,189,//85,3.3203125V 
5,195,//86,3.359375V 
13,179,//87,3.3984375V 
9,205,//88,3.4375V 
1,253,//89,3.4765625V 
9,191,//90,3.515625V 
7,149,//91,3.5546875V 
13,151,//92,3.59375V 
15,247,//93,3.6328125V 
28,119,//94,3.671875V 
71,247,//95,3.7109375V 
7,141,//96,3.75V 
5,253,//97,3.7890625V 
5,191,//98,3.828125V 
20,31,//99,3.8671875V 
22,211,//100,3.90625V 
25,77,//101,3.9453125V 
16,143,//102,3.984375V 
26,223,//103,4.0234375V 
17,243,//104,4.0625V 
22,199,//105,4.1015625V 
23,123,//106,4.140625V 
25,133,//107,4.1796875V 
27,213,//108,4.21875V 
17,215,//109,4.2578125V 
21,61,//110,4.296875V 
20,223,//111,4.3359375V 
23,179,//112,4.375V 
27,205,//113,4.4140625V 
17,253,//114,4.453125V 
17,191,//115,4.4921875V 
21,213,//116,4.53125V 
23,151,//117,4.5703125V 
61,183,//118,4.609375V 
87,245,//119,4.6484375V 
85,247,//120,4.6875V 
21,205,//121,4.7265625V 
23,189,//122,4.765625V 
21,191,//123,4.8046875V 
61,255,//124,4.84375V 
117,143,//125,4.8828125V 
95,255,//126,4.921875V 
215,255,//127,4.9609375V 
};

#define DDR_STR_ON   DDRA  |=  _BV(6)
#define PORT_STR_ON  PORTA |=  _BV(6)
#define PORT_STR_OFF PORTA &= ~_BV(6)

#define DDR_DAT_ON   DDRA  |=  _BV(7)
#define PORT_DAT_ON  PORTA |=  _BV(7)
#define PORT_DAT_OFF PORTA &= ~_BV(7)

#define DDR_OE_ON    DDRB  |=  _BV(2)
#define PORT_OE_ON   PORTB |=  _BV(2)
#define PORT_OE_OFF  PORTB &= ~_BV(2)


#define DDR_CLK_ON   DDRA  |=  _BV(5)
#define PORT_CLK_ON  PORTA |=  _BV(5)
#define PORT_CLK_OFF PORTA &= ~_BV(5)

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
void setVoltage(uint16_t idx){
  SendByte(pgm_read_byte_near(Voltage+(idx<<1)));
  SendByte(pgm_read_byte_near(Voltage+(idx<<1)+1));
  PORT_STR_ON;
  PORT_STR_OFF;
}

void wait(uint32_t n){
	{
		uint32_t i = 0;
		for(i=0;i<n;i++)
		{
			asm volatile("nop");
		}
	}
  
}


uint16_t getAnalog(){//0~1024
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);//ADATE ADIF ADIE
	DIDR0 |= _BV(ADC6D);
	ADCSRB = 0;

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
  return (uint16_t)val;
}
uint16_t getInitVal(){
	ADMUX = 4;
  getAnalog();
}
uint16_t getCurrent(){
	ADMUX = 3;
  ADMUX |= _BV(REFS1);
  getAnalog();
}
uint16_t getVoltage(){
	ADMUX = 2;
  getAnalog();
}
uint16_t getVoltage10(){
	ADMUX = 1;
  getAnalog();
}


void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
	TCCR1A = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
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



int main(void) {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
	SerialInit();
	DDR_DAT_ON;
	DDR_CLK_ON;
	DDR_STR_ON;
	DDR_OE_ON ;
	PORT_OE_OFF;

	wait(1000000);
  
	{
		uint16_t i = 0;
		uint16_t j = 0;
    uint16_t volt = 0;
		for(;;)
		{
      //setVoltage(volt);
      //volt++;
      //if(volt==128){
      //  volt = 0;
      //}
      
      uint16_t val;
      val = getInitVal();
      setVoltage(val);
      SerialSend('i');SerialSend(':');SendInt(val,5);SerialSend('\t');
      val = getCurrent();
      SerialSend('c');SerialSend(':');SendInt(val,5);SerialSend('\t');
      val = getVoltage();
      SerialSend('v');SerialSend(':');SendInt(val,5);SerialSend('\t');
      val = getVoltage10();
      SerialSend('1');SerialSend(':');SendInt(val,5);SerialSend('\t');
      val = volt;
      SerialSend('x');SerialSend(':');SendInt(val,5);SerialSend('\t');
      
      
      
      
      SerialSend('\r');
      SerialSend('\n');
      
      wait(1000000);
      
		}
	}
}
