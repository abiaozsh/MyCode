//d0,d1 serial
//d2~d7 6 io 74hc595
#define DDR_595_6 DDRD
#define PORT_595_6 PORTD

//b0 b1 b2 clk + st + oe 74hc595
#define DDR_CLK DDRB
#define PORT_CLK PORTB
#define BIT_CLK _BV(1)

#define DDR_OE DDRB
#define PORT_OE PORTB
#define BIT_OE _BV(0)

#define DDR_PNP1 DDRC
#define PORT_PNP1 PORTC
#define BIT_PNP1 _BV(3)

#define DDR_PNP2 DDRB
#define PORT_PNP2 PORTB
#define BIT_PNP2 _BV(2)

PROGMEM prog_uint8_t data[] = {

  
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//11111111,11111111,11111111,11111111,11111111,11111111
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,

  
  
/*
B00000001,B00000000,B00000000,B00000000,B00000000,B00000000,//00
B00000010,B00000000,B00000000,B00000000,B00000000,B00000000,//01
B00000100,B00000000,B00000000,B00000000,B00000000,B00000000,//02
B00001000,B00000000,B00000000,B00000000,B00000000,B00000000,//03
B00010000,B00000000,B00000000,B00000000,B00000000,B00000000,//04
B00100000,B00000000,B00000000,B00000000,B00000000,B00000000,//05
B01000000,B00000000,B00000000,B00000000,B00000000,B00000000,//06
B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,//07
B00000000,B00000001,B00000000,B00000000,B00000000,B00000000,//08
B00000000,B00000010,B00000000,B00000000,B00000000,B00000000,//09
B00000000,B00000100,B00000000,B00000000,B00000000,B00000000,//10
B00000000,B00001000,B00000000,B00000000,B00000000,B00000000,//11
B00000000,B00010000,B00000000,B00000000,B00000000,B00000000,//12
B00000000,B00100000,B00000000,B00000000,B00000000,B00000000,//13
B00000000,B01000000,B00000000,B00000000,B00000000,B00000000,//14
B00000000,B10000000,B00000000,B00000000,B00000000,B00000000,//15
B00000000,B00000000,B00000001,B00000000,B00000000,B00000000,//16
B00000000,B00000000,B00000010,B00000000,B00000000,B00000000,//17
B00000000,B00000000,B00000100,B00000000,B00000000,B00000000,//18
B00000000,B00000000,B00001000,B00000000,B00000000,B00000000,//19
B00000000,B00000000,B00010000,B00000000,B00000000,B00000000,//20
B00000000,B00000000,B00100000,B00000000,B00000000,B00000000,//21
B00000000,B00000000,B01000000,B00000000,B00000000,B00000000,//22
B00000000,B00000000,B10000000,B00000000,B00000000,B00000000,//23
B00000000,B00000000,B00000000,B00000001,B00000000,B00000000,//24
B00000000,B00000000,B00000000,B00000010,B00000000,B00000000,//25
B00000000,B00010000,B00000000,B00000100,B00000000,B00000000,//26
B00000000,B00010000,B00000000,B00001000,B00000000,B00000000,//27
B00000000,B00010000,B00000000,B00010000,B00000000,B00000000,//28
B00000000,B00010000,B00000000,B00100000,B00000000,B00000000,//29
B00000000,B11111111,B00000000,B01000000,B00000000,B00000000,//30
B00000000,B00010000,B00000000,B10000000,B00000000,B00000000,//31
B00000000,B00010000,B00000000,B00000000,B00000001,B00000000,//32
B00000000,B00010000,B00000000,B00000000,B00000010,B00000000,//33
B00000000,B00010000,B00000000,B00000000,B00000100,B00000000,//34
B00000000,B00000000,B00000000,B00000000,B00001000,B00000000,//35
B00000000,B00000000,B00000000,B00000000,B00010000,B00000000,//36
B00000000,B00000000,B00000000,B00000000,B00100000,B00000000,//37
B00000000,B00000000,B00000000,B00000000,B01000000,B00000000,//38
B00000000,B00000000,B00000000,B00000000,B10000000,B00000000,//39
B00000000,B00000000,B00000000,B00000000,B00000000,B00000001,//40
B00000000,B00000000,B00000000,B00000000,B00000000,B00000010,//41
B00000000,B00000000,B00000000,B00000000,B00000000,B00000100,//42
B00000000,B00000000,B00000000,B00000000,B00000000,B00001000,//43
B00000000,B00000000,B00000000,B00000000,B00000000,B00010000,//44
B00000000,B00000000,B00000000,B00000000,B00000000,B00100000,//45
B00000000,B00000000,B00000000,B00000000,B00000000,B01000000,//46
B00000000,B00000000,B00000000,B00000000,B00000000,B10000000,//47
*/
};

uint8_t buff[384];

void setup() {
    //pgm_read_byte_near(data0);
	TCCR1A = 0;
	TCCR1B = 1;
	TCCR1C = 0;
	TIMSK1 = 0;
	DDR_595_6 = B11111100;
	DDR_CLK |= BIT_CLK;
	DDR_OE |= BIT_OE;
	DDR_PNP1 |= BIT_PNP1;

	for(uint16_t i=0;i<48;i++)
	{
		for(uint8_t t = 0;t<6;t++)
		{
			buff[i*8+t] = pgm_read_byte_near(data+i*6+t);
		}
		Conv(buff+i*8);
	}
}


void loop() {
    while(true)
    {

	for(uint16_t i=0;i<48;i++)
	{
	TCNT1 = 0;
        SendLine(buff+i*8);
	while(TCNT1<9000);
	PORT_PNP1 |= BIT_PNP1;//PNP on H
	PORT_OE &= ~BIT_OE;//OE on L
	while(TCNT1<10000);
	PORT_PNP1 &= ~BIT_PNP1;//PNP disable L
	PORT_OE |= BIT_OE;//OE disable H
	}
    }
}

// 24*12 12bit width /8word
//buff : 8*12*8 = 768 byte
/*uint8_t buff[768];
void start()
{
	TCCR1A = 0;
	TCCR1B = 5;
	TCCR1C = 0;
	TIMSK1 = 0;
  while(true)//1s
  {
    TCNT1 = 0;
    //proc();
    Time t;
    GetTime(&t);
    
    //drawImg();
    
    //Conv();
    for(uint16_t i=0;i<768;i+=8)
    {
      Conv(buff+i);
    }
    
    
    
    while(TCNT1<15625)
    {
      //draw();
    }
  }
}
*/

void SendLine(uint8_t* data)
{
	PORT_CLK &= ~BIT_CLK;
        uint8_t* idx = data + 8;
	for(uint8_t i=0;i<8;i++)
	{
                idx--;
		PORT_595_6 = *idx;//setData bit0.1 is usart use bit2~7
		PORT_CLK |= BIT_CLK; //shift clock up
		PORT_CLK &= ~BIT_CLK; //shift clock down
	}
	PORT_CLK |= BIT_CLK; //shift clock up
	PORT_CLK &= ~BIT_CLK; //shift clock down
}

void Conv(uint8_t* data) // 6->8
{
  //                               7                       6                       5                       4                       3                       2  1  0 
  uint8_t b0 = ((data[5]&_BV(7))   ) | ((data[4]&_BV(0))<<6) | ((data[3]&_BV(7))>>2) | ((data[2]&_BV(0))<<4) | ((data[1]&_BV(7))>>4) | ((data[0]&_BV(0))<<2) |0 |0;
  uint8_t b1 = ((data[5]&_BV(5))<<2) | ((data[4]&_BV(2))<<4) | ((data[3]&_BV(5))   ) | ((data[2]&_BV(2))<<2) | ((data[1]&_BV(5))>>2) | ((data[0]&_BV(2))   ) |0 |0;
  uint8_t b2 = ((data[5]&_BV(3))<<4) | ((data[4]&_BV(4))<<2) | ((data[3]&_BV(3))<<2) | ((data[2]&_BV(4))   ) | ((data[1]&_BV(3))   ) | ((data[0]&_BV(4))>>2) |0 |0;
  uint8_t b3 = ((data[5]&_BV(1))<<6) | ((data[4]&_BV(6))   ) | ((data[3]&_BV(1))<<4) | ((data[2]&_BV(6))>>2) | ((data[1]&_BV(1))<<2) | ((data[0]&_BV(6))>>4) |0 |0;
  uint8_t b4 = ((data[4]&_BV(7))   ) | ((data[5]&_BV(0))<<6) | ((data[2]&_BV(7))>>2) | ((data[3]&_BV(0))<<4) | ((data[0]&_BV(7))>>4) | ((data[1]&_BV(0))<<2) |0 |0;
  uint8_t b5 = ((data[4]&_BV(5))<<2) | ((data[5]&_BV(2))<<4) | ((data[2]&_BV(5))   ) | ((data[3]&_BV(2))<<2) | ((data[0]&_BV(5))>>2) | ((data[1]&_BV(2))   ) |0 |0;
  uint8_t b6 = ((data[4]&_BV(3))<<4) | ((data[5]&_BV(4))<<2) | ((data[2]&_BV(3))<<2) | ((data[3]&_BV(4))   ) | ((data[0]&_BV(3))   ) | ((data[1]&_BV(4))>>2) |0 |0;
  uint8_t b7 = ((data[4]&_BV(1))<<6) | ((data[5]&_BV(6))   ) | ((data[2]&_BV(1))<<4) | ((data[3]&_BV(6))>>2) | ((data[0]&_BV(1))<<2) | ((data[1]&_BV(6))>>4) |0 |0;
  data[0] = b0;
  data[1] = b1;
  data[2] = b2;
  data[3] = b3;
  data[4] = b4;
  data[5] = b5;
  data[6] = b6;
  data[7] = b7;

}