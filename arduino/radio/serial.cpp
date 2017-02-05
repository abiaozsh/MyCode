#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	
#define CUR_TIMING TIMING__8M_TCCR1B_1_115200
#define TCCR1B_Value 1

PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1_115200[] = {   69,  138,  208,  277,  347,  416,  486,  555,  625,  694};

#define currTick ((TIFR0 & _BV(TOV0))?0x0FF:TCNT0)

#define DDR_SendRadio DDRA
#define PORT_SendRadio PORTA
#define BIT_SendRadio _BV(4)
#define SendRadioLo PORT_SendRadio &= ~BIT_SendRadio;
#define SendRadioHi PORT_SendRadio |= BIT_SendRadio;

#define DDR_RecvRadio DDRA
#define PIN_RecvRadio PINA
#define BIT_RecvRadio _BV(5)
#define Data_RecvRadio (PIN_RecvRadio&BIT_RecvRadio)


#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(0)
#define DDR_Recv DDRB
#define PIN_Recv PINB
#define BIT_Recv _BV(1)


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

uint8_t SerialRead()
{
	cli();
	uint8_t val = 0;
	while(PIN_Recv&BIT_Recv){
	}
	TCCR1B = TCCR1B_Value;
	TCNT1 = 0;
	uint16_t timing;
	prog_uint16_t* pTiming = CUR_TIMING;

	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		timing = pgm_read_word_near(pTiming++);while(TCNT1<timing);
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);
	}
	sei();
	return val;
}


void RadioSend(uint8_t val)
{
	cli();
  uint8_t chkbit = 0x01;
	TCCR0B = 3;//2 //1
  //CS02 CS01 CS00 Description
  //0 0 0 No clock source (Timer/Counter stopped)
  //0 0 1 clkI/O/(No prescaling)    8000000 / 160 = 50000 hz
  //0 1 0 clkI/O/8 (From prescaler) 8000000 / 160 / 8 = 6250 hz
  //0 1 1 clkI/O/64 (From prescaler)  8000000 / 160 / 64 = 781.25 hz
  //1 0 0 clkI/O/256 (From prescaler)
  //1 0 1 clkI/O/1024 (From prescaler)
	TCNT0 = 0;
  SendRadioLo;//低 初始
  while(TCNT0<80);
  SendRadioHi;
  while(TCNT0<160);//80 高
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
    TCNT0 = 0;
    SendRadioLo;
    if(val&chkbit){
      while(TCNT0<80); //短 1
    }else{
      while(TCNT0<160); //长 0
    }
    SendRadioHi;
    while(TCNT0<240);
		chkbit<<=1;
	}
	sei();
}

uint8_t RadioRead()
{
	cli();
	TCCR0B = 3;//2 //1
  uint8_t keyu = 0;
  uint8_t keyd = 0;
  uint8_t keyu2 = 0;
  uint8_t keyd2 = 0;
  //CS02 CS01 CS00 Description
  //0 0 0 No clock source (Timer/Counter stopped)
  //0 0 1 clkI/O/(No prescaling)    8000000 / 160 = 50000 hz
  //0 1 0 clkI/O/8 (From prescaler) 8000000 / 160 / 8 = 6250 hz
  //0 1 1 clkI/O/64 (From prescaler)  8000000 / 160 / 64 = 781.25 hz
  //1 0 0 clkI/O/256 (From prescaler)
  //1 0 1 clkI/O/1024 (From prescaler)
  uint8_t val;
  error:
  val = 0;
  while(!Data_RecvRadio);//等待高电平
  TCNT0 = 0;//开始计时
	TIFR0 |= _BV(TOV0);
  while(Data_RecvRadio);//等待低电平
  keyu = currTick;
	SendInt(keyu);
	SerialSend('\r');
	SerialSend('\n');
	return 0;
  TCNT0 = 0; //开始计时
  if(keyu>90 || keyu<70){//+- 10周期
    goto error;
  }
  keyu2 = keyu << 1;
  keyu += 3;
  keyd = keyu - 6;
  keyu2 += 3;
  keyd2 = keyu2 -6;
  
  for(uint8_t i = 8 ; i > 0 ; i--)
  {
    while(!Data_RecvRadio);//等待高电平
    uint8_t time = TCNT0;
    if(time<keyu && time > keyd){
      val>>=1;val |= 0x80;//短 1
    }else if(time<keyu2 && time > keyd2){
      val>>=1;val |= 0x00;//长 0
    }else{
      goto error;
    }
    while(Data_RecvRadio);//等待低电平
    TCNT0 = 0;
  }
	sei();
	return val;
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}



void TimerInit() {
  //Serial
	TCCR1A = 0;
	TCCR1C = 0;
	TIMSK1 = 0;

  //Radio  
	TCCR0A = 0;
	TIMSK0 = 0;
  
}
void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
  
  //radio
	DDR_SendRadio |= BIT_SendRadio;
	DDR_RecvRadio &= ~BIT_RecvRadio;
	PORT_SendRadio |= BIT_SendRadio;
  
}

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

void loop() {
	for(;;)
	{
    //uint8_t val = SerialRead();
    //RadioSend(val);
    
    uint8_t val = RadioRead();
		SerialSend(val);
	}
}


int main(void) {
	ClockInit();
	SerialInit();
	TimerInit();
	loop();
}

