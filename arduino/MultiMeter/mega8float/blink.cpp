#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


//basecount = freq / baute rate


//8000000 / 115200 = 69.4
//8000000 / 115200 * 1.5 = 104.16666666666666666666666666667
#define TCCR0_Value_115200 1
#define CNT_Value_115200 64
#define CNT_1_5_115200 96


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


void wait(uint16_t ticks)
{
	TCNT1 = 0;//timer reset
	while(TCNT1<ticks);
}
volatile uint8_t dummy=0;

#define EN (PINB & _BV(2))
#define CLK (PINB & _BV(0))
#define DATA (PIND & _BV(7))

int main()
{
	TCCR1B = 2;
 SerialInit();
 TimerInit();

	while (true) {
      int32_t val_req = 0;

	  int32_t mask = 1;
	  //val_req = 256000+dummy;
		while(!EN);
		for(uint8_t i=0;i<32;i++){
		  while(!CLK && EN);
		  if(DATA){
			val_req |= mask;
		  }
		  while(CLK && EN);
		  mask<<=1;
		}

      uint8_t _sign = 0;
      if(val_req < 0){
        val_req = -val_req;
        _sign = 1;
      }
      float val = (float)val_req; //max +- 512k / 1.1v
      
      uint8_t digiPos = 0;
      uint8_t digi0;
      uint8_t digi1;
      uint8_t digi2;
      uint8_t digi3;

      //a6 high gain low gain
      //a7 V/A ÂÖÁ÷¼à²â
//      if(PINA|=_BV(7)){//V
        // a3-a2 Vin    0x20 01 0000*1  0x21 01 0001*20
//        if(PINA|=_BV(6)){//high gain 20x
        
//        }else{//low gain 1x

          val = val / (512.0*256.0) * 1.1; //v
          
          //a1 1/50  max 55V  max 1.1V
          if(false){//PINA|=_BV(1)
            val = val * (50.0);
          }
          if(val>10){
          //xx.xx
            digiPos = 1;
            val = val * 0.1;
          }else if(val>0){
          //x.xxx
            digiPos = 0;
          }else{
          //xxxx
            digiPos = 0xFF;
            val = val * 10;
          }
          digi0 = (uint8_t)val;
          val -= digi0;
          val = val * 10;
          digi1 = (uint8_t)val;
          val -= digi1;
          val = val * 10;
          digi2 = (uint8_t)val;
          val -= digi2;
          val = val * 10;
          digi3 = (uint8_t)val;
          
		  
		  
		  
		  if(_sign){
			SerialSend('-');  
		  }else{
			SerialSend('+');
		  }
          SerialSend('0'+digi0);
		  if(digiPos==0){
			  SerialSend('.');
		  }
          SerialSend('0'+digi1);
		  if(digiPos==1){
			  SerialSend('.');
		  }
		  SerialSend('0'+digi2);
		  SerialSend('0'+digi3);
		  
		  SerialSend('\r');
		  SerialSend('\n');
		  
		//for(uint16_t i=0;i<50;i++){
		//	wait(10000);
		//}
      
    }

    
}

ISR(TIMER0_OVF_vect){

}
