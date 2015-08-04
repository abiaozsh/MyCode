#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/*
   0
1     2
   3
4     5
   6
直接用6个8显示星期 或者用秒的最后一位表示星期
星期1 8
星期2 88
星期3 888
星期4 8888
星期5 88888
星期6 888888
星期天 

按红色按钮，翻页：时间，日期，星期
按上下左右键后，进入设定状态，10秒无操作，返回显示状态

80 red
1 up
2 down
8 left
4 right


LED 10
LED 420 （10+41）
PNP 6 NPN 6
NPN 7
左上高，右下低

*/
#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

#define PORT_DAT_ON  PORTB |=  _BV(0)
#define PORT_DAT_OFF PORTB &= ~_BV(0)

#define PORT_CLK_ON  PORTA |=  _BV(1)
#define PORT_CLK_OFF PORTA &= ~_BV(1)

#define PORT_STR_ON  PORTA |=  _BV(2)
#define PORT_STR_OFF PORTA &= ~_BV(2)

#define PORT_OE_ON   PORTB |=  _BV(2)
#define PORT_OE_OFF  PORTB &= ~_BV(2)

#define PL_ON  PORTA |=  _BV(3)
#define PL_OFF PORTA &= ~_BV(3)
#define CP_ON  PORTA |=  _BV(4)
#define CP_OFF PORTA &= ~_BV(4)
#define PINQ7  (PINA & _BV(5))

//scl0 sda1
#define BIT_SCL  _BV(1)
#define DDR_SCL  DDRB
#define PORT_SCL PORTB

#define BIT_SDA  _BV(7)
#define DDR_SDA  DDRA
#define PORT_SDA PORTA
#define PIN_SDA  PINA

#define Alarm_ON  PORTA |=  _BV(6)
#define Alarm_OFF PORTA &= ~_BV(6)

//(LED1)	(LED2)
//(B7)	(A1)
//0	E
//0x0001	0x4000
#define LEDTimeL 0x01
#define LEDTimeH 0x00
#define LEDDateL 0x00
#define LEDDateH 0x40
#define LEDDateTimeL LEDDateL|LEDTimeL
#define LEDDateTimeH LEDTimeH|LEDDateH

#define KeyRED   0x80
#define KeyUp    0x01
#define KeyDown  0x02
#define KeyLeft  0x08
#define KeyRight 0x04


//先送高，后送低
//上1	上2	上3	上4	上5	上6	
//0x0040	0x0010	0x0004	0x0800	0x0200	0x8000
PROGMEM prog_uint8_t Up6[] = {
0x40,0x00,
0x10,0x00,
0x04,0x00,
0x00,0x08,
0x00,0x02,
0x00,0x80
};

PROGMEM prog_uint8_t Down7A[] = {
0x00,0x00,
0x20,0x00,
0x08,0x00,
0x02,0x00,
0x00,0x20,
0x00,0x10,
0x00,0x04,
0x00,0x01
};

#define EMPTY 10
PROGMEM prog_uint8_t Down7B[] = {
0x77,//0111 0111, 0
0x12,//0001 0010, 1
0x5D,//0101 1101, 2
0x5B,//0101 1011, 3
0x3A,//0011 1010, 4
0x6B,//0110 1011, 5
0x6F,//0110 1111, 6
0x52,//0101 0010, 7
0x7F,//0111 1111, 8
0x7B,//0111 1011, 9
0x00 //0000 0000  10 空
};


PROGMEM prog_uint8_t ClockTable[] = {
// 00,01,02,03,04,05,06,07,08,09,0A,0B,0C,0D,0E,0F
   12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
// 10,11,12,13,14,15,16,17,18,19,1A,1B,1C,1D,1E,1F
   10,11,12, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0,
// 20,21,22,23,24,15,16,17,18,19,1A,1B,1C,1D,1E,1F
    8, 9,10,11
};


void ClockInit();
void loop();
void Init();
void SendByte(uint8_t data);
uint8_t get165();
void ProcInput();
void DrawDateTime(uint8_t** p,uint8_t AdjFlashIdx,volatile uint8_t* pflash);
void DrawWeek();
void Alarm();
void Alarm(uint8_t AlarmCnt);
void Adjust();

void dly();
void i2c_SoftI2CMaster();
void i2c_beginTransmission(uint8_t address);
void i2c_requestFrom(uint8_t address);
void i2c_endTransmission();
void i2c_writebit(uint8_t c);
void i2c_start();
void i2c_write(uint8_t c);
uint8_t i2c_readbit();
uint8_t i2c_read(uint8_t ack);
uint8_t i2c_read();
uint8_t i2c_readLast();

void DS1307_read();
void DS1307_save();


volatile uint8_t wordArray[6];//0~9 显示相应数字，10全暗
volatile uint8_t LEDLowSign = 0;
volatile uint8_t LEDHighSign = 0;
volatile uint8_t wordCount = 0;
volatile uint8_t lineCount = 0;
volatile uint8_t lowSign;
volatile uint8_t highSign;
uint8_t inputdata;
uint8_t status = 0;
uint8_t CurAdj;
uint8_t AdjFlashIdx = 0;
uint8_t LastInputData = 0;


//0null,1 week,2/3 year,4/5 month,6/7 day,8/9 hour,10/11 minute,12/13 second 默认（11）
uint8_t* DS1307_p[7];
uint8_t DS1307_SEC; // 0
uint8_t DS1307_MIN; // 1
uint8_t DS1307_HR;  // 2
uint8_t DS1307_DOW; // 3
uint8_t DS1307_DATE;// 4
uint8_t DS1307_MTH; // 5
uint8_t DS1307_YR;  // 6


int main(void) {
  ClockInit();
  Init();
  loop();
}

inline void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

inline void Init(){
  //wordArray[0] = 0;
  //wordArray[1] = 0;
  //wordArray[2] = 0;
  //wordArray[3] = 0;
  //wordArray[4] = 0;
  //wordArray[5] = 0;
  DS1307_p[0] = &DS1307_DOW;
  DS1307_p[1] = &DS1307_YR;
  DS1307_p[2] = &DS1307_MTH;
  DS1307_p[3] = &DS1307_DATE;
  DS1307_p[4] = &DS1307_HR;
  DS1307_p[5] = &DS1307_MIN;
  DS1307_p[6] = &DS1307_SEC;

  DDRA=_BV(1)|_BV(2)|_BV(3)|_BV(4)|_BV(6);
  DDRB=_BV(0)|_BV(2);
  PORT_OE_ON;
  SendByte(0);
  SendByte(0);
  PORT_STR_ON; //store clock up
  PORT_STR_OFF; //store clock down

  //ADC初始化
  ADCSRB |= _BV(ADLAR);
  ADMUX = 0;//Initial Value 0 0 0 0 0 0 0 0
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
  
  //刷新定时器初始化
  //TCCR0A = 0;//Initial Value 0 0 0 0 0 0 0 0
  TCCR0B = 2;
  TCNT0 = 0;
  OCR0A = 128;//数字越大越暗（match以后开OE，定时器超时关OE）
  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);
  
  //TCCR1A = 0;//Initial Value 0 0 0 0 0 0 0 0
  TCCR1B = 5;//  1/1024 7812.5hz/0.119hz(0.000128s/8.4s)
  //TCCR1C = 0;//Initial Value 0 0 0 0 0 0 0 0
  //TIMSK1 = 0;//Initial Value 0 0 0 0 0 0 0 0
  
  //I2C不需要初始化
  //i2c_SoftI2CMaster();

  sei();
}

inline void loop() {
	while(true)
	{
    uint8_t CurInputData = get165();
    inputdata = 0;
    if(LastInputData==0 && CurInputData!=0)
    {
      inputdata = CurInputData;
    }
    LastInputData = CurInputData;
    
    switch(status)
    {
      case 0:
      {
        DS1307_read();
        DrawDateTime(&DS1307_p[4],1,wordArray);
        LEDLowSign =  LEDTimeL;
        LEDHighSign = LEDTimeH;

        ProcInput();
        Alarm();
      }
      break;
      case 1:
      {
        DS1307_read();
        DrawDateTime(&DS1307_p[1],1,wordArray);
        LEDLowSign =  LEDDateL;
        LEDHighSign = LEDDateH;

        ProcInput();
        Alarm();
      }
      break;
      case 2:
      {
        DS1307_read();
        DrawWeek();
        
        ProcInput();
        Alarm();
      }
      break;
      case 3://全led测试 打钟测试
      {
        wordArray[0] = 8;
        wordArray[1] = 8;
        wordArray[2] = 8;
        wordArray[3] = 8;
        wordArray[4] = 8;
        wordArray[5] = 8;
        LEDLowSign  = LEDDateTimeL;
        LEDHighSign = LEDDateTimeH;
        if(inputdata & 0x0F)
        {
          Alarm(1);
        }
        if(inputdata==KeyRED)
        {
          status=0;
        }
      }
      break;
      default : //else if(status == 4)
      {
        Adjust();
      }
      break;
    }
  }
}

inline void Adjust(){
  if(inputdata==KeyRED)
  {
    DS1307_save();
    status = 0;
    return;
  }
  if(inputdata==KeyLeft)
  {
    CurAdj--;
  }
  if(inputdata==KeyRight)
  {
    CurAdj++;
  }
  if(CurAdj==0)
  {
    CurAdj = 13;
  }
  if(CurAdj==14)
  {
    CurAdj = 1;
  }
  
  if(CurAdj==1)
  {
    if(inputdata==KeyUp)
    {
      DS1307_DOW--;
    }
    if(inputdata==KeyDown)
    {
      DS1307_DOW++;
    }
    DrawWeek();
  }
  else
  {
    uint8_t valAdj = 0;
    uint8_t* partAdj;

    partAdj = DS1307_p[CurAdj>>1];
    
    if(CurAdj&1)
    {
      valAdj = 1;
    }
    else
    {
      valAdj = 0x10;
    }
    
    if(inputdata==KeyUp)
    {
      *partAdj+=valAdj;
    }
    if(inputdata==KeyDown)
    {
      *partAdj-=valAdj;
    }
    
    if(currTick>=2000)//0.5s 闪烁
    {
      AdjFlashIdx=~AdjFlashIdx;
      TCNT1 = 0;TIFR1 |= _BV(TOV1);
    }
    if(CurAdj>=2&&CurAdj<=7)
    {
      DrawDateTime(&DS1307_p[1],AdjFlashIdx,&wordArray[CurAdj-2]);
      LEDLowSign  = LEDDateL;
      LEDHighSign = LEDDateH;
    }
    else if(CurAdj>=8&&CurAdj<=13)
    {
      DrawDateTime(&DS1307_p[4],AdjFlashIdx,&wordArray[CurAdj-8]);
      LEDLowSign  = LEDTimeL;
      LEDHighSign = LEDTimeH;
    }
  }
}
void Alarm(){
  uint8_t AlarmCnt = 0;
  if(DS1307_SEC==0)
  {
    if(DS1307_MIN==0x30)
    {
      AlarmCnt = 1;
    }
    else if(DS1307_MIN==0)
    {
      AlarmCnt = pgm_read_byte_near(ClockTable + DS1307_HR);
    }
    if(AlarmCnt)
    {
      Alarm(AlarmCnt);
    }
  }
}
void Alarm(uint8_t AlarmCnt){
  for(uint8_t i=0;i<AlarmCnt;i++)
  {
    TCNT1 = 0;TIFR1 |= _BV(TOV1);
    Alarm_ON;
    while(currTick<9000);
    TCNT1 = 0;TIFR1 |= _BV(TOV1);
    Alarm_OFF;
    while(currTick<1000);
  }
}
void DrawDateTime(uint8_t** p,uint8_t AdjFlashIdx,volatile uint8_t* pflash){
  volatile uint8_t* p2 = wordArray;
  for(uint8_t i=0;i<3;i++)
  {
    //年的第一位置空
    if(*p==&DS1307_YR)
    {
      if((AdjFlashIdx || p2!=pflash)&&((**p)>>4))
      {
        *p2 = (**p)>>4;
      }
      else
      {
        *p2 = EMPTY;
      }
    }
    else
    {
      if(AdjFlashIdx || p2!=pflash)
      {
        *p2 = (**p)>>4;
      }
      else
      {
        *p2 = EMPTY;
      }
    }
    p2++;
    if(AdjFlashIdx || p2!=pflash)
    {
      *p2 = (**p)&0x0F;
    }
    else
    {
      *p2 = EMPTY;
    }
    p2++;
    p++;
  }
}
void DrawWeek(){
  wordArray[0] = EMPTY;
  wordArray[1] = EMPTY;
  wordArray[2] = EMPTY;
  wordArray[3] = EMPTY;
  wordArray[4] = EMPTY;
  wordArray[5] = DS1307_DOW;
  LEDLowSign = 0;
  LEDHighSign = 0;
}
void ProcInput(){
  if(currTick>=65530)
  {
    status = 0;
  }
  if(inputdata & 0x0F)
  {
    status = 4;
    CurAdj = 11;
    //TCNT1 = 0;TIFR1 |= _BV(TOV1);
  }
  if(inputdata==KeyRED)
  {
    status++;
    status&=3;
    TCNT1 = 0;TIFR1 |= _BV(TOV1);
  }
}

void SendByte(uint8_t data){
  for(uint8_t i=0;i<8;i++)
  {
    if(data&1)
    {
      PORT_DAT_ON;
    }
    else
    {
      PORT_DAT_OFF;
    }
    data>>=1;
    PORT_CLK_ON; //shift clock up
    PORT_CLK_OFF; //shift clock down
  }
}
inline uint8_t get165(){
  uint8_t data = 0;
  PL_OFF;
  PL_ON;
  for(uint8_t i=0;i<8;i++)
  {
    data = data << 1;
    if(PINQ7){
      data|=1;
    }
    CP_ON;
    CP_OFF;
  }
  return data;
}

ISR(TIM0_OVF_vect){
  PORT_OE_ON;//关闭OE,开始传输
  SendByte(highSign);
  SendByte(lowSign);
  PORT_STR_ON;
  PORT_STR_OFF;
	sei();
  
  //准备输出数据 开始
  lineCount++;
  if(lineCount==8)
  {
    lineCount = 0;
    wordCount++;
    if(wordCount==6)
    {
      wordCount = 0;
    }
  }
  lowSign=0;
  highSign=0;

  //多余的一行用于字与字间切换
  prog_uint8_t* p = Up6+(wordCount<<1);
  lowSign  |= pgm_read_byte_near(p++);
  highSign |= pgm_read_byte_near(p);
  if(pgm_read_byte_near(Down7B+wordArray[wordCount])&(0x80>>lineCount))
  {
    p = Down7A+(lineCount<<1);
    lowSign  |= pgm_read_byte_near(p++);
    highSign |= pgm_read_byte_near(p);
  }

  if(wordCount==0&&lineCount==0)//显示时间:日期-
  {
    lowSign  |= LEDLowSign;
    highSign |= LEDHighSign;
    OCR0A = ADCH;ADCSRA |= _BV(ADSC);//设置亮度
  }
  //准备输出数据 结束
}

ISR(TIM0_COMPA_vect){
  PORT_OE_OFF;//打开输出
}


#define DS1307_CTRL_ID 0x68//B01101000  //DS1307

#define ADDRESS DS1307_CTRL_ID

#define I2C_NAK 0
#define I2C_ACK 1
// sets SCL low and drives output
#define i2c_scl_lo() PORT_SCL &= ~BIT_SCL; DDR_SCL |= BIT_SCL;
// sets SDA low and drives output
#define i2c_sda_lo() PORT_SDA &= ~BIT_SDA; DDR_SDA |= BIT_SDA;
// set SCL high and to input (releases pin) (i.e. change to input,turnon pullup)
#define i2c_scl_hi() DDR_SCL &=~ BIT_SCL;
// set SDA high and to input (releases pin) (i.e. change to input,turnon pullup)
#define i2c_sda_hi() DDR_SDA &=~ BIT_SDA;


void dly(){
  for(uint8_t i=0;i<100;i++)//6 is stable
  {
    asm volatile ("nop");
  }
}
//void i2c_SoftI2CMaster(){
//  i2c_sda_hi();
//  i2c_scl_hi();
//  PORT_SCL &= ~BIT_SCL;
//  PORT_SDA &= ~BIT_SDA;
//  dly();
//}
void i2c_start(void){
  // set both to high at the same time
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
  i2c_sda_lo();
  dly();
  i2c_scl_lo();
  dly();
}
//address非参数化，固定值，省空间
void i2c_beginTransmission(){
  i2c_start();
  i2c_write((ADDRESS<<1) | 0); // clr read bit
}
inline void i2c_requestFrom(){
  i2c_start();
  i2c_write((ADDRESS<<1) | 1); // set read bit
}
void i2c_endTransmission(void){
  i2c_scl_hi();
  dly();
  i2c_sda_hi();
  dly();
}
void i2c_writebit(uint8_t c){
  if (c) {
    i2c_sda_hi();
  } 
  else {
    i2c_sda_lo();
  }
  i2c_scl_hi();
  dly();
  i2c_scl_lo();
  dly();
  if (c) {
    i2c_sda_lo();
  }
  dly();
}
uint8_t i2c_readbit(void){
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
  uint8_t c = PIN_SDA; // I2C_PIN;
  i2c_scl_lo();
  dly();
  return c & BIT_SDA;
}
void i2c_write(uint8_t c){
  for ( uint8_t i=0;i<8;i++) {
    i2c_writebit( c & 128 );
    c<<=1;
  }
  i2c_readbit();
}
uint8_t i2c_read(uint8_t ack){
  uint8_t res = 0;
  for ( uint8_t i=0;i<8;i++) {
    res <<= 1;
    if(i2c_readbit())
    {
    res |= 1;
    }
  }
  if ( ack )
    i2c_writebit( 0 );
  else
    i2c_writebit( 1 );
  dly();
  return res;
}
uint8_t i2c_read(){
  return i2c_read( I2C_ACK );
}
uint8_t i2c_readLast(){
  return i2c_read( I2C_NAK );
}

void DS1307_read(){
  i2c_beginTransmission();
  i2c_write(0x00);
  i2c_endTransmission();
  i2c_requestFrom();
  DS1307_SEC =i2c_read();// 0
  DS1307_MIN =i2c_read();// 1
  DS1307_HR  =i2c_read();// 2
  DS1307_DOW =i2c_read();// 3
  DS1307_DATE=i2c_read();// 4
  DS1307_MTH =i2c_read();// 5
  DS1307_YR  =i2c_readLast();// 6
}

void DS1307_save(){
  i2c_beginTransmission();
  i2c_write(0x00); // reset register pointer
  i2c_write(DS1307_SEC);
  i2c_write(DS1307_MIN);
  i2c_write(DS1307_HR);
  i2c_write(DS1307_DOW);
  i2c_write(DS1307_DATE);
  i2c_write(DS1307_MTH);
  i2c_write(DS1307_YR);
  i2c_endTransmission();
}
