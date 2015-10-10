#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	
#define CUR_TIMING TIMING__8M_TCCR0B_1_115200
#define TCCR0B_Value 1

PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2___9600[] = {104,208, 56,160,  8,113,217, 65,169, 17};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2__14400[] = { 69,138,208, 21, 91,160,230, 43,113,182};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_1_115200[] = { 0, 69,138,208, 21, 91,160,230, 43,113,182};

#define DDR_Send DDRA
#define PORT_Send PORTA
#define BIT_Send _BV(2)
#define DDR_Recv DDRA
#define PIN_Recv PINA
#define BIT_Recv _BV(1)

volatile uint8_t SerialData;
volatile uint8_t SerialIdx = 0;


void ClockInit();
void loop();
void TimerInit();
void SerialInit();
void SerialSend(uint8_t val);
//uint8_t SerialRead();
//uint8_t SerialRead(uint16_t timeout, uint8_t* timoutParam);
void SendInt(uint32_t val);
//uint32_t ReadInt();
void StartSend(uint8_t val);

int main(void) {
	ClockInit();
	SerialInit();
	TimerInit();
	DDRA |= 1;
	loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

uint8_t buff[8];
void loop() {
	for(;;)
	{
		PINA = 1;
    while(SerialIdx);
    StartSend('a');
    
    for(uint32_t i = 0;i<40000;i++)
    {
      asm volatile("nop");
    }
	}
}


void TimerInit() {
	TCCR0A = 0;
	TIMSK0 = 0;
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


void StartListening(){
  GIMSK |= _BV(PCIE0);
  PCMSK0 |= BIT_Recv;//CLK
}
void StopListening(){
  GIMSK &= ~_BV(PCIE0);
  PCMSK0 &= ~BIT_Recv;//CLK
}
ISR(PCINT0_vect){
  if(SerialIdx)//发送中
  {
    return;
  }
  if(PIN_Recv&BIT_Recv)//相位不对
  {
    return;
  }
  StopListening();//关中断
  
  TCCR0B = TCCR0B_Value;
  TIMSK0 = 0;
  TCNT0 = 0;

  SerialData = 0;
  SerialIdx++;
  OCR0B = pgm_read_byte_near(CUR_TIMING+SerialIdx);
  TIMSK0 |= _BV(OCIE1B);
}

//接收
ISR(TIM0_COMPB_vect){
  SerialIdx++;
  OCR0A = pgm_read_byte_near(CUR_TIMING+SerialIdx);
  SerialData>>=1;
  SerialData |= (PIN_Recv&BIT_Recv?0x80:0);
  if(SerialIdx==10)
  {
    SerialIdx = 0;
    TIMSK0 &= ~_BV(OCIE1B);
    OCR0B = 0;
  }
}


void StartSend(uint8_t val){
  if(SerialIdx)
  {
    return;
  }
  TCCR0B = TCCR0B_Value;
  TIMSK0 = 0;
  
  SerialIdx = 0;

  PORT_Send &= ~BIT_Send;//startbit
  TCNT0 = 0;

  SerialData = val;
  SerialIdx++;
  OCR0A = pgm_read_byte_near(CUR_TIMING+SerialIdx);
  TIMSK0 |= _BV(OCIE1A);
}

//发送
ISR(TIM0_COMPA_vect){
  SerialIdx++;
  OCR0A = pgm_read_byte_near(CUR_TIMING+SerialIdx);
  if(SerialData&1)
  {
    PORT_Send |= BIT_Send;
  }else{
    PORT_Send &= ~BIT_Send;
  }
  SerialData>>=1;
  if(SerialIdx==10)
  {
    SerialIdx = 0;
    TIMSK0 &= ~_BV(OCIE1A);
    OCR0A = 0;
    PORT_Send |= BIT_Send;
  }
}


void SerialSend(uint8_t val)
{
	cli();
	TCCR0B = TCCR0B_Value;
	TCNT0 = 0;
	uint8_t timing;
	prog_uint8_t* pTiming = CUR_TIMING;
	PORT_Send &= ~BIT_Send;timing = pgm_read_byte_near(pTiming++);while(timing-TCNT0>0);//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 8 ; i > 0 ; i--)//先送低后送高
	{
		if(val&chkbit){PORT_Send |= BIT_Send;}else{PORT_Send &= ~BIT_Send;}chkbit<<=1;timing = pgm_read_byte_near(pTiming++);while(timing-TCNT0>0);
	}
	PORT_Send |= BIT_Send;timing = pgm_read_byte_near(pTiming);while(timing-TCNT0>0);
	sei();
}

/*
uint32_t ReadInt()
{
	uint32_t num = 0;
	uint8_t buff[10];
	uint8_t i;
	for(i=0;i<10;i++)
	{
		uint8_t data = SerialRead();
		if(data >= '0' && data <= '9')
		{
			buff[i] = data;
		}
		else
		{
			break;
		}
	}
	for(uint8_t j=0;j<i;j++)
	{
		num *= 10;
		num += buff[j] - '0';
	}
	return num;
}

uint8_t SerialRead()
{
	uint8_t timoutParam = 0;
	return SerialRead(65535, &timoutParam);
}

uint8_t SerialRead(uint16_t timeout, uint8_t* timoutParam)
{
	cli();
	uint8_t val = 0;
	TCCR1B = *timoutParam;//timeout param 1~5 : 1/(1~1024) max (0.008192 s ~ 8.388608 s)
	TCNT0 = 0;TIFR1 |= _BV(TOV1);
	while(PIN_Recv&BIT_Recv){
		if(((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)>=timeout)
		{
			*timoutParam = 0;
			return 0;
		}
	}
	TCCR0B = TCCR0B_Value;
	TCNT1 = 0;
	uint16_t timing;
	prog_uint16_t* pTiming = CUR_TIMING;

	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		timing = pgm_read_word_near(pTiming++);while(timing-TCNT0<128);
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);
	}
	sei();
	return val;
}
*/