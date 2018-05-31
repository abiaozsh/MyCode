#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	
#define CUR_TIMING TIMING__8M_TCCR1B_1___9600
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

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)


#define BIT_Send _BV(6)

void ClockInit();
void loop();
void TimerInit();
void SerialInit();
void SerialSend(uint8_t val);
void SendInt(uint32_t val);
uint8_t SerialRead();
uint8_t SerialRead(uint16_t timeout, uint8_t* timoutParam);
uint32_t ReadInt();

void wait(uint16_t timing){
	for(uint16_t i=0;i<timing;i++){
		TCNT1 = 0;
		while(TCNT1<8000);//startbit  8000/64
	}
}


uint8_t readAnalog(uint8_t pin_) {
	ADMUX = pin_;//A7 power

	ADCSRA |= _BV(ADSC);

	while(ADCSRA & _BV(ADSC));

	ADCSRA |= _BV(ADSC);
	
	while(ADCSRA & _BV(ADSC));
	
	return ADCH;
}

int main(void) {
	ClockInit();
	DDRA = 0;
	DDRB = _BV(0);
	PORTB = 0;
	SerialInit();
	TimerInit();
	DIDR0 = _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(4);
	ADCSRA = _BV(ADEN) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
	ADCSRB = _BV(ADLAR);
	TCCR1B = TCCR1B_Value;
	wait(1000);//1s = 8000000

	loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

uint8_t vals[48];
uint8_t valIdx = 0;

void teston(){
	vals[valIdx+0] = readAnalog(0);//valon0
	vals[valIdx+1] = readAnalog(1);//valon1
	vals[valIdx+2] = readAnalog(2);//valon2
	//valon3 = readAnalog(3);
	//valon4 = readAnalog(4);
}
void testoff(){
	vals[valIdx+3] = readAnalog(0);//valoff0
	vals[valIdx+4] = readAnalog(1);//valoff1
	vals[valIdx+5] = readAnalog(2);//valoff2
	//valoff3 = readAnalog(3);
	//valoff4 = readAnalog(4);
}

void alert1(){
	DDRB |= _BV(1);
	PORTB |= _BV(1);
	while(true);
}
void alert2(){
	DDRB |= _BV(2);
	PORTB |= _BV(2);
  while(true){
    uint8_t valIdx2 = valIdx;
    for(uint8_t i = 0;i<8;i++){

      SerialSend('0'+i);SerialSend(',');
			SendInt(vals[valIdx2+0]);SerialSend(',');
			SendInt(vals[valIdx2+1]);SerialSend(',');
			SendInt(vals[valIdx2+2]);SerialSend(',');
			SendInt(vals[valIdx2+3]);SerialSend(',');
			SendInt(vals[valIdx2+4]);SerialSend(',');
			SendInt(vals[valIdx2+5]);SerialSend(',');
			SerialSend('\r');
			SerialSend('\n');
      
      
			valIdx2+=6;
			if(valIdx2==48){
        valIdx2 = 0;
      }
    }
			wait(5000);
	}
}

uint8_t state = 0;
uint8_t errCount = 0;

void loop() {
	//PORTB |= _BV(0);while(true);
	//for(;;)
	//{
	//	SerialSend('a');
	//	SerialSend('\r');
	//	SerialSend('\n');
	//	wait(1000);
	//}
	for(;;)
	{
		uint8_t read5 = readAnalog(5);
		
		if(state)
		{
			PORTB |= _BV(0);wait(30);
			teston();
			PORTB &= ~_BV(0);wait(30);
			testoff();
//			SendInt(state);SerialSend(',');
//			SerialSend('\r');
//			SerialSend('\n');
//			SendInt(valon0);SerialSend(',');
//			SendInt(valon1);SerialSend(',');
//			SendInt(valon2);SerialSend(',');
//			//SendInt(valon3);SerialSend(',');
//			//SendInt(valon4);SerialSend(',');
//			SerialSend('\r');
//			SerialSend('\n');
//			SendInt(valoff0);SerialSend(',');
//			SendInt(valoff1);SerialSend(',');
//			SendInt(valoff2);SerialSend(',');
//			//SendInt(valoff3);SerialSend(',');
//			//SendInt(valoff4);SerialSend(',');
//			SerialSend('\r');
//			SerialSend('\n');
//			wait(500);
			
			
			uint8_t flg = 0;
      //vals[valIdx+0] = readAnalog(0);//valon0
      //vals[valIdx+1] = readAnalog(1);//valon1
      //vals[valIdx+2] = readAnalog(2);//valon2
      //vals[valIdx+3] = readAnalog(0);//valoff0
      //vals[valIdx+4] = readAnalog(1);//valoff1
      //vals[valIdx+5] = readAnalog(2);//valoff2
			if(vals[valIdx+3]<vals[valIdx+0] || vals[valIdx+3]-vals[valIdx+0]<70)flg=1;
			if(vals[valIdx+4]<vals[valIdx+1] || vals[valIdx+4]-vals[valIdx+1]<70)flg=1;
			if(vals[valIdx+5]<vals[valIdx+2] || vals[valIdx+5]-vals[valIdx+2]<70)flg=1;
			//if(valoff3-valon3<100)alert();
			//if(valoff4-valon4<100)alert();
			if(flg){
				errCount++;
			}else
			{
				errCount = 0;
			}
      
			if(state>1)
			{
				state--;
				if(errCount>=5)alert1();//小警报
			}
			else
			{
				if(errCount>=5)alert2();//大警报
			}
			
			if(read5<160){//数字越大越暗
				state = 0;//alarm off
			}
		
			valIdx+=6;
			if(valIdx==48){
				valIdx = 0;
			}
		}
		else
		{
//			SendInt(state);SerialSend(',');
//			SerialSend('\r');
//			SerialSend('\n');
			
			if(read5>170){//数字越大越暗
				state = 50;//alarm on
			}
			wait(500);
			//unalert();
		}
		
		
	}
}

void TimerInit() {
	TCCR1A = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
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
		SerialSend('0' + outNum);
	}
}

#define DDR_Send DDRA
#define PORT_Send PORTA
//#define SEND_ON PORT_Send |= BIT_Send
//#define SEND_OFF PORT_Send &= ~BIT_Send
#define SEND_ON DDR_Send |= BIT_Send
#define SEND_OFF DDR_Send &= ~BIT_Send

void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	//DDR_Send |= BIT_Send;
	PORT_Send &= ~BIT_Send;
	SEND_OFF;
}

void SerialSend(uint8_t val){
	//cli();
	
	TCCR1B = TCCR1B_Value;
	TCNT1 = 0;
	uint16_t timing;
	prog_uint16_t* pTiming = CUR_TIMING;
	SEND_ON;timing = pgm_read_word_near(pTiming++);while(TCNT1<timing);//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit){SEND_OFF;}else{SEND_ON;}chkbit<<=1;timing = pgm_read_word_near(pTiming++);while(TCNT1<timing);
	}
	SEND_OFF;timing = pgm_read_word_near(pTiming);while(TCNT1<timing);
	//sei();
}
