#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/*
   0
1     2
   3
4     5
   6
dataB0 clkA1 stA2 oeB2  2A 2B
 sdaB0 sclB1            1B
adj 4 5 6 7 (digital in)4A // 调整月日时分
light sense A0           1A
A3 free (Page)
11

74HC595(1):1~7(7) down 0 alarm
74HC595(2):2~7(6) up 0:1 time index date index
74HC595(3) week
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


LED 10
LED 420 （10+41）
PNP 6 NPN 6
NPN 7
左上高，右下低

*/

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
0x7E,//0111 1110, A 
0x2F,//0010 1111, B 
0x65,//0110 0101, C 
0x1F,//0001 1111, D 
0x6D,//0110 1101, E 
0x6C,//0110 1100  F 
0x00 //0000 0000    
};

//(LED1)	(LED2)
//(B7)	(A1)
//0	E
//0x0001	0x4000
#define LED1L 0x01
#define LED1H 0x00
#define LED2L 0x00
#define LED2H 0x40

//alarm
//B0
//7
//0x0080
#define AlarmL 0x80
#define AlarmH 0x00



void ClockInit();

void loop();
void Init();
void Page(uint8_t* vals);
void SendByte(uint8_t data);
uint8_t get165();

void dly();
void i2c_SoftI2CMaster();
void i2c_SoftI2CMasterEnd();
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

uint8_t bcdToDec(const uint8_t bcd);
uint8_t decToBcd(const uint8_t dec);
void DS1307_read();
void DS1307_save();

volatile uint8_t wordArray[6];//0~15 显示相应数字，16全暗
volatile uint8_t wordCount = 0;
volatile uint8_t lineCount = 0;
volatile uint8_t lowSign = 0;
volatile uint8_t highSign = 0;
volatile uint8_t LEDLowSign = 0;
volatile uint8_t LEDHighSign = 0;
volatile uint8_t AllowGetTime = 0;
volatile uint8_t analogRead = 0;

volatile uint8_t DS1307_SEC;// 0
volatile uint8_t DS1307_MIN;// 1
volatile uint8_t DS1307_HR;// 2
volatile uint8_t DS1307_DOW;// 3
volatile uint8_t DS1307_DATE;// 4
volatile uint8_t DS1307_MTH;// 5
volatile uint8_t DS1307_YR;// 6

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
  wordArray[0] = 10;
  wordArray[1] = 11;
  wordArray[2] = 12;
  wordArray[3] = 13;
  wordArray[4] = 14;
  wordArray[5] = 15;
  
  DDRB|=_BV(0);
  DDRA|=_BV(1);
  DDRA|=_BV(2);
  DDRB|=_BV(2);
  PORT_OE_ON;
  PORT_DAT_OFF;
  for(uint8_t i=0;i<16;i++)
  {
    PORT_CLK_ON; //shift clock up
    PORT_CLK_OFF; //shift clock down
  }
  PORT_STR_ON; //store clock up
  PORT_STR_OFF; //store clock down

  ADCSRB |= _BV(ADLAR);
  ADMUX = 0;
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
  
  TCCR0A = 0;
  TCCR0B = 2;
  TCNT0 = 0;
  OCR0A = 128;//数字越大越暗（match以后开OE，定时器超时关OE）
  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);
  sei();
}

void loop() {
	while(true)
	{
    //if(AllowGetTime>13)//13次每秒
    //{
      //cli();
      //DS1307_read();
      //sei();
      uint8_t data1 = get165();
      
      wordArray[0] = data1&0x0F;//DS1307_SEC/10;
      wordArray[1] = data1>>4;//DS1307_SEC%10;
      //wordArray[2] = analogRead/100;//DS1307_MIN%10;
      //wordArray[3] = analogRead/100;//DS1307_MIN/10;
      //wordArray[4] = analogRead/100;//DS1307_HR%10;
      //wordArray[5] = analogRead/100;//DS1307_HR/10;

      //
      
      AllowGetTime = 0;//取完时间后关闭窗口
    //}
    
    if(analogRead>128)
    {
      LEDLowSign  = LED1L;
      LEDHighSign = LED1H;
    }
    else
    {
      LEDLowSign  = LED2L;
      LEDHighSign = LED2H;
    }
    
    
	}
}

inline void SendByte(uint8_t data){
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

uint8_t get165()
{
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

ISR(TIM0_OVF_vect)
{
  PORT_OE_ON;//关闭OE,开始传输
  SendByte(highSign);
  SendByte(lowSign);
  PORT_STR_ON;
  PORT_STR_OFF;
	sei();
  ///////////////////////////一笔已画完,数据线空出/////////////////////
  if(wordCount==5&&lineCount==7)//最后一字已画完,可以取时间了
  {
    analogRead = ADCH;ADCSRA |= _BV(ADSC);
    OCR0A = analogRead;
    AllowGetTime++;
  }
  
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
  if(lineCount)//多余的一行用于字与字间切换
  {
    prog_uint8_t* p = Up6+(wordCount<<1);
    lowSign  |= pgm_read_byte_near(p++);
    highSign |= pgm_read_byte_near(p);
    ////////////////////////////////////////////////////
    if(pgm_read_byte_near(Down7B+wordArray[wordCount])&(0x80>>lineCount))
    {
      p = Down7A+(lineCount<<1);
      lowSign  |= pgm_read_byte_near(p++);
      highSign |= pgm_read_byte_near(p);
    }
    ////////////////////////////////////////////////////
    /*p = Down7+(wordArray[wordCount]<<4)+(lineCount<<1);
    lowSign  |= pgm_read_byte_near(p++);
    highSign |= pgm_read_byte_near(p);*/
    ////////////////////////////////////////////////////
  }
  if(wordCount==0&&lineCount==0)//显示时间:日期-
  {
    lowSign  |= LEDLowSign;
    highSign |= LEDHighSign;
  }
  //准备输出数据 结束
}

ISR(TIM0_COMPA_vect)
{
  PORT_OE_OFF;//打开输出
}

