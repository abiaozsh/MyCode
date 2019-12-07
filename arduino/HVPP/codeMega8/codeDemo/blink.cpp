#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define CUR_TIMING_REC TIMINGR_8M_TCCR1B_1___9600
#define CUR_TIMING TIMING__8M_TCCR1B_1___9600
//#define CUR_TIMING TIMING__8M_TCCR1B_1_115200
#define TCCR1B_Value 1

PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1_115200[] = {   69,  138,  208,  277,  347,  416,  486,  555,  625,  694};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1___9600[] = {  833, 1666, 2500, 3333, 4166, 5000, 5833, 6666, 7500, 8333};
PROGMEM prog_uint16_t TIMINGR_8M_TCCR1B_1___9600[] = { 1233, 2066, 2900, 3733, 4566, 5400, 6233, 7066, 7900, 8333};

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
  TCCR1A = 0;
  //ICNC1 ICES1 ¨C WGM13 WGM12 CS12 CS11 CS10

  //0 0 1 clkI/O/1 (No prescaling)
  //0 1 0 clkI/O/8 (From prescaler)
  //0 1 1 clkI/O/64 (From prescaler)
  //1 0 0 clkI/O/256 (From prescaler)
  //1 0 1 clkI/O/1024 (From prescaler)
  //TCCR1B = 1;//  1/1
  //TCCR1B = 2;//  1/8 //1Mhz
  //TCCR1B = 3;//  1/64
  
  TCCR1B = TCCR1B_Value;
}
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

uint8_t SerialRead()
{
	cli();
	uint8_t val = 0;
	//TCCR1B = *timoutParam;//timeout param 1~5 : 1/(1~1024) max (0.008192 s ~ 8.388608 s)
	//TCNT1 = 0;TIFR |= _BV(TOV1);
	while(PIN_Recv&BIT_Recv){
		//if(((TIFR & _BV(TOV1))?0x0FFFF:TCNT1)>=timeout)
		//{
		//	*timoutParam = 0;
		//	return 0;
		//}
	}
	TCCR1B = TCCR1B_Value;
	TCNT1 = 0;
	uint16_t timing;
	prog_uint16_t* pTiming = CUR_TIMING_REC;
	timing = pgm_read_word_near(pTiming++);
	while(TCNT1<timing);
	
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);
		timing = pgm_read_word_near(pTiming++);
		while(TCNT1<timing);
	}
	sei();
	return val;
}

void wait(uint16_t ticks)
{
	TCNT1 = 0;//timer reset
	while(TCNT1<(ticks<<8))
	{
		;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char convt[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void printOK(){
  SerialSend('O');SerialSend('K');
}
void printHex(uint16_t val){//"ll"+"hh"
  SerialSend(convt[((val & 0xF0) >> 4)]);
  SerialSend(convt[((val & 0x0F))]);
  SerialSend(convt[((val & 0xF000) >> 12)]);
  SerialSend(convt[((val & 0x0F00) >> 8)]);
}
void printHex(uint8_t val){//"hl"
  SerialSend(convt[((val & 0xF0) >> 4)]);
  SerialSend(convt[((val & 0x0F))]);
}

uint8_t ConvBCD(uint8_t val){
  if(val>='0'&&val<='9')
  {
    val = val - '0';
  }
  else if(val>='a'&&val<='f')
  {
    val = val - 'a' + 10;
  }
  else if(val>='A'&&val<='F')
  {
    val = val - 'A' + 10;
  }
  return val;
}

uint8_t GetByte(){
  uint8_t vh = SerialRead();
  uint8_t vl = SerialRead();
  uint8_t val = (( ConvBCD(vh) << 4 ) & 0xF0 ) | (ConvBCD(vl) & 0x0F);
  return val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
atmega8p
8io pb data
pb0		PB0
pb1		PB1
pb2		PB2
pb3		PB3
pb4		PB4
pb5		PB5
pb6		PC0
pb7		PC1

pc0 in  rdy	PD1
pc1 out clock	PB6
pc2 out bs2	PC2
pc3 out VCC 5v
pc4 out V12 12v

pd0 rxd		usb txd
pd1 txd		usb rxd

pd2 out !oe	PD2
pd3 out !wr	PD3
pd4 out bs1	PD4
pd5 out xa0	PD5
pd6 out xa1	PD6
pd7 out pagel	PD7
*/
#define DATA      PORTB
#define GETDATA   PINB
#define DATA_IN   DDRB = 0
#define DATA_OUT  DDRB = 0xFF

#define RDY  PINC & _BV(0)

#define CLK_OUT  DDRC  |=  _BV(1)
#define CLK_H    PORTC |=  _BV(1)
#define CLK_L    PORTC &= ~_BV(1)

#define BS2_OUT  DDRC  |=  _BV(2)
#define BS2_H    PORTC |=  _BV(2)
#define BS2_L    PORTC &= ~_BV(2)

#define VCC_OUT  DDRC  |=  _BV(3)
#define VCC_ON   PORTC &= ~_BV(3)
#define VCC_OFF  PORTC |=  _BV(3)

#define V12_OUT  DDRC  |=  _BV(4)
#define V12_ON   PORTC |=  _BV(4)
#define V12_OFF  PORTC &= ~_BV(4)

#define O_E_OUT  DDRD  |=  _BV(2)
#define O_E_H    PORTD |=  _BV(2)
#define O_E_L    PORTD &= ~_BV(2)

#define W_R_OUT  DDRD  |=  _BV(3)
#define W_R_H    PORTD |=  _BV(3)
#define W_R_L    PORTD &= ~_BV(3)

#define BS1_OUT  DDRD  |=  _BV(4)
#define BS1_H    PORTD |=  _BV(4)
#define BS1_L    PORTD &= ~_BV(4)

#define XA0_OUT  DDRD  |=  _BV(5)
#define XA0_H    PORTD |=  _BV(5)
#define XA0_L    PORTD &= ~_BV(5)

#define XA1_OUT  DDRD  |=  _BV(6)
#define XA1_H    PORTD |=  _BV(6)
#define XA1_L    PORTD &= ~_BV(6)

#define PGL_OUT  DDRD  |=  _BV(7)
#define PGL_H    PORTD |=  _BV(7)
#define PGL_L    PORTD &= ~_BV(7)





int main()
{
	TimerInit(); //do later
	SerialInit();
  DATA_IN  ;
  CLK_L    ;
  CLK_OUT  ;
  BS2_L    ;
  BS2_OUT  ;
  VCC_OFF  ;
  VCC_OUT  ;
  V12_OFF  ;
  V12_OUT  ;
  O_E_H    ;
  O_E_OUT  ;
  W_R_H    ;
  W_R_OUT  ;
  BS1_L    ;
  BS1_OUT  ;
  XA0_L    ;
  XA0_OUT  ;
  XA1_L    ;
  XA1_OUT  ;
  PGL_L    ;
  PGL_OUT  ;



  
  
  
	for (;;) {
    uint8_t c = SerialRead();
	
    
    if(c=='A'){
      uint8_t val = GETDATA;
      printHex(val);
    }
    
    //#define DATA      PORTB
    if(c=='a'){
      uint8_t val = GetByte();
      DATA = val;
      printOK();
    }
//#define DATA_IN   DDRB = 0
    if(c=='b'){
      DATA_IN;
    }
//#define DATA_OUT  DDRB = 0xFF
    if(c=='c'){
      DATA_OUT;
    }
//#define RDY  PINC & _BV(0)
    if(c=='d'){
      printHex((uint8_t)(RDY));
    }
//#define CLK_H    PORTC |=  _BV(1)
    if(c=='e'){
      CLK_H;
    }
//#define CLK_L    PORTC &= ~_BV(1)
    if(c=='f'){
      CLK_L;
    }
//#define BS2_H    PORTC |=  _BV(2)
    if(c=='g'){
      BS2_H;
    }
//#define BS2_L    PORTC &= ~_BV(2)
    if(c=='h'){
      BS2_L;
    }
//#define VCC_ON   PORTC &= ~_BV(3)
    if(c=='i'){
      VCC_ON;
    }
//#define VCC_OFF  PORTC |=  _BV(3)
    if(c=='j'){
      VCC_OFF;
    }
//#define V12_ON   PORTC |=  _BV(4)
    if(c=='k'){
      V12_ON;
    }
//#define V12_OFF  PORTC &= ~_BV(4)
    if(c=='l'){
      V12_OFF;
    }
//#define O_E_H    PORTD |=  _BV(2)
    if(c=='m'){
      O_E_H;
    }
//#define O_E_L    PORTD &= ~_BV(2)
    if(c=='n'){
      O_E_L;
    }
//#define W_R_H    PORTD |=  _BV(3)
    if(c=='o'){
      W_R_H;
    }
//#define W_R_L    PORTD &= ~_BV(3)
    if(c=='p'){
      W_R_L;
    }
//#define BS1_H    PORTD |=  _BV(4)
    if(c=='q'){
      BS1_H;
    }
//#define BS1_L    PORTD &= ~_BV(4)
    if(c=='r'){
      BS1_L;
    }
//#define XA0_H    PORTD |=  _BV(5)
    if(c=='s'){
      XA0_H;
    }
//#define XA0_L    PORTD &= ~_BV(5)
    if(c=='t'){
      XA0_L;
    }
//#define XA1_H    PORTD |=  _BV(6)
    if(c=='u'){
      XA1_H;
    }
//#define XA1_L    PORTD &= ~_BV(6)
    if(c=='v'){
      XA1_L;
    }
//#define PGL_H    PORTD |=  _BV(7)
    if(c=='w'){
      PGL_H;
    }
//#define PGL_L    PORTD &= ~_BV(7)
    if(c=='x'){
      PGL_L;
    }
    if(c=='y'){
      VCC_ON;
      V12_ON;
    }
    if(c=='z'){
      VCC_OFF;
      V12_OFF;
    }

    }
	return 0;
}
