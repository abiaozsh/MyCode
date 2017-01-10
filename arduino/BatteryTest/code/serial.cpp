#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

PROGMEM prog_uint8_t Voltage[] = {   
17,178,//0,1.953125V 
19,178,//1,1.962890625V 
19,242,//2,1.97265625V 
20,180,//3,1.982421875V 
20,244,//4,1.9921875V 
22,198,//5,2.001953125V 
52,244,//6,2.01171875V 
23,106,//7,2.021484375V 
60,150,//8,2.03125V 
60,214,//9,2.041015625V 
60,182,//10,2.05078125V 
60,246,//11,2.060546875V 
62,246,//12,2.0703125V 
0,1,//13,2.080078125V 
0,65,//14,2.08984375V 
0,17,//15,2.099609375V 
0,81,//16,2.109375V 
2,81,//17,2.119140625V 
2,49,//18,2.12890625V 
8,67,//19,2.138671875V 
8,19,//20,2.1484375V 
8,83,//21,2.158203125V 
0,51,//22,2.16796875V 
0,115,//23,2.177734375V 
2,115,//24,2.1875V 
20,188,//25,2.197265625V 
20,252,//26,2.20703125V 
22,206,//27,2.216796875V 
21,130,//28,2.2265625V 
21,194,//29,2.236328125V 
21,146,//30,2.24609375V 
21,210,//31,2.255859375V 
21,178,//32,2.265625V 
21,242,//33,2.275390625V 
23,242,//34,2.28515625V 
0,5,//35,2.294921875V 
0,69,//36,2.3046875V 
0,21,//37,2.314453125V 
0,85,//38,2.32421875V 
2,85,//39,2.333984375V 
8,7,//40,2.34375V 
8,71,//41,2.353515625V 
0,23,//42,2.36328125V 
0,87,//43,2.373046875V 
0,55,//44,2.3828125V 
0,119,//45,2.392578125V 
2,119,//46,2.40234375V 
4,17,//47,2.412109375V 
4,81,//48,2.421875V 
6,81,//49,2.431640625V 
6,49,//50,2.44140625V 
6,113,//51,2.451171875V 
12,19,//52,2.4609375V 
6,19,//53,2.470703125V 
4,51,//54,2.48046875V 
4,115,//55,2.490234375V 
6,115,//56,2.5V 
0,13,//57,2.509765625V 
0,77,//58,2.51953125V 
2,77,//59,2.529296875V 
8,129,//60,2.5390625V 
8,193,//61,2.548828125V 
0,15,//62,2.55859375V 
0,79,//63,2.568359375V 
0,31,//64,2.578125V 
0,95,//65,2.587890625V 
0,63,//66,2.59765625V 
0,127,//67,2.607421875V 
2,127,//68,2.6171875V 
1,51,//69,2.626953125V 
4,101,//70,2.63671875V 
10,243,//71,2.646484375V 
12,7,//72,2.65625V 
12,71,//73,2.666015625V 
4,23,//74,2.67578125V 
4,87,//75,2.685546875V 
4,55,//76,2.6953125V 
4,119,//77,2.705078125V 
6,119,//78,2.71484375V 
36,119,//79,2.724609375V 
38,119,//80,2.734375V 
68,71,//81,2.744140625V 
1,5,//82,2.75390625V 
0,197,//83,2.763671875V 
0,149,//84,2.7734375V 
0,213,//85,2.783203125V 
0,181,//86,2.79296875V 
0,245,//87,2.802734375V 
2,245,//88,2.8125V 
4,13,//89,2.822265625V 
4,77,//90,2.83203125V 
0,183,//91,2.841796875V 
0,247,//92,2.8515625V 
10,247,//93,2.861328125V 
4,15,//94,2.87109375V 
4,79,//95,2.880859375V 
4,31,//96,2.890625V 
4,95,//97,2.900390625V 
4,63,//98,2.91015625V 
4,127,//99,2.919921875V 
6,127,//100,2.9296875V 
12,179,//101,2.939453125V 
12,243,//102,2.94921875V 
14,243,//103,2.958984375V 
0,141,//104,2.96875V 
0,205,//105,2.978515625V 
0,157,//106,2.98828125V 
0,221,//107,2.998046875V 
2,221,//108,3.0078125V 
2,189,//109,3.017578125V 
2,253,//110,3.02734375V 
0,159,//111,3.037109375V 
0,223,//112,3.046875V 
0,191,//113,3.056640625V 
0,255,//114,3.06640625V 
2,255,//115,3.076171875V 
4,149,//116,3.0859375V 
4,213,//117,3.095703125V 
4,181,//118,3.10546875V 
4,245,//119,3.115234375V 
6,245,//120,3.125V 
12,151,//121,3.134765625V 
12,215,//122,3.14453125V 
4,187,//123,3.154296875V 
4,247,//124,3.1640625V 
6,251,//125,3.173828125V 
68,181,//126,3.18359375V 
68,249,//127,3.193359375V 
76,199,//128,3.203125V 
9,133,//129,3.212890625V 
9,197,//130,3.22265625V 
9,149,//131,3.232421875V 
9,213,//132,3.2421875V 
1,135,//133,3.251953125V 
1,199,//134,3.26171875V 
3,199,//135,3.271484375V 
3,151,//136,3.28125V 
3,215,//137,3.291015625V 
4,157,//138,3.30078125V 
4,221,//139,3.310546875V 
4,189,//140,3.3203125V 
4,253,//141,3.330078125V 
6,253,//142,3.33984375V 
5,131,//143,3.349609375V 
5,195,//144,3.359375V 
4,191,//145,3.369140625V 
4,255,//146,3.37890625V 
6,255,//147,3.388671875V 
13,179,//148,3.3984375V 
13,243,//149,3.408203125V 
15,243,//150,3.41796875V 
9,141,//151,3.427734375V 
9,205,//152,3.4375V 
1,157,//153,3.447265625V 
1,221,//154,3.45703125V 
1,189,//155,3.466796875V 
1,253,//156,3.4765625V 
3,207,//157,3.486328125V 
9,159,//158,3.49609375V 
9,223,//159,3.505859375V 
9,191,//160,3.515625V 
9,255,//161,3.525390625V 
11,255,//162,3.53515625V 
13,149,//163,3.544921875V 
7,149,//164,3.5546875V 
5,135,//165,3.564453125V 
5,199,//166,3.57421875V 
7,203,//167,3.583984375V 
13,151,//168,3.59375V 
13,215,//169,3.603515625V 
13,183,//170,3.61328125V 
13,247,//171,3.623046875V 
15,247,//172,3.6328125V 
22,71,//173,3.642578125V 
22,39,//174,3.65234375V 
22,87,//175,3.662109375V 
28,119,//176,3.671875V 
30,119,//177,3.681640625V 
52,119,//178,3.69140625V 
54,119,//179,3.701171875V 
71,247,//180,3.7109375V 
24,133,//181,3.720703125V 
24,197,//182,3.73046875V 
13,141,//183,3.740234375V 
7,141,//184,3.75V 
5,157,//185,3.759765625V 
5,221,//186,3.76953125V 
5,189,//187,3.779296875V 
5,253,//188,3.7890625V 
7,253,//189,3.798828125V 
13,159,//190,3.80859375V 
13,223,//191,3.818359375V 
5,191,//192,3.828125V 
5,255,//193,3.837890625V 
7,255,//194,3.84765625V 
22,79,//195,3.857421875V 
20,31,//196,3.8671875V 
20,195,//197,3.876953125V 
20,63,//198,3.88671875V 
20,127,//199,3.896484375V 
22,211,//200,3.90625V 
22,179,//201,3.916015625V 
22,243,//202,3.92578125V 
25,13,//203,3.935546875V 
25,77,//204,3.9453125V 
26,205,//205,3.955078125V 
16,157,//206,3.96484375V 
16,221,//207,3.974609375V 
16,143,//208,3.984375V 
16,253,//209,3.994140625V 
18,253,//210,4.00390625V 
17,131,//211,4.013671875V 
26,223,//212,4.0234375V 
17,147,//213,4.033203125V 
17,211,//214,4.04296875V 
17,179,//215,4.052734375V 
17,243,//216,4.0625V 
19,243,//217,4.072265625V 
20,135,//218,4.08203125V 
20,199,//219,4.091796875V 
22,199,//220,4.1015625V 
22,151,//221,4.111328125V 
22,215,//222,4.12109375V 
28,247,//223,4.130859375V 
23,123,//224,4.140625V 
52,247,//225,4.150390625V 
54,247,//226,4.16015625V 
84,135,//227,4.169921875V 
25,133,//228,4.1796875V 
25,197,//229,4.189453125V 
19,201,//230,4.19921875V 
25,217,//231,4.208984375V 
27,213,//232,4.21875V 
17,135,//233,4.228515625V 
17,199,//234,4.23828125V 
17,151,//235,4.248046875V 
17,215,//236,4.2578125V 
19,219,//237,4.267578125V 
20,157,//238,4.27734375V 
20,221,//239,4.287109375V 
21,61,//240,4.296875V 
22,189,//241,4.306640625V 
22,207,//242,4.31640625V 
20,175,//243,4.326171875V 
20,223,//244,4.3359375V 
20,191,//245,4.345703125V 
20,255,//246,4.35546875V 
23,211,//247,4.365234375V 
23,179,//248,4.375V 
23,243,//249,4.384765625V 
53,243,//250,4.39453125V 
27,141,//251,4.404296875V 
27,205,//252,4.4140625V 
17,157,//253,4.423828125V 
17,221,//254,4.43359375V 
17,189,//255,4.443359375V 
17,253,//256,4.453125V 
19,253,//257,4.462890625V 
25,239,//258,4.47265625V 
27,223,//259,4.482421875V 
17,191,//260,4.4921875V 
51,223,//261,4.501953125V 
57,191,//262,4.51171875V 
21,149,//263,4.521484375V 
21,213,//264,4.53125V 
21,139,//265,4.541015625V 
21,203,//266,4.55078125V 
23,199,//267,4.560546875V 
23,151,//268,4.5703125V 
23,219,//269,4.580078125V 
61,151,//270,4.58984375V 
61,215,//271,4.599609375V 
61,183,//272,4.609375V 
61,247,//273,4.619140625V 
63,247,//274,4.62890625V 
87,181,//275,4.638671875V 
87,245,//276,4.6484375V 
85,151,//277,4.658203125V 
85,215,//278,4.66796875V 
85,183,//279,4.677734375V 
85,247,//280,4.6875V 
87,247,//281,4.697265625V 
125,183,//282,4.70703125V 
21,141,//283,4.716796875V 
21,205,//284,4.7265625V 
21,157,//285,4.736328125V 
21,221,//286,4.74609375V 
23,221,//287,4.755859375V 
23,189,//288,4.765625V 
23,253,//289,4.775390625V 
21,159,//290,4.78515625V 
21,223,//291,4.794921875V 
21,191,//292,4.8046875V 
21,255,//293,4.814453125V 
23,255,//294,4.82421875V 
53,255,//295,4.833984375V 
61,255,//296,4.84375V 
85,143,//297,4.853515625V 
85,207,//298,4.86328125V 
87,207,//299,4.873046875V 
117,143,//300,4.8828125V 
117,207,//301,4.892578125V 
93,191,//302,4.90234375V 
93,255,//303,4.912109375V 
95,255,//304,4.921875V 
125,191,//305,4.931640625V 
125,255,//306,4.94140625V 
127,255,//307,4.951171875V 
215,255,//308,4.9609375V 
245,191,//309,4.970703125V 
245,255,//310,4.98046875V 
247,255,//311,4.990234375V 
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
			SerialSend('0' + outNum);
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
  DDR_Recv &= ~BIT_Recv;
  sendData(0, 0);
  uint16_t cmax;
  uint16_t cmin;
  
  //开始
  while(PIN_Recv & BIT_Recv){
      uint16_t val;
      val = getInitVal();
      cmax = val;
      SerialSend('i');SerialSend(':');SendInt(val>>1,5);SerialSend('\r');SerialSend('\n');
      wait(10000);
  }
  cmax = cmax>>1;//0~512
  cmin = cmax>>3;//
  cmin = cmax - cmin;
  if(cmin>2)
  {
    cmin--;
  }
  
	{
    uint16_t mosVolt = 0;//0~311  2v-5v
		uint8_t started = 0;
		for(;;)
		{
      uint16_t c = getCurrent();
      SerialSend('c');SerialSend(':');SendInt(c,5);SerialSend('\t');
      uint16_t v = getVoltage();
      SerialSend('v');SerialSend(':');SendInt(v,5);SerialSend('\t');
      
      if(c<cmin){
        if(mosVolt<311){
          mosVolt++;
        }
      }
			else{
				started = 1;
			}
	  
	  
      if(c>cmax){
        if(mosVolt>0){
          mosVolt--;
        }
      }

      SerialSend('m');SerialSend(':');SendInt(mosVolt,5);SerialSend('\t');
      SerialSend('u');SerialSend(':');SendInt(cmax,5);SerialSend('\t');
      SerialSend('d');SerialSend(':');SendInt(cmin,5);SerialSend('\t');

      SerialSend('\r');SerialSend('\n');
      
      setVoltage(mosVolt);
      
      
      if(v<614){
        sendData(0, 0);
        SerialSend('S');SerialSend('T');SerialSend('O');SerialSend('P');SerialSend('\r');SerialSend('\n');
        for(;;);
      }
      
      //val = getVoltage10();
      //SerialSend('1');SerialSend(':');SendInt(val,5);SerialSend('\t');
			if(started){
				wait(1000000);
			}
			else
			{
				wait(20000);
			}
		}
	}
}
