PROGMEM prog_uint32_t IRdata1[] = {
  0x0FFF00F,//0
  0x0FF12ED,//1
  0x0FFD02F,//2
  0x0FF50AF,//3
  0x0FF1AE5,//4
  0x0FFD827,//5
  0x0FF58A7,//6
  0x0FF3AC5,//7
  0x0FF9867,//8
  0x0FF7887//9
};
PROGMEM prog_uint32_t IRdata2[] = {
  0x35A9425F,//0
  0xE35A7437,//1
  0xD538681B,//2
  0x2A89195F,//3
  0xA8E05FBB,//4
  0x86B0E697,//5
  0xDC0197DB,//6
  0xB3D4B87F,//7
  0x97483BFB,//8
  0xF63C8657//9
};

#define LEDAOUT DDRB |= _BV(0);
#define LEDBOUT DDRD |= _BV(5);
#define LEDAON  PORTB &= ~_BV(0);
#define LEDAOFF PORTB |=  _BV(0);
#define LEDBON  PORTD &= ~_BV(5);
#define LEDBOFF PORTD |=  _BV(5);

byte digit1;
byte digit2;
byte digit3;
byte digit4;
//int RECV_PIN = 15;//定义红外接收器的引脚为11

IRrecv irrecv;

decode_results results;

void InitIR()
{
  irrecv.enableIRIn(); // 初始化红外接收器
}

void InitTimer() {
  int idx = 0;
  LEDAOUT;
  LEDBOUT;
  LEDAON;
  LEDBON;
  //Serial.begin(9600);
  while(true)
  {
    if (irrecv.decode(&results)) {
      byte value = 99;
      for(int i=0;i<10;i++)
      {
        if(results.value == pgm_read_dword_near(IRdata1+i))
        {
          value = i;
        }
      }
      for(int i=0;i<10;i++)
      {
        if(results.value == pgm_read_dword_near(IRdata2+i))
        {
          value = i;
        }
      }
      //Serial.println(value);
      if(value!=99)
      {
        if(idx == 0)
        {
          digit1 = value;
          LEDAON;
          LEDBOFF;
        }
        if(idx == 1)
        {
          digit2 = value;
          LEDAOFF;
          LEDBON;
        }
        if(idx == 2)
        {
          digit3 = value;
          LEDAON;
          LEDBON;
        }
        if(idx == 3)
        {
          digit4 = value;
          LEDAOFF;
          LEDBOFF;
          hour = digit1*10+digit2;
          miniute = digit2*10+digit3;
          return;
        }
        idx++;
      }
      irrecv.resume(); // 接收下一个值
    }
  }
}

/*
1 FF12ED E35A7437
 2 FFD02F D538681B
 3 FF50AF 2A89195F
 4 FF1AE5 A8E05FBB
 5 FFD827 86B0E697
 6 FF58A7 DC0197DB
 7 FF3AC5 B3D4B87F
 8 FF9867 97483BFB
 9 FF7887 F63C8657
 0 FFF00F 35A9425F
 */







