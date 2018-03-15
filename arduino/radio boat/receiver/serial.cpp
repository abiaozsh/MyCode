#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define DDR_Recv DDRB
#define PIN_Recv PINB
#define BIT_Recv _BV(0)

//B0 ����
//A6 ���
//A5 ���
//A4 ����̵���

void loop();
uint8_t SerialRead();

int main(void) {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
  
  
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Recv &= ~BIT_Recv;
  
	TCCR1A = _BV(COM1A1) + _BV(COM1B1) + _BV(WGM11);
  TCCR1B = _BV(CS11) + _BV(WGM13) + _BV(WGM12);
	TIMSK1 = 0;
  ICR1 = 20000;
  //PA6 OC1A
  DDRA |= _BV(6);//���
  //PA5 OC1B
  DDRA |= _BV(5);//���

  DDRA |= _BV(4);//����̵���
  
  OCR1A = 1000;
  OCR1B = 1500;
  //sei();
  
	loop();
}


void loop() {
	for(;;)
	{
		uint8_t val = SerialRead();
    if(val==0x5A){
      uint8_t val0 = SerialRead();//��� 0~64 �� 192~255 �� ������
      uint8_t val1 = SerialRead();
      if(val0==val1){
        
        uint8_t val2 = SerialRead();//��� 0~255 ת0~128
        uint8_t val3 = SerialRead();
        if(val2==val3){
          
          if(val0&0x80){
            PORTA |= _BV(4);//����̵���
          }else{
            PORTA &= ~_BV(4);//����̵���
          }
          val0 = val0 & ~0x80;
          if(val0<=64){
            uint16_t tempA = 1000;
            uint32_t temp = val0;
            temp = temp * 1000;
            temp = temp >> 6;//  /64
            tempA = tempA + temp;
            OCR1A = tempA;//1000~2000;//1ms~2ms
          }
          
          if(val2<=128){
            uint16_t tempB = 1000;
            uint32_t temp = val2;
            temp = temp * 1000;
            temp = temp >> 7;//  /128
            tempB = tempB + temp;
            OCR1B = tempB;// 1~2ms ��Ӧ ����� -45�ȵ�+45��
            //0.5��2.5mS ��Ӧ ����� -90�ȵ�+90��

          }
          
        }
      }
    }
	}
}


//#define PIN_Recv_val (PIN_Recv&BIT_Recv)
#define PIN_Recv_val (!(PIN_Recv&BIT_Recv))
#define timing 200

uint8_t SerialRead()
{
	//cli();
	uint8_t val = 0;
	while(PIN_Recv_val);
	TCCR0B = 3;
	TCNT0 = 0;
	while(TCNT0<100);TCNT0 = 0;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		while(TCNT0<timing);TCNT0 = 0;
		val>>=1;val |= (PIN_Recv_val?0x80:0);
	}
	while(TCNT0<timing);TCNT0 = 0;
	//sei();
	return val;
}
