#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

//实现功能：定时闹钟，设定（1闹钟，2整点报时，3静音）
/*
   0
1     2
   3
4     5
   6
  asm volatile("break\n");


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
void DrawDateTime(uint8_t* p,volatile uint8_t* pflash);
void DrawWeek();
void Alarm();
void AlarmP(uint8_t AlarmCnt);
void Adjust();

void dly();
void i2c_SoftI2CMaster();
void i2c_beginTransmission();
void i2c_requestFrom();
void i2c_endTransmission();
void i2c_writebit(uint8_t c);
void i2c_start();
void i2c_write(uint8_t c);
uint8_t i2c_readbit();
uint8_t i2c_readack(uint8_t ack);
uint8_t i2c_read();
uint8_t i2c_readLast();

void DS1307_read();
void DS1307_save();


volatile uint8_t wordArray[6];//0~9 显示相应数字，10全暗
volatile uint8_t LEDLowSign = 0;
volatile uint8_t LEDHighSign = 0;
uint8_t wordCount = 0;
uint8_t lineCount = 0;
uint8_t lineCountBit = 0x80;
uint8_t lowSign;
uint8_t highSign;
uint8_t inputdata;
uint8_t status = 3;
uint8_t CurAdj;
uint8_t AdjFlashIdx = 0;
uint8_t LastInputData = 0;


//0null,1 week,2/3 year,4/5 month,6/7 day,8/9 hour,10/11 minute,12/13 second 默认（11）
uint8_t DS1307_DATA[16];
#define DS1307_SEC      0
#define DS1307_MIN      1
#define DS1307_HR       2
#define DS1307_DOW      3
#define DS1307_DATE     4
#define DS1307_MTH      5
#define DS1307_YR       6
#define DS1307_CONTROL  7
#define DS1307_SET      8
#define AlarmMIN        9
#define AlarmHR         10

#define DS1307_SEC_ADJ0    0
#define DS1307_SEC_ADJ1    1
#define DS1307_MIN_ADJ0    2
#define DS1307_MIN_ADJ1    3
#define DS1307_HR_ADJ0     4
#define DS1307_HR_ADJ1     5

#define DS1307_DOW_ADJ0    6
#define DS1307_SET_ADJ0    7

#define DS1307_DATE_ADJ0   8
#define DS1307_DATE_ADJ1   9
#define DS1307_MTH_ADJ0    10
#define DS1307_MTH_ADJ1    11
#define DS1307_YR_ADJ0     12
#define DS1307_YR_ADJ1     13

#define AlarmMIN_ADJ1      14
#define AlarmMIN_ADJ0      15
#define AlarmHR_ADJ1       16
#define AlarmHR_ADJ0       17

PROGMEM prog_uint8_t CheckTable[] = {
//if((DS1307_DATA[DS1307_DOW]&0x0F)==0x08)DS1307_DATA[DS1307_DOW]-=0x07;
DS1307_DOW,0x0F,0x08,0x07,
//if((DS1307_DATA[DS1307_DOW]&0x0F)==0x00)DS1307_DATA[DS1307_DOW]-=0xF9;//+0x07
DS1307_DOW,0x0F,0x00,0xF9,

//if((DS1307_DATA[DS1307_SEC]&0xF0)==0x60)DS1307_DATA[DS1307_SEC]-=0x60;
DS1307_SEC,0xF0,0x60,0x60,
//if((DS1307_DATA[DS1307_SEC]&0xF0)==0xF0)DS1307_DATA[DS1307_SEC]-=0xA0;
DS1307_SEC,0xF0,0xF0,0xA0,
//if(((*partAdj)&0x0F)==0x0F)(*partAdj)-=0xF6;//n0-- = 0xnF n--
DS1307_SEC,0x0F,0x0F,0xF6,
//if(((*partAdj)&0x0F)==0x0A)(*partAdj)-=0x0A;
DS1307_SEC,0x0F,0x0A,0x0A,

//if((DS1307_DATA[DS1307_MIN]&0xF0)==0x60)DS1307_DATA[DS1307_MIN]-=0x60;
DS1307_MIN,0xF0,0x60,0x60,
//if((DS1307_DATA[DS1307_MIN]&0xF0)==0xF0)DS1307_DATA[DS1307_MIN]-=0xA0;//f-a=6
DS1307_MIN,0xF0,0xF0,0xA0,
//if(((*partAdj)&0x0F)==0x0F)(*partAdj)-=0xF6;//n0-- = 0xnF n--
DS1307_MIN,0x0F,0x0F,0xF6,
//if(((*partAdj)&0x0F)==0x0A)(*partAdj)-=0x0A;
DS1307_MIN,0x0F,0x0A,0x0A,

//if((DS1307_DATA[DS1307_HR]&0xF0)==0x30)DS1307_DATA[DS1307_HR]-=0x30;
DS1307_HR,0xF0,0x30,0x30,
//if((DS1307_DATA[DS1307_HR]&0xF0)==0xF0)DS1307_DATA[DS1307_HR]-=0xD0;//f-d=2
DS1307_HR,0xF0,0xF0,0xD0,
//if(((*partAdj)&0x0F)==0x0F)(*partAdj)-=0xF6;//n0-- = 0xnF n--
DS1307_HR,0x0F,0x0F,0xF6,
//if(((*partAdj)&0x0F)==0x0A)(*partAdj)-=0x0A;
DS1307_HR,0x0F,0x0A,0x0A,

//if((DS1307_DATA[DS1307_DATE]&0xF0)==0x40)DS1307_DATA[DS1307_DATE]-=0x40;
DS1307_DATE,0xF0,0x40,0x40,
//if((DS1307_DATA[DS1307_DATE]&0xF0)==0xF0)DS1307_DATA[DS1307_DATE]-=0xC0;//f-c=3
DS1307_DATE,0xF0,0xF0,0xC0,
//if(((*partAdj)&0x0F)==0x0F)(*partAdj)-=0xF6;//n0-- = 0xnF n--
DS1307_DATE,0x0F,0x0F,0xF6,
//if(((*partAdj)&0x0F)==0x0A)(*partAdj)-=0x0A;
DS1307_DATE,0x0F,0x0A,0x0A,

//if((DS1307_DATA[DS1307_DATE]&0xF0)==0x40)DS1307_DATA[DS1307_DATE]-=0x40;
DS1307_MTH,0xF0,0x20,0x20,
//if((DS1307_DATA[DS1307_DATE]&0xF0)==0xF0)DS1307_DATA[DS1307_DATE]-=0xC0;//f-c=3
DS1307_MTH,0xF0,0xF0,0xE0,
//if(((*partAdj)&0x0F)==0x0F)(*partAdj)-=0xF6;//n0-- = 0xnF n--
DS1307_MTH,0x0F,0x0F,0xF6,
//if(((*partAdj)&0x0F)==0x0A)(*partAdj)-=0x0A;
DS1307_MTH,0x0F,0x0A,0x0A,

//if(((*partAdj)&0xF0)==0xF0)(*partAdj)-=0x60;
DS1307_YR,0xF0,0xF0,0x60,
//if(((*partAdj)&0xF0)==0xA0)(*partAdj)-=0xA0;
DS1307_YR,0xF0,0xA0,0xA0,
//if(((*partAdj)&0x0F)==0x0F)(*partAdj)-=0xF6;//n0-- = 0xnF n--
DS1307_YR,0x0F,0x0F,0xF6,
//if(((*partAdj)&0x0F)==0x0A)(*partAdj)-=0x0A;
DS1307_YR,0x0F,0x0A,0x0A,

//if((DS1307_DATA[DS1307_DOW]&0x0F)==0x08)DS1307_DATA[DS1307_DOW]-=0x07;
DS1307_SET,0x0F,0x03,0x03,
//if((DS1307_DATA[DS1307_DOW]&0x0F)==0x00)DS1307_DATA[DS1307_DOW]-=0xF9;//+0x07
DS1307_SET,0x0F,0x0F,0xFD,

//if((DS1307_DATA[DS1307_MIN]&0xF0)==0x60)DS1307_DATA[DS1307_MIN]-=0x60;
AlarmMIN,0xF0,0x60,0x60,
//if((DS1307_DATA[DS1307_MIN]&0xF0)==0xF0)DS1307_DATA[DS1307_MIN]-=0xA0;//f-a=6
AlarmMIN,0xF0,0xF0,0xA0,
//if(((*partAdj)&0x0F)==0x0F)(*partAdj)-=0xF6;//n0-- = 0xnF n--
AlarmMIN,0x0F,0x0F,0xF6,
//if(((*partAdj)&0x0F)==0x0A)(*partAdj)-=0x0A;
AlarmMIN,0x0F,0x0A,0x0A,

//if((DS1307_DATA[DS1307_HR]&0xF0)==0x30)DS1307_DATA[DS1307_HR]-=0x30;
AlarmHR,0xF0,0x30,0x30,
//if((DS1307_DATA[DS1307_HR]&0xF0)==0xF0)DS1307_DATA[DS1307_HR]-=0xD0;//f-d=2
AlarmHR,0xF0,0xF0,0xD0,
//if(((*partAdj)&0x0F)==0x0F)(*partAdj)-=0xF6;//n0-- = 0xnF n--
AlarmHR,0x0F,0x0F,0xF6,
//if(((*partAdj)&0x0F)==0x0A)(*partAdj)-=0x0A;
AlarmHR,0x0F,0x0A,0x0A,
};

int main(void) {
  ClockInit();
  Init();
  loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

void Init(){
  DDRA=_BV(1)|_BV(2)|_BV(3)|_BV(4)|_BV(6);
  DDRB=_BV(0)|_BV(2);
  PORT_OE_ON;
  //SendByte(0);                         //DEL
  //SendByte(0);                         //DEL
  //PORT_STR_ON; //store clock up        //DEL
  //PORT_STR_OFF; //store clock down     //DEL

  //ADC初始化
  ADCSRB |= _BV(ADLAR);
  //ADMUX = 0;//Initial Value 0 0 0 0 0 0 0 0  //DEL
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
  
  //刷新定时器初始化
  //TCCR0A = 0;//Initial Value 0 0 0 0 0 0 0 0 //DEL
  TCCR0B = 2;
  //TCNT0 = 0;                                 //DEL
  OCR0A = 128;//数字越大越暗（match以后开OE，定时器超时关OE）
  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);
  
  //asm volatile("break\n");
  //TCCR1A = 0;//Initial Value 0 0 0 0 0 0 0 0                      //DEL
  TCCR1B = 5;//  1/1024 7812.5hz/0.119hz(0.000128s/8.4s)          
  //TCCR1C = 0;//Initial Value 0 0 0 0 0 0 0 0                      //DEL
  //TIMSK1 = 0;//Initial Value 0 0 0 0 0 0 0 0                      //DEL

  sei();
}

void loop() {
  for(;;)
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
        DS1307_read();
        DrawDateTime(&DS1307_DATA[DS1307_HR],wordArray);
        LEDLowSign =  LEDTimeL;
        LEDHighSign = LEDTimeH;
        ProcInput();
        Alarm();
        break;
      case 1:
        DS1307_read();
        DrawDateTime(&DS1307_DATA[DS1307_YR],wordArray);
        LEDLowSign =  LEDDateL;
        LEDHighSign = LEDDateH;
        ProcInput();
        Alarm();
        break;
      case 2:
        DS1307_read();
        DrawWeek();
        ProcInput();
        Alarm();
        break;
      case 3://全led测试 打钟测试
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
          AlarmP(inputdata);
        }
        if(inputdata==KeyRED)
        {
          status=0;
        }
        break;
      default:
        Adjust();
        break;
    }
  }
}

void Adjust(){
uint8_t i;
  if(inputdata==KeyRED)
  {
    DS1307_save();
    status = 0;
    AdjFlashIdx = 0;
    return;
  }
  if(inputdata==KeyLeft)
  {
    CurAdj++;
  }
  if(inputdata==KeyRight)
  {
    CurAdj--;
  }
  if(CurAdj==255)
  {
    CurAdj = 17;
  }
  if(CurAdj==18)
  {
    CurAdj = 0;
  }
  
  if(currTick>=2000)//0.5s 闪烁
  {
    AdjFlashIdx=~AdjFlashIdx;
    TCNT1 = 0;TIFR1 |= _BV(TOV1);
  }

  uint8_t valAdj = 0;
  uint8_t* partAdj;
  uint8_t isWeek = 1;
  switch(CurAdj)
  {
    case DS1307_DOW_ADJ0:
      partAdj = &DS1307_DATA[DS1307_DOW];
      valAdj = 1;
      break;
    case DS1307_SET_ADJ0:
      partAdj = &DS1307_DATA[DS1307_SET];
      valAdj = 1;
      break;
    case AlarmHR_ADJ0:
      partAdj = &DS1307_DATA[AlarmHR];
      valAdj = 0x10;
      break;
    case AlarmHR_ADJ1:
      partAdj = &DS1307_DATA[AlarmHR];
      valAdj = 1;
      break;
    case AlarmMIN_ADJ0:
      partAdj = &DS1307_DATA[AlarmMIN];
      valAdj = 0x10;
      break;
    case AlarmMIN_ADJ1:
      partAdj = &DS1307_DATA[AlarmMIN];
      valAdj = 1;
      break;
    default:
      partAdj = &DS1307_DATA[CurAdj>>1];
      if(CurAdj&1)
      {
        valAdj = 0x10;
      }
      else
      {
        valAdj = 1;
      }
      isWeek=0;
      break;
  }

  if(inputdata==KeyUp)
  {
    *partAdj+=valAdj;
  }
  if(inputdata==KeyDown)
  {
    *partAdj-=valAdj;
  }
  
  //边界检查
	
	for(i=0;i<sizeof(CheckTable);i+=4)
	{
		if((DS1307_DATA[pgm_read_byte_near(CheckTable+i)]&pgm_read_byte_near(CheckTable+i+1))==pgm_read_byte_near(CheckTable+i+2))DS1307_DATA[pgm_read_byte_near(CheckTable+i)]-=pgm_read_byte_near(CheckTable+i+3);
	}
  
  if(isWeek)
  {
    DrawWeek();
  }
  else if(CurAdj>=DS1307_DATE_ADJ0 && CurAdj<=DS1307_YR_ADJ1)
  {
    DrawDateTime(&DS1307_DATA[DS1307_YR],&wordArray[DS1307_YR_ADJ1-CurAdj]);
    LEDLowSign  = LEDDateL;
    LEDHighSign = LEDDateH;
  }
  else if(CurAdj>=DS1307_SEC_ADJ0 && CurAdj<=DS1307_HR_ADJ1)
  {
    DrawDateTime(&DS1307_DATA[DS1307_HR],&wordArray[DS1307_HR_ADJ1-CurAdj]);
    LEDLowSign  = LEDTimeL;
    LEDHighSign = LEDTimeH;
  }
}


void Alarm(){
  if(DS1307_DATA[DS1307_SET]==1)
  {
    uint8_t AlarmCnt = 0;
    if(DS1307_DATA[DS1307_SEC]==0)
    {
      if(DS1307_DATA[DS1307_MIN]==0x30)
      {
        AlarmCnt = 1;
      }
      else if(DS1307_DATA[DS1307_MIN]==0)
      {
        AlarmCnt = pgm_read_byte_near(ClockTable + DS1307_DATA[DS1307_HR]);
      }
      if(AlarmCnt)
      {
        AlarmP(AlarmCnt);
      }
    }
  }
  else if(DS1307_DATA[DS1307_SET]==2)
  {
    if(DS1307_DATA[DS1307_SEC]==0)
    {
      if(DS1307_DATA[DS1307_MIN]==DS1307_DATA[AlarmMIN] && DS1307_DATA[DS1307_HR]==DS1307_DATA[AlarmHR])
      {
        AlarmP(1);
      }
    }
  }
}
void AlarmP(uint8_t AlarmCnt){
  uint8_t i;
  for(i=0;i<AlarmCnt;i++)
  {
    TCNT1 = 0;TIFR1 |= _BV(TOV1);
    Alarm_ON;
    while(currTick<9000);
    TCNT1 = 0;TIFR1 |= _BV(TOV1);
    Alarm_OFF;
    while(currTick<1000);
  }
}
void DrawDateTime(uint8_t* p,volatile uint8_t* pflash){
  volatile uint8_t* p2 = wordArray;
  uint8_t i;
  for(i=0;i<3;i++)
  {
    //年和小时的第一位置空
    if( (AdjFlashIdx && p2==pflash) || (i==0)&&(((*p)>>4)==0))
    {
      *p2 = EMPTY;
    }
    else
    {
      *p2 = (*p)>>4;
    }
    p2++;
    if(AdjFlashIdx && p2==pflash)
    {
      *p2 = EMPTY;
    }
    else
    {
      *p2 = (*p)&0x0F;
    }
    p2++;
    p--;
  }
}
void DrawWeek(){
  wordArray[0] = (AdjFlashIdx && CurAdj==AlarmHR_ADJ0   )?EMPTY:(DS1307_DATA[AlarmHR]>>4);
  wordArray[1] = (AdjFlashIdx && CurAdj==AlarmHR_ADJ1   )?EMPTY:(DS1307_DATA[AlarmHR]&0x0F);
  wordArray[2] = (AdjFlashIdx && CurAdj==AlarmMIN_ADJ0  )?EMPTY:(DS1307_DATA[AlarmMIN]>>4);
  wordArray[3] = (AdjFlashIdx && CurAdj==AlarmMIN_ADJ1  )?EMPTY:(DS1307_DATA[AlarmMIN]&0x0F);
  wordArray[4] = (AdjFlashIdx && CurAdj==DS1307_SET_ADJ0)?EMPTY: DS1307_DATA[DS1307_SET];
  wordArray[5] = (AdjFlashIdx && CurAdj==DS1307_DOW_ADJ0)?EMPTY: DS1307_DATA[DS1307_DOW];
  LEDLowSign = 0;
  LEDHighSign = 0;
}
void ProcInput(){
  if(currTick>=65530)
  {
    status = 0;
    AdjFlashIdx = 0;
  }
  if(inputdata & 0x0F)
  {
    status = 4;
    CurAdj = DS1307_MIN_ADJ0;
  }
  if(inputdata==KeyRED)
  {
    status++;
    status&=3;
    TCNT1 = 0;TIFR1 |= _BV(TOV1);
  }
}

void SendByte(uint8_t data){
  uint8_t i;
  for(i=0;i<8;i++)
  {
    PORT_DAT_OFF;
    if(data&1)
    {
      PORT_DAT_ON;
    }
    data>>=1;
    PORT_CLK_ON; //shift clock up
    PORT_CLK_OFF; //shift clock down
  }
}
uint8_t get165(){
  uint8_t data = 0;
  uint8_t i;
  PL_OFF;
  PL_ON;
  for(i=0;i<8;i++)
  {
    data <<= 1;
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
  lineCount+=2;
  lineCountBit>>=1;
  if(lineCount==16)
  {
    lineCount = 0;
    lineCountBit = 0x80;
    wordCount+=2;
    if(wordCount==12)
    {
      wordCount = 0;
    }
  }
  lowSign=0;
  highSign=0;

  //多余的一行用于字与字间切换
  prog_uint8_t* p = Up6+wordCount;
  lowSign  |= pgm_read_byte_near(p++);
  highSign |= pgm_read_byte_near(p);
  if(pgm_read_byte_near(Down7B+wordArray[wordCount>>1])&(lineCountBit))
  {
    p = Down7A+lineCount;
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

// sets SCL low and drives output PORT_SCL &= ~BIT_SCL; 
#define i2c_scl_lo DDR_SCL |= BIT_SCL
// sets SDA low and drives output PORT_SDA &= ~BIT_SDA; 
#define i2c_sda_lo DDR_SDA |= BIT_SDA
// set SCL high and to input (releases pin) (i.e. change to input,turnon pullup)
#define i2c_scl_hi DDR_SCL &=~ BIT_SCL
// set SDA high and to input (releases pin) (i.e. change to input,turnon pullup)
#define i2c_sda_hi DDR_SDA &=~ BIT_SDA

#define DLY asm volatile("rcall dly\n")
//#define DLY dly()

void dly(){
    asm volatile(
	"push r24 \n"
"	ldi r24,lo8(0)	 ;  i,          \n"
".Loop1:                            \n"
"	nop                             \n"
"	inc r24	 ;  i,      \n"
"	cpi r24,lo8(200)	 ;  i,      \n"
"	brne .Loop1	 ; ,                \n"
"pop r24 \n"
"	ret                             \n"
	);
}

void i2c_start(void){
  // set both to high at the same time
  i2c_sda_hi;
  i2c_scl_hi;
  DLY;
  i2c_sda_lo;
  DLY;
  i2c_scl_lo;
  DLY;
}
//address非参数化，固定值，省空间
void i2c_beginTransmission(){
  i2c_start();
  i2c_write((ADDRESS<<1) | 0); // clr read bit
}
void i2c_requestFrom(){
  i2c_start();
  i2c_write((ADDRESS<<1) | 1); // set read bit
}
void i2c_endTransmission(void){
  i2c_scl_hi;
  DLY;
  i2c_sda_hi;
  DLY;
}
void i2c_writebit(uint8_t c){
  if (c) {
    i2c_sda_hi;
  } 
  else {
    i2c_sda_lo;
  }
  i2c_scl_hi;
  DLY;
  i2c_scl_lo;
  DLY;
  if (c) {
    i2c_sda_lo;
  }
  DLY;
}
uint8_t i2c_readbit(void){
  i2c_sda_hi;
  i2c_scl_hi;
  DLY;
  uint8_t c = PIN_SDA & BIT_SDA; // I2C_PIN;
  i2c_scl_lo;
  DLY;
  return c;
}
void i2c_write(uint8_t c){
  uint8_t i;
  for (i=0;i<8;i++) {
    i2c_writebit( c & 128 );
    c<<=1;
  }
  i2c_readbit();
}
uint8_t i2c_readack(uint8_t ack){
  uint8_t res = 0;
  uint8_t i;
  for (i=0;i<8;i++) {
    res <<= 1;
    if(i2c_readbit())
    {
    res |= 1;
    }
  }
  i2c_writebit(ack);
  DLY;
  return res;
}
void DS1307_read(){
  uint8_t i;
  i2c_beginTransmission();
  i2c_write(0x00);
  i2c_endTransmission();
  i2c_requestFrom();
  for(i = 0;i<15;i++)
  {
    DS1307_DATA[i] = i2c_readack(0);// 0
  }
  DS1307_DATA[15] = i2c_readack(1);// 0
}
void DS1307_save(){
  uint8_t i;
  i2c_beginTransmission();
  i2c_write(0x00); // reset register pointer
  for(i = 0;i<16;i++)
  {
    i2c_write(DS1307_DATA[i]);
  }
  i2c_endTransmission();
}
