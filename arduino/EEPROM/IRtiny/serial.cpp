#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#define IRPIN (PINA & _BV(1))

#define CUR_TIMING TIMING__8M_TCCR0B_1_115200
#define TCCR0B_Value 1

PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2___9600[] = {104,208, 56,160,  8,113,217, 65,169, 17};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2__14400[] = { 69,138,208, 21, 91,160,230, 43,113,182};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_1_115200[] = { 69,138,208, 21, 91,160,230, 43,113,182};

#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(0)
#define DDR_Recv DDRB
#define PIN_Recv PINB
#define BIT_Recv _BV(1)

void EEPROM_write(unsigned int ucAddress, unsigned char ucData)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEPE));
/* Set Programming mode */
EECR = (0<<EEPM1)|(0<<EEPM0);
/* Set up address and data registers */
EEAR = ucAddress;
EEDR = ucData;
/* Write logical one to EEMPE */
EECR |= (1<<EEMPE);
/* Start eeprom write by setting EEPE */
EECR |= (1<<EEPE);
}
unsigned char EEPROM_read(unsigned int ucAddress)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEPE))
;
/* Set up address register */
EEAR = ucAddress;
/* Start eeprom read by writing EERE */
EECR |= (1<<EERE);
/* Return data from data register */
return EEDR;
}

void ClockInit();
void loop();
void TimerInit();
void SerialInit();
void SerialSend(uint8_t val);
//uint8_t SerialRead();
//uint8_t SerialRead(uint16_t timeout, uint8_t* timoutParam);
void SendInt(uint32_t val);
//uint32_t ReadInt();
uint8_t GetIR();
void RAWTimeline();

int main(void) {
	ClockInit();
	SerialInit();
	TimerInit();
	SerialSend('f');
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
    //RAWTimeline();
    uint8_t v = GetIR();
    
    uint8_t dat;
    if(v==0x0C)//1
    {
      dat = 1;
      SendInt(dat);
      eeprom_write_byte(0,dat);
    }
    if(v==0x18)//2
    {
      dat = 2;
      SendInt(dat);
      eeprom_write_byte(0,dat);
    }
    if(v==0x09)//4
    {
      dat = 1;
      SendInt(dat);
      EEPROM_write(0,dat);
    }
    if(v==0x1C)//5
    {
      dat = 2;
      SendInt(dat);
      EEPROM_write(0,dat);
    }
    if(v==0x42)//7
    {
      dat = eeprom_read_byte(0);
      SendInt(dat);
    }
    if(v==0x52)//8
    {
      dat = EEPROM_read(0);
      SendInt(dat);
    }
    
  
  
	SendInt(v);
	SerialSend('\r');
	SerialSend('\n');
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

void SendInt(uint32_t val)
{
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

void SerialSend(uint8_t val)
{
	cli();
	TCCR0B = TCCR0B_Value;
	TCNT0 = 0;
	uint8_t timing;
	prog_uint8_t* pTiming = CUR_TIMING;
	PORT_Send &= ~BIT_Send;timing = pgm_read_byte_near(pTiming++);while(timing-TCNT0>0);//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit){PORT_Send |= BIT_Send;}else{PORT_Send &= ~BIT_Send;}chkbit<<=1;timing = pgm_read_byte_near(pTiming++);while(timing-TCNT0>0);
	}
	PORT_Send |= BIT_Send;timing = pgm_read_byte_near(pTiming);while(timing-TCNT0>0);
	sei();
}


uint8_t GetIR()
{
  uint8_t data = 0;
  uint8_t datahi = 0;
  uint8_t dataidx = 0;
  uint8_t byteidx = 0;
  TCCR0A = 0;
  TCCR0B = 4;//0 1 1 clkI/O/64 (From prescaler)
  while(IRPIN);//等待低电平 下降沿
  while(!IRPIN);
  while(IRPIN);
  uint8_t OLDflg = IRPIN;
  TCNT0 = 0;
  TIMSK0 = 0;
  TIFR0 |= _BV(TOV0);
  while((!(TIFR0 & _BV(TOV0))) && (byteidx<4))
  {
    if(OLDflg != IRPIN)
    {
      if(IRPIN)
      {
      }
      else
      {
        //35~105 70
        data>>=1;
        if(TCNT0<35)
        {
          data|=0x80;
        }
        dataidx++;
        if(dataidx==8)
        {
          dataidx=0;
          switch(byteidx)
          {
            case 0:
              if(data!=0xFF)return 0xFF;
              break;
            case 1:
              if(data!=0x00)return 0xFF;
              break;
            case 2:
              datahi = ~data;
              break;
          }
          byteidx++;
        }
      }
      TCNT0 = 0;
      OLDflg = IRPIN;
    }
  }
  if(data!=datahi)
  {
    return 0xFF;
  }
  return data;
}
/*
void RAWTimeline()
{
  uint16_t data[32];
  uint16_t dataidx;
  TCCR1A = 0;
  TCCR1B = 4;//1 0 0 clkI/O/256 (From prescaler)
  TIMSK1 = 0;
  while(IRPIN);//等待低电平 下降沿
  uint8_t OLDflg = IRPIN;
  dataidx = 0;
  TCNT1 = 0;
  TIFR1 |= _BV(TOV1);
  while(!(TIFR1 & _BV(TOV1)) && dataidx<32)
  {
    if(OLDflg != IRPIN)
    {
      data[dataidx++] = TCNT1;
      OLDflg = IRPIN;
    }
  }
  
  SendInt(dataidx);
  for(uint16_t i=1;i<dataidx;i++)
  {
    uint16_t dif = data[i] - data[i-1];
    SendInt(i);
    SerialSend('\t');
    SendInt(dif);
	SerialSend('\r');
	SerialSend('\n');
  }
}
*/