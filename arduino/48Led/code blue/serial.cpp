#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

//d0,d1 serial
//d2~d7 6 io 74hc595
#define DDR_595_6 DDRD
#define PORT_595_6 PORTD

//b0 b1 b2 clk + st + oe 74hc595
#define DDR_CLK_ON   DDRB  |=  _BV(1)
#define PORT_CLK_ON  PORTB |=  _BV(1)
#define PORT_CLK_OFF PORTB &= ~_BV(1)

#define DDR_OE_ON   DDRB  |=  _BV(0)
#define PORT_OE_ON  PORTB |=  _BV(0)
#define PORT_OE_OFF PORTB &= ~_BV(0)

#define DDR_PNP1_ON   DDRC  |=  _BV(3)
#define DDR_PNP1_OFF  DDRC  &= ~_BV(3)
#define PORT_PNP1_ON  PORTC |=  _BV(3)
#define PORT_PNP1_OFF PORTC &= ~_BV(3)

#define DDR_PNP2_ON   DDRC  |=  _BV(2)
#define DDR_PNP2_OFF  DDRC  &= ~_BV(2)
#define PORT_PNP2_ON  PORTC |=  _BV(2)
#define PORT_PNP2_OFF PORTC &= ~_BV(2)

#define DDR_LED1_ON   DDRB  |=  _BV(4)
#define PORT_LED1_ON  PORTB |=  _BV(4)
#define PORT_LED1_OFF PORTB &= ~_BV(4)

#define DDR_LED2_ON   DDRB  |=  _BV(5)
#define PORT_LED2_ON  PORTB |=  _BV(5)
#define PORT_LED2_OFF PORTB &= ~_BV(5)

PROGMEM  prog_uint8_t DitherTable[] = {
0xff,0x7f,0xbf,0x3f,0xdf,0x5f,0x9f,0x1f,0xef,0x6f,0xaf,0x2f,0xcf,0x4f,0x8f,0x0f,
0xf7,0x77,0xb7,0x37,0xd7,0x57,0x97,0x17,0xe7,0x67,0xa7,0x27,0xc7,0x47,0x87,0x07,
0xfb,0x7b,0xbb,0x3b,0xdb,0x5b,0x9b,0x1b,0xeb,0x6b,0xab,0x2b,0xcb,0x4b,0x8b,0x0b,
0xf3,0x73,0xb3,0x33,0xd3,0x53,0x93,0x13,0xe3,0x63,0xa3,0x23,0xc3,0x43,0x83,0x03,
0xfd,0x7d,0xbd,0x3d,0xdd,0x5d,0x9d,0x1d,0xed,0x6d,0xad,0x2d,0xcd,0x4d,0x8d,0x0d,
0xf5,0x75,0xb5,0x35,0xd5,0x55,0x95,0x15,0xe5,0x65,0xa5,0x25,0xc5,0x45,0x85,0x05,
0xf9,0x79,0xb9,0x39,0xd9,0x59,0x99,0x19,0xe9,0x69,0xa9,0x29,0xc9,0x49,0x89,0x09,
0xf1,0x71,0xb1,0x31,0xd1,0x51,0x91,0x11,0xe1,0x61,0xa1,0x21,0xc1,0x41,0x81,0x01,
0xfe,0x7e,0xbe,0x3e,0xde,0x5e,0x9e,0x1e,0xee,0x6e,0xae,0x2e,0xce,0x4e,0x8e,0x0e,
0xf6,0x76,0xb6,0x36,0xd6,0x56,0x96,0x16,0xe6,0x66,0xa6,0x26,0xc6,0x46,0x86,0x06,
0xfa,0x7a,0xba,0x3a,0xda,0x5a,0x9a,0x1a,0xea,0x6a,0xaa,0x2a,0xca,0x4a,0x8a,0x0a,
0xf2,0x72,0xb2,0x32,0xd2,0x52,0x92,0x12,0xe2,0x62,0xa2,0x22,0xc2,0x42,0x82,0x02,
0xfc,0x7c,0xbc,0x3c,0xdc,0x5c,0x9c,0x1c,0xec,0x6c,0xac,0x2c,0xcc,0x4c,0x8c,0x0c,
0xf4,0x74,0xb4,0x34,0xd4,0x54,0x94,0x14,0xe4,0x64,0xa4,0x24,0xc4,0x44,0x84,0x04,
0xf8,0x78,0xb8,0x38,0xd8,0x58,0x98,0x18,0xe8,0x68,0xa8,0x28,0xc8,0x48,0x88,0x08,
0xf0,0x70,0xb0,0x30,0xd0,0x50,0x90,0x10,0xe0,0x60,0xa0,0x20,0xc0,0x40,0x80,0x00
};

//存储转换表
PROGMEM  prog_uint8_t AddressTable[] = {
 0,43, 6,37,12,31,
18,25,24,19,30,13,
36, 7,42, 1, 2,45,
 8,39,14,33,20,27,
26,21,32,15,38, 9,
44, 3, 4,47,10,41,
16,35,22,29,28,23,
34,17,40,11,46, 5, 48
};

volatile uint8_t Count256;
volatile uint8_t CurrentDT;

volatile uint8_t data0;
volatile uint8_t data1;
volatile uint8_t data2;
volatile uint8_t data3;
volatile uint8_t data4;
volatile uint8_t data5;
volatile uint8_t data6;
volatile uint8_t data7;

volatile uint8_t* volatile currBuff;//正在写入
volatile uint8_t* volatile AltBuff;//显示用
volatile uint8_t buff0[49];
volatile uint8_t buff1[49];
volatile uint8_t buffidx = 0;

void SendLine();
void Conv();

int main(void) {
  cli();
  DDR_595_6 = 0xFC;//B11111100;
  DDR_CLK_ON;
  DDR_OE_ON;
  PORT_PNP1_ON;//DDR_PNP1_ON;
  PORT_PNP2_OFF;

  //刷新定时器初始化
  //2 0 1 0 CTC OCRA Immediate MAX
  TCCR0A = 0;//_BV(WGM01);//Initial Value 0 0 0 0 0 0 0 0
  TCCR0B = 3;
  TCNT0 = 0;
  OCR0A = 128;//60周期 30us
  OCR0B = 180;//60周期 30us
  TIMSK0 = _BV(OCIE0B) | _BV(OCIE0A) | _BV(TOIE0);

  TCCR1A = 0;
  TCCR1B = 5;//1/1024 (16000000/1024=15625)tick/s
  TCCR1C = 0;
  TIMSK1 = 0;

  UCSR0A = _BV(U2X0);//U2Xn=1
  UCSR0B = _BV(RXCIE0) | _BV(RXEN0);
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
  UBRR0 = 7;//250000
  
  PORT_OE_ON;
  
  AltBuff = buff0;
  currBuff = buff1;
  sei();
	while(true)
	{
  }
}

ISR(USART_RX_vect){
  uint8_t val = UDR0;
  if(buffidx==48)
  {
    buffidx++;
    OCR0B = val;
  }
  else if(buffidx==49)
  {
    buffidx = 0;
    if(val == 1){
      DDR_PNP1_ON;
      PORT_PNP1_ON;
      DDR_PNP2_OFF;
    }
    else if(val == 2){
      DDR_PNP1_OFF;
      PORT_PNP1_OFF;
      DDR_PNP2_ON;
    }
    else
    {
      DDR_PNP1_OFF;
      PORT_PNP1_OFF;
      DDR_PNP2_OFF;
    }
    volatile uint8_t* tempBuff;
    tempBuff = currBuff;
    currBuff = AltBuff;
    AltBuff = tempBuff;
  }
  else
  {
    currBuff[pgm_read_byte_near(AddressTable+buffidx)] = val;
    buffidx++;
  }
}

#define proc(num){\
  asm volatile(\
  "ldi r18,0\n\t" \
  /*r17<-AltBuff[i] ; AltBuff[i]-DT store carry*/ \
  "ld r17,X+\n\t" "cp r16,r17\n\t" "ror r18\n\t" \
  "ld r17,X+\n\t" "cp r16,r17\n\t" "ror r18\n\t" \
  "ld r17,X+\n\t" "cp r16,r17\n\t" "ror r18\n\t" \
  "ld r17,X+\n\t" "cp r16,r17\n\t" "ror r18\n\t" \
  "ld r17,X+\n\t" "cp r16,r17\n\t" "ror r18\n\t" \
  "ld r17,X+\n\t" "cp r16,r17\n\t" "ror r18\n\t" \
  "sts %0,r18\n\t" \
  ::"m" (num));\
}

ISR(TIMER0_OVF_vect){
  
  //输出
  {
    uint8_t data[8];
    CurrentDT = pgm_read_byte_near(DitherTable + Count256);
    asm volatile("push r26");//16 17
    asm volatile("push r27");
    asm volatile("push r16");
    asm volatile("push r17");
    asm volatile("push r18");
    asm volatile("lds r26,AltBuff");
    asm volatile("lds r27,(AltBuff)+1");
    asm volatile("lds r16,CurrentDT");
    proc(data7);
    proc(data6);
    proc(data5);
    proc(data4);
    proc(data3);
    proc(data2);
    proc(data1);
    proc(data0);
    asm volatile("pop r18");
    asm volatile("pop r17");
    asm volatile("pop r16");
    asm volatile("pop r27");
    asm volatile("pop r26");
    Count256++;
    PORT_CLK_OFF;
    PORT_595_6 = data0;PORT_CLK_ON;PORT_CLK_OFF;
    PORT_595_6 = data1;PORT_CLK_ON;PORT_CLK_OFF;
    PORT_595_6 = data2;PORT_CLK_ON;PORT_CLK_OFF;
    PORT_595_6 = data3;PORT_CLK_ON;PORT_CLK_OFF;
    PORT_595_6 = data4;PORT_CLK_ON;PORT_CLK_OFF;
    PORT_595_6 = data5;PORT_CLK_ON;PORT_CLK_OFF;
    PORT_595_6 = data6;PORT_CLK_ON;PORT_CLK_OFF;
    PORT_595_6 = data7;PORT_CLK_ON;PORT_CLK_OFF;
    PORT_CLK_ON; //shift clock up
    PORT_CLK_OFF; //shift clock down
  }

}

//TIMER0_OVF_vect
ISR(TIMER0_COMPA_vect){
  //亮
  PORT_OE_OFF;//(on)
  //DDR_PNP2_ON;//PORT_PNP1_ON;
}

//TIMER0_OVF_vect
ISR(TIMER0_COMPB_vect){
  //暗
  //DDR_PNP2_OFF;//PORT_PNP1_OFF;
  PORT_OE_ON;//(off)
}
