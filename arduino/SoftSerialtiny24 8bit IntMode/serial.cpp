#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
	
#define CUR_TIMING TIMING__8M_TCCR0B_1_115200
#define TCCR0B_Value 1

PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2___9600[] = {104,208, 56,160,  8,113,217, 65,169, 17};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2__14400[] = { 69,138,208, 21, 91,160,230, 43,113,182};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_1_115200[] = { 0, 69,138,208, 21, 91,160,230, 43,113,182};

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
		while(SerialIdx);
		StartSend(0);
		while(SerialIdx);
		StartSend(0xFF);
		while(SerialIdx);
		StartSend(1);   //5  0000 0101
		while(SerialIdx);
		StartSend(128);//0
		while(SerialIdx);
		StartSend(0x55);// 应该0101 0101    
		while(SerialIdx);//实际1011 0101
		StartSend(0xaa);// 应该1010 1010    
		while(SerialIdx);//实际1011 0010
		StartSend(0xF0);// 应该1111 0000    
		while(SerialIdx);//实际0010 1000
		StartSend(0x0F);// 应该0000 1111    
                        // 实际0111 1101
		for(uint32_t i = 0;i<400000;i++)
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
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
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


void StartSend(uint8_t val){
  if(SerialIdx)
  {
    return;
  }
  TCCR0B = TCCR0B_Value;
  TIMSK0 = 0;

  PORT_Send &= ~BIT_Send;//startbit
  TCNT0 = 0;

  SerialData = val;
  SerialIdx++;
  OCR0A = pgm_read_byte_near(CUR_TIMING+SerialIdx);
  TIMSK0 |= _BV(OCIE0A);
}

//发送
ISR(TIM0_COMPA_vect){
  SerialIdx++;
  if(SerialIdx==11)
  {
    PORT_Send |= BIT_Send;
    SerialIdx = 0;
    TIMSK0 &= ~_BV(OCIE0A);
    OCR0A = 0;
	return;
  }
  if(SerialData&1)
  {
    PORT_Send |= BIT_Send;
  }else{
    PORT_Send &= ~BIT_Send;
  }
  SerialData>>=1;
  OCR0A = pgm_read_byte_near(CUR_TIMING+SerialIdx);
}
