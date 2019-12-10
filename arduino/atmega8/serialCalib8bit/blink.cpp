#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

//basecount = freq / baute rate


//8000000 / 115200 = 69.4
//8000000 / 115200 * 1.5 = 104.16666666666666666666666666667
#define TCCR0_Value_115200 1
#define CNT_Value_115200 69
#define CNT_1_5_115200 104


//1000000 / 9600 = 104.16666666666666666666666666667
//1000000 / 9600 = 156.25
#define TCCR0_Value_9600 2
#define CNT_Value_9600 104
#define CNT_1_5_9600 156


#define TCCR0_Value TCCR0_Value_115200
#define CNT_Value CNT_Value_115200
#define CNT_1_5 CNT_1_5_115200


//pd0 rxd		usb txd
//pd1 txd		usb rxd
#define DDR_Send DDRD
#define PORT_Send PORTD
#define BIT_Send _BV(1)
#define DDR_Recv DDRD
#define PIN_Recv PIND
#define BIT_Recv _BV(0)

void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}
void TimerInit()
{
  // COM1A1 COM1A0 COM1B1 COM1B0 ¨C ¨C WGM11 WGM10
//CS02 CS01 CS00 Description
//0   0 0 0 No clock source (Timer/Counter stopped)
//1 8Mhz      0 0 1 clkI/O/(No prescaling)
//2 1Mhz      0 1 0 clkI/O/8 (From prescaler)
//3 1/8Mhz    0 1 1 clkI/O/64 (From prescaler)
//4 1/32Mhz   1 0 0 clkI/O/256 (From prescaler)
//5 1/128Mhz  1 0 1 clkI/O/1024 (From prescaler)
  TCCR0 = TCCR0_Value;

}
void SerialSend(uint8_t val){
	cli();
	TCCR0 = TCCR0_Value;
	TCNT0 = 0;
	PORT_Send &= ~BIT_Send;
  while(TCNT0<CNT_Value);TCNT0 = 0;//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit)
    {
      PORT_Send |= BIT_Send;
    }else{
      PORT_Send &= ~BIT_Send;
    }
    chkbit<<=1;
    while(TCNT0<CNT_Value);TCNT0 = 0;
	}
	PORT_Send |= BIT_Send;
  while(TCNT0<CNT_Value);TCNT0 = 0;//stopbit
	sei();
}

uint8_t SerialRead()
{
	cli();
	uint8_t val = 0;
	TCCR0 = TCCR0_Value;
	while(PIN_Recv&BIT_Recv){
	}
	TCNT0 = 0;
	while(TCNT0<CNT_1_5);TCNT0 = 0;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);
		while(TCNT0<CNT_Value);TCNT0 = 0;
	}
	sei();
	return val;
}

void wait(uint16_t ticks)
{
	TCCR1B = 1;
	TCNT1 = 0;//timer reset
	while(TCNT1<(ticks<<8))
	{
		;
	}
}



uint8_t buff[256];

int main()
{
	TimerInit(); //do later
	SerialInit();

  while(1){
    SerialRead();

    for(uint16_t i=0;i<256;i++){
      SerialSend((uint8_t)i);
    }
    
    uint8_t test = 0;
    for(uint16_t i=0;i<256;i++){
      uint8_t val = SerialRead();
      buff[i] = val;
      if(val!=(uint8_t)i){
        test = 1;
      }
    }
    
    SerialSend(test);

    for(uint16_t i=0;i<256;i++){
      SerialSend(buff[i]);
    }

    
  }

	return 0;
}
