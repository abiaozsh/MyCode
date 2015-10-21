#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	
#define CUR_TIMING TIMING__8M_TCCR0B_2___9600
#define TCCR0B_Value 2

//PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2___9600[] = {102,206, 54,158,  6,111,215, 63,167, 15};

//PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2___9600[] = {103,207, 55,159,  7,112,216, 64,168, 16};

PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2___9600[] = {104,208, 56,160,  8,113,217, 65,169, 17};

#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(0)
#define DDR_Recv DDRB
#define PIN_Recv PINB
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
void StartListening();
void StopListening();
void DataReceived();

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

uint8_t buff[8];
void loop() {
	for(;;)
	{

    uint8_t i=0;
    while(i!=255)
    {
      for(uint32_t l = 0;l<10000;l++){asm volatile("nop");}while(SerialIdx);
      StartSend(i++);
    }
    for(uint32_t l = 0;l<10000;l++){asm volatile("nop");}while(SerialIdx);
    StartSend(255);

    for(uint32_t l = 0;l<10000;l++){asm volatile("nop");}
    i=0;
    while(i!=255)
    {
      while(SerialIdx);
      StartSend(i++);
    }
    while(SerialIdx);
    StartSend(255);

		for(uint32_t l = 0;l<10000000;l++)
		{
			asm volatile("nop");
		}
		
		//StartListening();
		//for(;;);
	}
}
void DataReceived(){
  for(uint32_t i = 0;i<4000;i++)
  {
    asm volatile("nop");
  }
  StartSend(SerialData);
}

void TimerInit() {
	TCCR0A = 0;
	TIMSK0 = 0;
}

void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	PORT_Send |= BIT_Send;
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
}

/*
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
  TIMSK0 |= _BV(OCIE0B);
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
    TIMSK0 &= ~_BV(OCIE0B);
    OCR0B = 0;
    DataReceived();
  }
}
*/

void StartSend(uint8_t val){
  if(SerialIdx)
  {
    return;
  }
  TCCR0B = TCCR0B_Value;
  TIMSK0 = 0;

  PORT_Send &= ~BIT_Send;//start bit
  TCNT0 = 0;

  SerialData = val;
  OCR0A = pgm_read_byte_near(CUR_TIMING);
  SerialIdx++;
  TIMSK0 |= _BV(OCIE0A);
}

//发送
ISR(TIM0_COMPA_vect){
  if(SerialIdx>8)//stop bit
  {
    PORT_Send |= BIT_Send;
  }
  else
  {
    if(SerialData&1)
    {
      PORT_Send |= BIT_Send;
    }else{
      PORT_Send &= ~BIT_Send;
    }
    SerialData>>=1;
  }
  if(SerialIdx==10)
  {
    SerialIdx = 0;
    TCCR0B = 0;
    TIMSK0 &= ~_BV(OCIE0A);
    return;
  }
  OCR0A = pgm_read_byte_near(CUR_TIMING+SerialIdx);
  SerialIdx++;
}
