/*
08 80 53

c2 11000010
08 00001000

20 00100000
80 10000000

14 00010100
53 01010110

FF08 80 53 08 80 53 08 80 53 08 80


ofonsd9fsd00sd00sd00sd00sd00sd00ofrt
onsdabsd00sd00sd00sd00ofrt
onsd90sd00sd00sd01sd00sd00ofrt
onsd90sd00sd00sd00sd00sd00ofrt

9f RDID Command manufacturer ID memory type memory density
C2 20 14
ab RES Command electronic ID
13
90 REMS Command manufacturer ID device ID
C2 13

onsd03sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00sd00of

FFFFFFFFFFFFFFFFFFFFFFFF140A0300208AAFE514008CB2FFFF1B0000
FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
FFFFFFFFFFFFFFFF140A0300208AAFE514008CB2FFFF1B0000


ofonrd00000011ofrt
ofonrd00000020ofrt FFFFFFFFFFFFFFFF140A0300208AAFE514008CB2FFFF1B0000000000120A0300
ofonrd00001020ofrt                                 14008CB2FFFF1B0000000000120A03008BA206002DB08CBDF758D29D67FBEF7B
ofonrd00002020ofrt                                                                 8BA206002DB08CBDF758D29D67FBEF7B7777772E597760424808C9008C4002C8
ofonrd0f004020ofrt

ofonfr00001010ofrt
ofonfr00000020ofrt FFFFFFFFFFFFFFFF140A0300208AAFE514008CB2FFFF1B0000000000120A0300
ofonfr00001020ofrt                                 14008CB2FFFF1B0000000000120A03008BA206002DB08CBDF758D29D67FBEF7B
ofonfr00002020ofrt                                                                 8BA206002DB08CBDF758D29D67FBEF7B7777772E597760424808C9008C4002C8
ofonfr0f004020ofrt

of
on
rd0f004010rt
of
rt


ofonrd11000040ofrt
*/
//	_BV(1), // D2 - PD1
//	_BV(0),	// D3 - PD0
//	_BV(4),	// D4 - PD4
//	_BV(6), // D5 - PC6

#define si _BV(1)//_BV(2)//data out
#define sclk _BV(0)//_BV(3)
#define cs _BV(4)//_BV(4)
#define so _BV(6)//_BV(5)//readin

void setup()
{
  //PORTD = 0;
  //	_BV(1), // D2 - PD1
//	_BV(0),	// D3 - PD0
//	_BV(4),	// D4 - PD4
//	_BV(6), // D5 - PC6
  PORTD &=~_BV(1);
  PORTD &=~_BV(0);
  PORTD &=~_BV(4);
  PORTC &=~_BV(6);
  Serial.begin(9600);
  //U2Xn is 1
  //UBRR0 = 7;//250000
  //UBRR0 = 1;//1M
}



void loop()
{
  int cmd1 = Serial.read();
  if(cmd1>='a'&&cmd1<='z'){}else{return;}
  int cmd2 = Serial.read();
  if(cmd2>='a'&&cmd2<='z'){}else{return;}

  if(cmd1=='o' && cmd2=='n')
  {
    DDRD |= cs;
  }
  else if(cmd1=='o' && cmd2=='f')
  {
    DDRD &= ~cs;
  }
  else if(cmd1=='s' && cmd2=='d')
  {
    uint8_t val = GetByte();
    shiftOut2(val);
  }
  else if(cmd1=='g' && cmd2=='t')
  {
    uint8_t data = shiftIn2();
    printHex(data);
  }
  else if(cmd1=='b' && cmd2=='r')
  {
    uint8_t add0 = GetByte();
    uint8_t add1 = GetByte();
    uint8_t add2 = GetByte();
    uint8_t shift = GetByte();
    uint32_t cnt = 1;
    cnt <<= shift;
    shiftOut2(0x0B);
    shiftOut2(add0);
    shiftOut2(add1);
    shiftOut2(add2);
    shiftOut2(0);
    while(cnt--)
    {
      uint8_t data = shiftIn2();
      Serial.write(data);
    }
  }
  else if(cmd1=='b' && cmd2=='w')
  {
    for(uint8_t i = 0;i<64;i++)
    {
      uint8_t data = Serial.read();
	  shiftOut2(data);
    }
  }
  else if(cmd1=='r' && cmd2=='t')
  {
    Serial.print('\n');
  }
  else if(cmd1=='t' && cmd2=='s')
  {
    Serial.print("test test\n");
    Serial.flush();
  }

}

char convt[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void printHex(uint16_t val){//"ll"+"hh"
  Serial.print(convt[((val & 0xF0) >> 4)]);
  Serial.print(convt[((val & 0x0F))]);
  Serial.print(convt[((val & 0xF000) >> 12)]);
  Serial.print(convt[((val & 0x0F00) >> 8)]);
}
void printHex(uint8_t val){//"hl"
  Serial.print(convt[((val & 0xF0) >> 4)]);
  Serial.print(convt[((val & 0x0F))]);
}

uint8_t GetByte(){
  while(!Serial.available());
  int vh = ConvBCD(Serial.read());
  while(!Serial.available());
  int vl = ConvBCD(Serial.read());
  uint8_t val = (( vh << 4 ) & 0xF0 ) | (vl & 0x0F);
  return val;
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
void wait(){
	for(uint16_t i = 0;i<10;i++){
		volatile uint8_t tmp = 0;
	}
}
void shiftOut2(byte val){
  for (uint8_t i = 0; i < 8; i++)  {
    //msb first
    if(val & 0x80)
    {
      DDRD &= ~si;//1
    }
    else
    {
      DDRD |= si;//0
    }
    val<<=1;

    DDRD |= sclk;//fall
	wait();
    DDRD &= ~sclk;//rise
	wait();
  }
}

uint8_t shiftIn2(){
  uint8_t inBits = 0;
  for (uint8_t i = 0; i < 8; i++)  {
    DDRD |= sclk;//fall
	wait();
    DDRD &= ~sclk;//rise
	wait();
    inBits <<=1;
    if(PINC & so)
    {
      inBits |= 1;
	}
	else
	{
    }
  }
  return inBits;
}
