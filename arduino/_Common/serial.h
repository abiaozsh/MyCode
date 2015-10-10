#define CUR_TIMING TIMING__8M_TCCR0B_1_115200
#define TCCR0B_Value 1

PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2___9600[] = {104,208, 56,160,  8,113,217, 65,169, 17};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2__14400[] = { 69,138,208, 21, 91,160,230, 43,113,182};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_1_115200[] = { 69,138,208, 21, 91,160,230, 43,113,182};

#define DDR_Send DDRA
#define PORT_Send PORTA
#define BIT_Send _BV(2)
#define DDR_Recv DDRA
#define PIN_Recv PINA
#define BIT_Recv _BV(1)

void SerialInit();
void SerialSend(uint8_t val);
//uint8_t SerialRead();
//uint8_t SerialRead(uint16_t timeout, uint8_t* timoutParam);
void SendInt(uint32_t val);
//uint32_t ReadInt();

void SerialInit(){
	TCCR0A = 0;
	TIMSK0 = 0;
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

void SendInt(uint32_t val)
{
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

void SerialSend(uint8_t val)
{
	cli();
	TCCR0B = TCCR0B_Value;
	TCNT0 = 0;
	uint8_t timing;
	prog_uint8_t* pTiming = CUR_TIMING;
	PORT_Send &= ~BIT_Send;timing = pgm_read_byte_near(pTiming++);while(timing-TCNT0>0);//startbit
	uint8_t chkbit = 0x01;
  uint8_t i;
	for(i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit){PORT_Send |= BIT_Send;}else{PORT_Send &= ~BIT_Send;}chkbit<<=1;timing = pgm_read_byte_near(pTiming++);while(timing-TCNT0>0);
	}
	PORT_Send |= BIT_Send;timing = pgm_read_byte_near(pTiming);while(timing-TCNT0>0);
	sei();
}
