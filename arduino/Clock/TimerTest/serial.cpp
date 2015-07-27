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

LED 10
LED 420 （10+41）
PNP 6 NPN 6
NPN 7
左上高，右下低

*/

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define PORT_DAT_ON  PORTB |=  _BV(0)
#define PORT_DAT_OFF PORTB &= ~_BV(0)

#define PORT_CLK_ON  PORTA |=  _BV(1)
#define PORT_CLK_OFF PORTA &= ~_BV(1)

#define PORT_STR_ON  PORTA |=  _BV(2)
#define PORT_STR_OFF PORTA &= ~_BV(2)

#define PORT_OE_ON   PORTB |=  _BV(2)
#define PORT_OE_OFF  PORTB &= ~_BV(2)

#define PIN_ADJU PINA &= ~_BV(3)
#define PIN_ADJD PINA &= ~_BV(5)
#define PIN_ADJL PINA &= ~_BV(7)
#define PIN_ADJR PINA &= ~_BV(4)


#define DS1307_CTRL_ID 0x68//B01101000  //DS1307

//scl0 sda1
#define BIT_SCL  _BV(1)
#define DDR_SCL  DDRB
#define PORT_SCL PORTB

#define BIT_SDA  _BV(0)
#define DDR_SDA  DDRB
#define PORT_SDA PORTB
#define PIN_SDA  PINB

#define I2C_ACK 1
#define I2C_NAK 0
#define i2c_scl_release() DDR_SCL &= ~BIT_SCL
#define i2c_sda_release() DDR_SDA &= ~BIT_SDA
// sets SCL low and drives output
#define i2c_scl_lo() PORT_SCL &= ~BIT_SCL; DDR_SCL |= BIT_SCL;
// sets SDA low and drives output
#define i2c_sda_lo() PORT_SDA &= ~BIT_SDA; DDR_SDA |= BIT_SDA;
// set SCL high and to input (releases pin) (i.e. change to input,turnon pullup)
#define i2c_scl_hi() DDR_SCL &=~ BIT_SCL;
// set SDA high and to input (releases pin) (i.e. change to input,turnon pullup)
#define i2c_sda_hi() DDR_SDA &=~ BIT_SDA;

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
  
//下1	下2	下3	下4	下5	下6	下7
//0x0020	0x0008	0x0002	0x2000	0x1000	0x0400	0x0100
PROGMEM prog_uint8_t Down7[] = {
0x20,0x00,	0x08,0x00,	0x02,0x00,	0x00,0x00,	0x00,0x10,	0x00,0x04,	0x00,0x01,
0x00,0x00,	0x00,0x00,	0x02,0x00,	0x00,0x00,	0x00,0x00,	0x00,0x04,	0x00,0x00,
0x20,0x00,	0x00,0x00,	0x02,0x00,	0x00,0x20,	0x00,0x10,	0x00,0x00,	0x00,0x01,
0x20,0x00,	0x00,0x00,	0x02,0x00,	0x00,0x20,	0x00,0x00,	0x00,0x04,	0x00,0x01,
0x00,0x00,	0x08,0x00,	0x02,0x00,	0x00,0x20,	0x00,0x00,	0x00,0x04,	0x00,0x00,
0x20,0x00,	0x08,0x00,	0x00,0x00,	0x00,0x20,	0x00,0x00,	0x00,0x04,	0x00,0x01,
0x20,0x00,	0x08,0x00,	0x00,0x00,	0x00,0x20,	0x00,0x10,	0x00,0x04,	0x00,0x01,
0x20,0x00,	0x00,0x00,	0x02,0x00,	0x00,0x00,	0x00,0x00,	0x00,0x04,	0x00,0x00,
0x20,0x00,	0x08,0x00,	0x02,0x00,	0x00,0x20,	0x00,0x10,	0x00,0x04,	0x00,0x01,
0x20,0x00,	0x08,0x00,	0x02,0x00,	0x00,0x20,	0x00,0x00,	0x00,0x04,	0x00,0x01
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

//PROGMEM prog_uint8_t SEGS[] = {
//0x77,//0=01110111=
//0x12,//1=00010010=
//0x5D,//2=01011101=
//0x5B,//3=01011011=
//0x3A,//4=00111010=
//0x6B,//5=01101011=
//0x6F,//6=01101111=
//0x52,//7=01010010=
//0x7F,//8=01111111=
//0x7B,//9=01111011=
//};




void ClockInit();

void loop();
void Init();
void Page(uint8_t* vals);
void SendByte(uint8_t data);

void dly();
void i2c_SoftI2CMaster();
uint8_t i2c_beginTransmission(uint8_t address);
uint8_t i2c_requestFrom(uint8_t address);
uint8_t i2c_endTransmission(void);
void i2c_write(uint8_t* data, uint8_t quantity);
void i2c_writebit(uint8_t c);
uint8_t i2c_readbit(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write(uint8_t c);
uint8_t i2c_read(uint8_t ack);
uint8_t i2c_read();
uint8_t i2c_readLast();

uint8_t bcdToDec(const uint8_t bcd);
uint8_t decToBcd(const uint8_t dec);
void DS1307_read();
void DS1307_save();

uint8_t wordArray[6];
uint8_t wordCount = 0;
uint8_t lineCount = 0;
uint8_t lowSign = 0;
uint8_t highSign = 0;
uint8_t LEDLowSign = 0;
uint8_t LEDHighSign = 0;
uint8_t AllowGetTime = 0;

uint8_t DS1307_SEC;// 0
uint8_t DS1307_MIN;// 1
uint8_t DS1307_HR;// 2
uint8_t DS1307_DOW;// 3
uint8_t DS1307_DATE;// 4
uint8_t DS1307_MTH;// 5
uint8_t DS1307_YR;// 6

uint8_t iicDelay = 100;

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
  wordArray[0] = 1;
  wordArray[1] = 2;
  wordArray[2] = 3;
  wordArray[3] = 4;
  wordArray[4] = 5;
  wordArray[5] = 6;

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
  TCCR0B = 5;//1/1024
  TCNT0 = 0;
  OCR0A = 128;//数字越大越暗（match以后开OE，定时器超时关OE）
  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);
}

void loop() {
	for(;;)
	{
    if(AllowGetTime)
    {
      uint8_t aread = ADCH;ADCSRA |= _BV(ADSC);
      wordArray[2] = aread%10;aread/=10;
      wordArray[1] = aread%10;aread/=10;
      wordArray[0] = aread%10;
      AllowGetTime = 0;//取完时间后关闭窗口
  //i2c_SoftI2CMaster();
	//DS1307_read();
	//DS1307_save();
    }
      
      //LEDLowSign  = LED1L;
      //LEDHighSign = LED1H;
      //LEDLowSign  = LED2L;
      //LEDHighSign = LED2H;
    
	}
}

inline void SendByte(uint8_t data){
  //uint8_t _data = ~data;
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
    //if(data&1)
    //{
    //  PORT_DAT_ON;
    //}
    //if(_data&1)
    //{
    //  PORT_DAT_OFF;
    //}
    data>>=1;
    //_data>>=1;
    PORT_CLK_ON; //shift clock up
    PORT_CLK_OFF; //shift clock down
  }
}

ISR(TIM0_OVF_vect)
{
  if(wordCount==0&&lineCount==0)//开始画第一字第一笔，不可以取时间
  {
    AllowGetTime = 0;
  }
  PORT_OE_ON;//关闭OE,开始传输
  SendByte(highSign);
  SendByte(lowSign);
  PORT_STR_ON;
  PORT_STR_OFF;
  ///////////////////////////一笔已画完,数据线空出/////////////////////
  if(wordCount==5&&lineCount==7)//最后一字已画完,可以取时间了
  {
    AllowGetTime = 1;
  }
  
  //准备输出数据开始
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
  if(lineCount!=7)//多余的一行用于字与字间切换
  {
    lowSign  |= pgm_read_byte_near(Up6+(wordCount<<1));
    highSign |= pgm_read_byte_near(Up6+(wordCount<<1)+1);
    
    lowSign  |= pgm_read_byte_near(Down7+wordArray[wordCount]*14+(lineCount<<1));
    highSign |= pgm_read_byte_near(Down7+wordArray[wordCount]*14+(lineCount<<1)+1);
  }
  if(wordCount==0)//显示时间日期
  {
    lowSign  |= LEDLowSign;
    highSign |= LEDHighSign;
  }
  //准备输出数据结束
  

}

ISR(TIM0_COMPA_vect)
{
  PORT_OE_OFF;//打开输出
}




void dly(){
  for(uint8_t i=0;i<iicDelay;i++)
  {
    volatile uint8_t v=0;
    v++;
  }
}
void i2c_SoftI2CMaster(){
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
}
uint8_t i2c_beginTransmission(uint8_t address){
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 0); // clr read bit
  return rc;
}
uint8_t i2c_requestFrom(uint8_t address){
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 1); // set read bit
  return rc;
}
uint8_t i2c_endTransmission(void){
  i2c_stop();
  //return ret; // FIXME
  return 0;
}
void i2c_write(uint8_t* data, uint8_t quantity){
  for(uint8_t i = 0; i < quantity; ++i){
    i2c_write(data[i]);
  }
}
void i2c_writebit(uint8_t c){
  if ( c > 0 ) {
    i2c_sda_hi();
  } 
  else {
    i2c_sda_lo();
  }
  i2c_scl_hi();
  dly();
  i2c_scl_lo();
  dly();
  if ( c > 0 ) {
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
  return ( c & BIT_SDA) ? 1 : 0;
}
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
void i2c_stop(void){
  i2c_scl_hi();
  dly();
  i2c_sda_hi();
  dly();
}
uint8_t i2c_write(uint8_t c){
  for ( uint8_t i=0;i<8;i++) {
    i2c_writebit( c & 128 );
    c<<=1;
  }
  return i2c_readbit();
}
uint8_t i2c_read(uint8_t ack){
  uint8_t res = 0;
  for ( uint8_t i=0;i<8;i++) {
    res <<= 1;
    res |= i2c_readbit();
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








uint8_t bcdToDec(const uint8_t bcd) {
  return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}
uint8_t decToBcd(const uint8_t dec) {
  const uint8_t tens = dec / 10;
  const uint8_t ones = dec % 10;
  return (tens << 4) | ones;
}

void DS1307_read(){
  i2c_beginTransmission(DS1307_CTRL_ID);
  i2c_write(0x00);
  i2c_endTransmission();
  i2c_requestFrom(DS1307_CTRL_ID);
  DS1307_SEC=bcdToDec(i2c_read());// 0
  DS1307_MIN=bcdToDec(i2c_read());// 1
  DS1307_HR=bcdToDec(i2c_read());// 2
  DS1307_DOW=i2c_read();// 3
  DS1307_DATE=bcdToDec(i2c_read());// 4
  DS1307_MTH=bcdToDec(i2c_read());// 5
  DS1307_YR=bcdToDec(i2c_readLast());// 6
}

void DS1307_save(){
  i2c_beginTransmission(DS1307_CTRL_ID);
  i2c_write(0x00); // reset register pointer
  i2c_write(decToBcd(DS1307_SEC));
  i2c_write(decToBcd(DS1307_MIN));
  i2c_write(decToBcd(DS1307_HR));
  i2c_write(DS1307_DOW);
  i2c_write(decToBcd(DS1307_DATE));
  i2c_write(decToBcd(DS1307_MTH));
  i2c_write(decToBcd(DS1307_YR));
  i2c_endTransmission();
}



void loopa()
{
  //digitalWrite(13, HIGH);
  //DS1307_read();
  //char buf[60];
  //snprintf(buf, sizeof(buf), "DS1307:%02d 20%02d-%02d-%02d %02d:%02d:%02d",DS1307_DOW, DS1307_YR, DS1307_MTH, DS1307_DATE, DS1307_HR, DS1307_MIN, DS1307_SEC);
  //Serial.println(buf);
  //if(DS1307_SEC!=80)
  //{
  //  digitalWrite(13, LOW);
  //}
  //int start = Serial.parseInt();
  //if(start == 33)
  //{
  //  DS1307_YR  = Serial.parseInt();
  //  DS1307_MTH = Serial.parseInt();
  //  DS1307_DATE = Serial.parseInt();
  //  DS1307_HR  = Serial.parseInt();
  //  DS1307_MIN = Serial.parseInt();
  //  DS1307_SEC = Serial.parseInt();
  //  DS1307_DOW= Serial.parseInt();
  //
  //  DS1307_save();
  //}
}
