#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
//ʵ�ֹ��ܣ���ʱ���ӣ��趨��1���ӣ�2���㱨ʱ��3������
/*
   0
1     2
   3
4     5
   6


����ɫ��ť����ҳ��ʱ�䣬���ڣ�����
���������Ҽ��󣬽����趨״̬��10���޲�����������ʾ״̬

80 red
1 up
2 down
8 left
4 right


LED 10
LED 420 ��10+41��
PNP 6 NPN 6
NPN 7
���ϸߣ����µ�

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


//���͸ߣ����͵�
//��1	��2	��3	��4	��5	��6	
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
0x00 //0000 0000  10 ��
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
void DrawDateTime(uint8_t* p,uint8_t AdjFlashIdx,volatile uint8_t* pflash);
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


volatile uint8_t wordArray[6];//0~9 ��ʾ��Ӧ���֣�10ȫ��
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


//0null,1 week,2/3 year,4/5 month,6/7 day,8/9 hour,10/11 minute,12/13 second Ĭ�ϣ�11��
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
  DDRA=_BV(1)|_BV(2)|_BV(3)|_BV(4)|_BV(6);
  DDRB=_BV(0)|_BV(2);
  PORT_OE_ON;
  //SendByte(0);
  //SendByte(0);
  //PORT_STR_ON; //store clock up
  //PORT_STR_OFF; //store clock down

  //ADC��ʼ��
  ADCSRB |= _BV(ADLAR);
  //ADMUX = 0;//Initial Value 0 0 0 0 0 0 0 0
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
  
  //ˢ�¶�ʱ����ʼ��
  //TCCR0A = 0;//Initial Value 0 0 0 0 0 0 0 0
  TCCR0B = 2;
  TCNT0 = 0;
  OCR0A = 128;//����Խ��Խ����match�Ժ�OE����ʱ����ʱ��OE��
  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);
  
  //TCCR1A = 0;//Initial Value 0 0 0 0 0 0 0 0
  TCCR1B = 5;//  1/1024 7812.5hz/0.119hz(0.000128s/8.4s)
  //TCCR1C = 0;//Initial Value 0 0 0 0 0 0 0 0
  //TIMSK1 = 0;//Initial Value 0 0 0 0 0 0 0 0
  
  //I2C����Ҫ��ʼ��
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
        DrawDateTime(&DS1307_DATA[DS1307_HR],1,wordArray);
        LEDLowSign =  LEDTimeL;
        LEDHighSign = LEDTimeH;

        ProcInput();
        Alarm();
      }
      break;
      case 1:
      {
        DS1307_read();
        DrawDateTime(&DS1307_DATA[DS1307_YR],1,wordArray);
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
      case 3://ȫled���� ���Ӳ���
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
          Alarm(inputdata);
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
  
  if(currTick>=2000)//0.5s ��˸
  {
    AdjFlashIdx=~AdjFlashIdx;
    TCNT1 = 0;TIFR1 |= _BV(TOV1);
  }

  if(CurAdj==DS1307_DOW_ADJ0 || CurAdj==DS1307_SET_ADJ0 || CurAdj>=AlarmMIN_ADJ1 && CurAdj<=AlarmHR_ADJ0)
  {
    uint8_t valAdj = 0;
    uint8_t* partAdj;

    if(CurAdj==DS1307_DOW_ADJ0)
    {
      partAdj = &DS1307_DATA[DS1307_DOW];
      valAdj = 1;
    }
    else if(CurAdj==DS1307_SET_ADJ0)
    {
      partAdj = &DS1307_DATA[DS1307_SET];
      valAdj = 1;
    }
    else if(CurAdj==AlarmHR_ADJ0)
    {
      partAdj = &DS1307_DATA[AlarmHR];
      valAdj = 0x10;
    }
    else if(CurAdj==AlarmHR_ADJ1)
    {
      partAdj = &DS1307_DATA[AlarmHR];
      valAdj = 1;
    }
    else if(CurAdj==AlarmMIN_ADJ0)
    {
      partAdj = &DS1307_DATA[AlarmMIN];
      valAdj = 0x10;
    }
    else if(CurAdj==AlarmMIN_ADJ1)
    {
      partAdj = &DS1307_DATA[AlarmMIN];
      valAdj = 1;
    }
    
    if(inputdata==KeyUp)
    {
      *partAdj+=valAdj;
    }
    if(inputdata==KeyDown)
    {
      *partAdj-=valAdj;
    }
    
    DrawWeek();
  }
  else
  {
    uint8_t valAdj = 0;
    uint8_t* partAdj;

    partAdj = &DS1307_DATA[CurAdj>>1];
    
    if(CurAdj&1)
    {
      valAdj = 0x10;
    }
    else
    {
      valAdj = 1;
    }
    
    if(inputdata==KeyUp)
    {
      *partAdj+=valAdj;
    }
    if(inputdata==KeyDown)
    {
      *partAdj-=valAdj;
    }
    
    if(CurAdj>=DS1307_DATE_ADJ0 && CurAdj<=DS1307_YR_ADJ1)
    {
      DrawDateTime(&DS1307_DATA[DS1307_YR],AdjFlashIdx,&wordArray[DS1307_YR_ADJ1-CurAdj]);
      LEDLowSign  = LEDDateL;
      LEDHighSign = LEDDateH;
    }
    else if(CurAdj>=DS1307_SEC_ADJ0 && CurAdj<=DS1307_HR_ADJ1)
    {
      DrawDateTime(&DS1307_DATA[DS1307_HR],AdjFlashIdx,&wordArray[DS1307_HR_ADJ1-CurAdj]);
      LEDLowSign  = LEDTimeL;
      LEDHighSign = LEDTimeH;
    }
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
        Alarm(AlarmCnt);
      }
    }
  }
  else if(DS1307_DATA[DS1307_SET]==2)
  {
    if(DS1307_DATA[DS1307_SEC]==0)
    {
      if(DS1307_DATA[DS1307_MIN]==DS1307_DATA[AlarmMIN] && DS1307_DATA[DS1307_HR]==DS1307_DATA[AlarmHR])
      {
        Alarm(1);
      }
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

void DrawDateTime(uint8_t* p,uint8_t AdjFlashIdx,volatile uint8_t* pflash){
  volatile uint8_t* p2 = wordArray;
  for(uint8_t i=0;i<3;i++)
  {
    //���Сʱ�ĵ�һλ�ÿ�
    if(i==0)
    {
      if((AdjFlashIdx || p2!=pflash)&&((*p)>>4))
      {
        *p2 = (*p)>>4;
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
        *p2 = (*p)>>4;
      }
      else
      {
        *p2 = EMPTY;
      }
    }
    p2++;
    if(AdjFlashIdx || p2!=pflash)
    {
      *p2 = (*p)&0x0F;
    }
    else
    {
      *p2 = EMPTY;
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
  wordArray[4] = (AdjFlashIdx && CurAdj==DS1307_SET_ADJ0)?EMPTY:DS1307_DATA[DS1307_SET];
  wordArray[5] = (AdjFlashIdx && CurAdj==DS1307_DOW_ADJ0)?EMPTY:DS1307_DATA[DS1307_DOW];
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
  PORT_OE_ON;//�ر�OE,��ʼ����
  SendByte(highSign);
  SendByte(lowSign);
  PORT_STR_ON;
  PORT_STR_OFF;
  sei();
  
  //׼��������� ��ʼ
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

  //�����һ�����������ּ��л�
  prog_uint8_t* p = Up6+(wordCount<<1);
  lowSign  |= pgm_read_byte_near(p++);
  highSign |= pgm_read_byte_near(p);
  if(pgm_read_byte_near(Down7B+wordArray[wordCount])&(0x80>>lineCount))
  {
    p = Down7A+(lineCount<<1);
    lowSign  |= pgm_read_byte_near(p++);
    highSign |= pgm_read_byte_near(p);
  }

  if(wordCount==0&&lineCount==0)//��ʾʱ��:����-
  {
    lowSign  |= LEDLowSign;
    highSign |= LEDHighSign;
    OCR0A = ADCH;ADCSRA |= _BV(ADSC);//��������
  }
  //׼��������� ����
}

ISR(TIM0_COMPA_vect){
  PORT_OE_OFF;//�����
}


#define DS1307_CTRL_ID 0x68//B01101000  //DS1307

#define ADDRESS DS1307_CTRL_ID

// sets SCL low and drives output PORT_SCL &= ~BIT_SCL; 
#define i2c_scl_lo() DDR_SCL |= BIT_SCL;
// sets SDA low and drives output PORT_SDA &= ~BIT_SDA; 
#define i2c_sda_lo() DDR_SDA |= BIT_SDA;
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
//address�ǲ��������̶�ֵ��ʡ�ռ�
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
  i2c_writebit(ack);
  dly();
  return res;
}

void DS1307_read(){
  i2c_beginTransmission();
  i2c_write(0x00);
  i2c_endTransmission();
  i2c_requestFrom();
  for(uint8_t i = 0;i<15;i++)
  {
    DS1307_DATA[i] = i2c_read(0);// 0
  }
  DS1307_DATA[15] = i2c_read(1);// 0
}

void DS1307_save(){
  i2c_beginTransmission();
  i2c_write(0x00); // reset register pointer
  for(uint8_t i = 0;i<16;i++)
  {
    i2c_write(DS1307_DATA[i]);
  }
  i2c_endTransmission();
}