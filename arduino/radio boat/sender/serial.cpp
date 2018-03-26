#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

//A0 //电调 0~64 反 192~255 正 其他关
//A1 //舵机 0~255 转0~128
//B0 输出

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(0)

void ClockInit();
void loop();
void TimerInit();
void SerialInit();
void SerialSend(uint8_t val);
void SendInt(uint32_t val);
uint8_t SerialRead();
uint8_t SerialRead(uint16_t timeout, uint8_t* timoutParam);
uint32_t ReadInt();

int main(void) {
	ClockInit();
	SerialInit();
	TimerInit();
  
  //ADMUX = POWER_IN;//A7 power
  DIDR0 |= _BV(0);//1
  DIDR0 |= _BV(1);//1
  ADCSRA = _BV(ADEN) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);// | _BV(ADIE) | _BV(ADATE) _BV(ADSC) | 
  ADCSRB = _BV(ADLAR);
  
  
	loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

uint8_t aread(uint8_t pin){
  ADMUX = pin;
  ADCSRA |= _BV(ADSC);while(ADCSRA & _BV(ADSC));
  return ADCH;
}

void wait(uint16_t timing){
  TCNT1 = 0;
  while(TCNT1<timing);
}

void loop() {
  uint8_t status=0;
	for(;;)
	{
      wait(1000);
      uint8_t val1 = aread(0);//电调 0~64 反 192~255 正 其他关
      uint8_t val2 = aread(1);//舵机 0~255 转0~128
      
      SerialSend(0x5A);
      
      if(val1<=64){
        status = 0;
      }
      else if(val1>=128){
        status = 1;
      }
      
      uint8_t send0 = 0;
      if(status){  
       send0 = 0x80;
       }
      if(val1<=64){
        if(PINA&_BV(5)){
        send0 += (64-val1)>>3;
        }else{
        send0 += (64-val1)>>2;
        }
      }
      else if(val1>=128){
        if(PINA&_BV(5)){
        send0 += (val1-128)>>3;
        }else{
        send0 += (val1-128)>>1;
        }
      }
      else{
      }
      SerialSend(send0);
      SerialSend(send0);
      
      {
        uint8_t send2 = val2>>1;
        SerialSend(send2);
        SerialSend(send2);
      }
      
	}
}


void TimerInit() {
	TCCR1A = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
}

//#define PORT_Send_ON PORT_Send |= BIT_Send
//#define PORT_Send_OFF PORT_Send &= ~BIT_Send
#define timing 100
#define PORT_Send_ON PORT_Send &= ~BIT_Send
#define PORT_Send_OFF PORT_Send |= BIT_Send

void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	PORT_Send_ON;
	TCCR1B = 3;
}

void SerialSend(uint8_t val){
	TCCR1B = 3;
	TCNT1 = 0;
	PORT_Send_OFF;while(TCNT1<timing);TCNT1=0;//startbit
	uint8_t chkbit = 0x01;
	//低bit先发送
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit){PORT_Send_ON;}else{PORT_Send_OFF;}chkbit<<=1;
		while(TCNT1<timing);TCNT1=0;
	}
	PORT_Send_ON;while(TCNT1<timing);TCNT1=0;
}
