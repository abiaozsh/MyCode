#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define STAOn   DDRB |= _BV(3) ;/**/
#define STAOff  DDRB &= ~_BV(3);/**/

#define CUR_TIMING TIMING__8M_TCCR1B_1_115200
#define TCCR1B_Value 1
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1_115200[] = {   69,  138,  208,  277,  347,  416,  486,  555,  625,  694};

#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(0)
#define DDR_Recv DDRA
#define PIN_Recv PINA
#define BIT_Recv _BV(7)/*6*/

#define BP1D _BV(5)
#define BP1U _BV(4)
#define BP2D _BV(3)
#define BP2U _BV(2)
#define BP3D _BV(1)
#define BP3U _BV(0)

uint8_t STEP1[] = {
BP1D + BP2D + BP3D,
BP1D +    0 + BP3D,//o	L	o
   0 + BP2D + BP3D,//L	o	o
BP1D + BP2D +    0,//o	o	L
BP1D +    0 + BP3D,//o	L	o
  };
uint8_t STEP2[] = {
BP1D + BP2D + BP3D,
   0 + BP2D + BP3D +BP2U,//L	H	o
BP1D +    0 + BP3D +BP1U,//H	L	o
BP1D +    0 + BP3D +BP3U,//o	L	H
BP1D + BP2D +    0 +BP2U,//o	H	L
  };

uint8_t SerialRead()
{
	TCCR1B = TCCR1B_Value;
	uint8_t val = 0;
	while(PIN_Recv&BIT_Recv){}
	TCNT1 = 0;
	uint16_t timing;
	timing = pgm_read_word_near(CUR_TIMING);while(TCNT1<timing);//startbit

  uint8_t i;
	for(i = 1;i<=8;i++)
	{
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);timing = pgm_read_word_near(CUR_TIMING + i);while(TCNT1<timing);
	}
	return val;
}

volatile uint8_t step = 0;
volatile uint8_t STEP1_A = 0;
volatile uint8_t STEP2_A = 0;
volatile uint8_t lenA = 0xFF;

volatile uint8_t STEP1_B = 0;
volatile uint8_t STEP2_B = 0;
volatile uint8_t lenB = 0xFF;

int main(void) {
  //初始化时钟：1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  
  DDR_Send |= BIT_Send;
  DDR_Recv &= ~BIT_Recv;
  PORT_Send |= BIT_Send;
  
  TCCR1A = 0;
  TCCR1C = 0;
  TIMSK1 = 0;

  TCCR0A = 0;
  TCCR0B = 3;//??
  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);
  
  //打开输出端口
  DDRA = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  
  sei();
  //主循环
  for(;;) 
  {
    uint8_t data = SerialRead();
    if(data==23)
    {
      STAOn;
      uint8_t data1 = SerialRead();
      uint8_t data2 = SerialRead();
      uint8_t data3 = SerialRead();
      uint8_t data4 = SerialRead();
      
      STEP1_A = STEP1[data1];
      STEP2_A = STEP2[data1];
      lenA = data2;
      STEP1_B = STEP1[data3];
      STEP2_B = STEP2[data3];
      lenB = data4;
    }
    else
    {
      STAOff;
    }
  }
}

ISR(TIM0_OVF_vect){
  PORTA = STEP1_A;
  OCR0A = lenA;
  
  uint8_t temp;
  
  temp = STEP1_A;
  STEP1_A = STEP1_B;
  STEP1_B = temp;
  
  temp = STEP2_A;
  STEP2_A = STEP2_B;
  STEP2_B = temp;
  
  temp = lenA;
  lenA = lenB;
  lenB = temp;
}

ISR(TIM0_COMPA_vect){
  PORTA = STEP2_B;
}
