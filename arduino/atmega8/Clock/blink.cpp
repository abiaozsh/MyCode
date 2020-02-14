#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


PROGMEM prog_uint8_t conv[] = {
  //  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
//~0xC0,~0xF9,~0xA4,~0xB0,~0x99,~0x92,~0x82,~0xF8,~0x80,~0x90,~0x88,~0x83,~0xC6,~0xA1,~0x86,~0x8E
   0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47
};

#define Max7219_pinCLKH PORTB |=  _BV(1)
#define Max7219_pinCLKL PORTB &= ~_BV(1)
#define Max7219_pinCLKO DDRB  |=  _BV(1)
#define Max7219_pinCSH  PORTB |=  _BV(2)
#define Max7219_pinCSL  PORTB &= ~_BV(2)
#define Max7219_pinCSO  DDRB  |=  _BV(2)
#define Max7219_pinDINH PORTB |=  _BV(3)
#define Max7219_pinDINL PORTB &= ~_BV(3)
#define Max7219_pinDINO DDRB  |=  _BV(3)


void Write_Max7219_byte(uint8_t DATA)
{
  uint8_t i;
  for(i=8;i>=1;i--)
  {
    Max7219_pinCLKL;
    if(DATA&0x80){
      Max7219_pinDINH;
    }else{
      Max7219_pinDINL;
    }
    DATA=DATA<<1;
    Max7219_pinCLKH;
  }
}
//-------------------------------------------
//功能：向MAX7219写入数据
//入口参数：address、dat
//出口参数：无
//说明：
void Write_Max7219(uint8_t address,uint8_t dat)
{ 
  Max7219_pinCSL;
  Write_Max7219_byte(address);           //写入地址，即数码管编号
  Write_Max7219_byte(dat);               //写入数据，即数码管显示数字 
  Max7219_pinCSH;
}
void Init_MAX7219()
{
  Max7219_pinCLKO;
  Max7219_pinCSO ;
  Max7219_pinDINO;
  Write_Max7219(0x09, 0x00);       //译码方式：BCD码
  Write_Max7219(0x0a, 0x00);       //亮度 
  Write_Max7219(0x0b, 0x07);       //扫描界限；8个数码管显示
  Write_Max7219(0x0c, 0x01);       //掉电模式：0，普通模式：1
  Write_Max7219(0x0f, 0x00);       //显示测试：1；测试结束，正常显示：0
}

void SetData_Max7219(uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3){
  Write_Max7219(1,pgm_read_byte_near(conv+ ( d0&0x0F     ) ) );
  Write_Max7219(2,pgm_read_byte_near(conv+ ((d0&0xF0)>>4 ) ) );
  Write_Max7219(3,pgm_read_byte_near(conv+ ( d1&0x0F     ) ) );
  Write_Max7219(4,pgm_read_byte_near(conv+ ((d1&0xF0)>>4 ) ) );
  Write_Max7219(5,pgm_read_byte_near(conv+ ( d2&0x0F     ) ) );
  Write_Max7219(6,pgm_read_byte_near(conv+ ((d2&0xF0)>>4 ) ) );
  Write_Max7219(7,pgm_read_byte_near(conv+ ( d3&0x0F     ) ) );
  Write_Max7219(8,pgm_read_byte_near(conv+ ((d3&0xF0)>>4 ) ) );
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//1000000 / 9600 = 104.16666666666666666666666666667
//1000000 / 9600 = 156.25
#define TCCR0_Value_9600 2
#define CNT_Value_9600 104
#define CNT_1_5_9600 156


#define TCCR0_Value TCCR0_Value_9600
#define CNT_Value CNT_Value_9600
#define CNT_1_5 CNT_1_5_9600


//pd0 rxd		usb txd
//pd1 txd		usb rxd
#define DDR_Send DDRD
#define PORT_Send PORTD
#define BIT_Send _BV(1)
#define DDR_Recv DDRD
#define PIN_Recv PIND
#define BIT_Recv _BV(0)

void SerialSend(uint8_t val){
	cli();
	TCCR0 = TCCR0_Value;
	TCNT0 = 0;
	PORT_Send &= ~BIT_Send;
  while(TCNT0<CNT_Value);TCNT0 = 0;//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit)
    {
      PORT_Send |= BIT_Send;
    }else{
      PORT_Send &= ~BIT_Send;
    }
    chkbit<<=1;
    while(TCNT0<CNT_Value);TCNT0 = 0;
	}
	PORT_Send |= BIT_Send;
  while(TCNT0<CNT_Value);TCNT0 = 0;//stopbit
	sei();
}

uint8_t SerialRead()
{
	cli();
	uint8_t val = 0;
	TCCR0 = TCCR0_Value;
	while(PIN_Recv&BIT_Recv){
	}
	TCNT0 = 0;
	while(TCNT0<CNT_1_5);TCNT0 = 0;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);
		while(TCNT0<CNT_Value);TCNT0 = 0;
	}
	sei();
	return val;
}

void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}
void TimerInit()
{
  // COM1A1 COM1A0 COM1B1 COM1B0 – – WGM11 WGM10
//CS02 CS01 CS00 Description
//0   0 0 0 No clock source (Timer/Counter stopped)
//1 8Mhz      0 0 1 clkI/O/(No prescaling)
//2 1Mhz      0 1 0 clkI/O/8 (From prescaler)
//3 1/8Mhz    0 1 1 clkI/O/64 (From prescaler)
//4 1/32Mhz   1 0 0 clkI/O/256 (From prescaler)
//5 1/128Mhz  1 0 1 clkI/O/1024 (From prescaler)
  TCCR0 = TCCR0_Value;
}

void wait(uint8_t ticks)
{
	TCNT0 = 0;//timer reset
	while(TCNT0<ticks);//1Mhz
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////












/////////////////////////////////////////////////////////////////////////////////////////
#define DDR_sclk   DDRD
#define PORT_sclk PORTD
#define BIT_sclk _BV(5)

#define DDR_io   DDRD
#define PIN_io   PIND
#define PORT_io PORTD
#define BIT_io _BV(6)

#define DDR_ce   DDRD
#define PORT_ce PORTD
#define BIT_ce _BV(7)


uint8_t Time_sec;
uint8_t Time_min;
uint8_t Time_hr;
uint8_t Time_day;
uint8_t Time_mon;
uint8_t Time_yr;
uint8_t Time_week;
/////////////////////////////////////////////////////////////////////////////////////////

void PortInit()
{
  DDR_ce |= BIT_ce;
  DDR_sclk |= BIT_sclk;
}

void wait()
{
  wait(10);
}


void writeOut(uint8_t value) {
  DDR_io |= BIT_io;
  uint8_t val = value;
  PORT_sclk &= ~BIT_sclk;
  wait();
  for (uint8_t i = 0; i < 8; i++)  {
    if(val & (1 << i))
    {
      PORT_io |= BIT_io;
    }
    else
    {
      PORT_io &= ~BIT_io;
    }
    wait();
    PORT_sclk |= BIT_sclk;
    wait();
    PORT_sclk &= ~BIT_sclk;
  }
  DDR_io &= ~BIT_io;
}

uint8_t readIn() {
  uint8_t input_value = 0;
  uint8_t bit = 0;
  PORT_sclk &= ~BIT_sclk;
  wait();
  for (int i = 0; i < 8; ++i) {
    if(PIN_io & BIT_io)
    {
      bit = 1;
    }
    else
    {
      bit = 0;
    }
    input_value |= (bit << i);
    wait();
    PORT_sclk |= BIT_sclk;
    wait();
    PORT_sclk &= ~BIT_sclk;
  }

  return input_value;
}

#define kClockBurstRead 0xBF
void ReadTime() {
  PORT_sclk &= ~BIT_sclk;
  PORT_ce |= BIT_ce;
  wait();
  writeOut(kClockBurstRead);
  Time_sec = readIn();
  Time_min = readIn();
  Time_hr = readIn();
  Time_day = readIn();
  Time_mon = readIn();
  Time_week = readIn();
  Time_yr = readIn();
  PORT_ce &= ~BIT_ce;
  wait();
}

#define kClockBurstWrite 0xBE
void WriteTime() {
  PORT_sclk &= ~BIT_sclk;
  PORT_ce |= BIT_ce;
  wait();
  writeOut(kClockBurstWrite);
  writeOut(Time_sec);
  writeOut(Time_min);
  writeOut(Time_hr);
  writeOut(Time_day);
  writeOut(Time_mon);
  writeOut(Time_week);
  writeOut(Time_yr);
  writeOut(0);
  PORT_ce &= ~BIT_ce;
  wait();
  for(uint16_t i=0;i<1000;i++){
    wait(200);
  }
}



#define PINBTN0 (PINC & _BV(5))

#define PIN2PLUS (PINC & _BV(3))
#define PIN1PLUS (PINC & _BV(2))
#define PIN0PLUS (PINC & _BV(1))


#define BellDDR DDRB |=  _BV(0);
#define BellH  PORTB |=  _BV(0);
#define BellL  PORTB &= ~_BV(0);


//////////////////////////////////////  
//////////////////////////////////////  
//////////////////////////////////////  
//////////////////////////////////////SetData_Max7219(val,val+1,val+2,val+3);

int main()
{
	TimerInit(); //do later
	PortInit();
  SerialInit();
  Init_MAX7219();  
  
  BellDDR;
  uint8_t lastPIN0PLUS = PIN0PLUS;
  uint8_t lastPIN1PLUS = PIN1PLUS;
  uint8_t lastPIN2PLUS = PIN2PLUS;
  uint16_t timer = 0;
  
	for (;;) {
    TCNT0 = 0;
    if(PINBTN0){
      if(!PIN2PLUS && lastPIN2PLUS){
          Time_hr++;
          if((Time_hr&0x0F) == 0x0A){
            Time_hr += 6;
          }
          if(Time_hr==0x24){
            Time_hr = 0;
          }
          WriteTime();
      }
      if(!PIN1PLUS && lastPIN1PLUS){
          Time_min++;
          if((Time_min&0x0F) == 0x0A){
            Time_min += 6;
          }
          if(Time_min==0x60){
            Time_min = 0;
          }
          WriteTime();
      }
      if(!PIN0PLUS && lastPIN0PLUS){
          Time_sec++;
          if((Time_sec&0x0F) == 0x0A){
            Time_sec += 6;
          }
          if(Time_sec==0x60){
            Time_sec = 1;
          }
          WriteTime();
      }
    }else{
      if(!PIN2PLUS && lastPIN2PLUS){
          Time_mon++;
          if((Time_mon&0x0F) == 0x0A){
            Time_mon += 6;
          }
          if(Time_mon==0x13){
            Time_mon = 1;
          }
          WriteTime();
      }
      if(!PIN1PLUS && lastPIN1PLUS){
          Time_day++;
          if((Time_day&0x0F) == 0x0A){
            Time_day += 6;
          }
          if(Time_day==0x32){
            Time_day = 1;
          }
          WriteTime();
      }
      if(!PIN0PLUS && lastPIN0PLUS){
          Time_hr++;
          if((Time_hr&0x0F) == 0x0A){
            Time_hr += 6;
          }
          if(Time_hr==0x24){
            Time_hr = 0;
          }
          WriteTime();
      }
    }
    lastPIN0PLUS = PIN0PLUS;
    lastPIN1PLUS = PIN1PLUS;
    lastPIN2PLUS = PIN2PLUS;
    while(TCNT0<200);
    timer++;
    //0.2s
    if(timer==1000){
      timer = 0;
      ReadTime();
      //SerialSend('0'+((Time_yr & 0xF0)>>4));
      //SerialSend('0'+(Time_yr & 0x0F));
      //SerialSend('/');
      //SerialSend('0'+((Time_mon & 0xF0)>>4));
      //SerialSend('0'+(Time_mon & 0x0F));
      //SerialSend('/');
      //SerialSend('0'+((Time_day & 0xF0)>>4));
      //SerialSend('0'+(Time_day & 0x0F));
      //SerialSend(' ');
      //SerialSend('0'+((Time_hr & 0xF0)>>4));
      //SerialSend('0'+(Time_hr & 0x0F));
      //SerialSend(':');
      //SerialSend('0'+((Time_min & 0xF0)>>4));
      //SerialSend('0'+(Time_min & 0x0F));
      //SerialSend(':');
      //SerialSend('0'+((Time_sec & 0xF0)>>4));
      //SerialSend('0'+(Time_sec & 0x0F));
      //SerialSend('\r');
      //SerialSend('\n');
      if(PINBTN0){
        SetData_Max7219(Time_sec,Time_min,Time_hr,Time_day);
      }else{
        SetData_Max7219(Time_min,Time_hr,Time_day,Time_mon);
      }
      
      //0.2s
      for(uint16_t i=0;i<1000;i++){
        wait(200);
      }
      
      if(Time_sec==0 && Time_min==0){
        uint8_t hr = Time_hr;
        if(hr>12){
          hr-=12;
        }
        for(uint8_t cnt=0;cnt<=hr;cnt++){
          BellH;
          //1.5s
          for(uint16_t i=0;i<75000;i++){
            wait(200);
          }
          BellL;
          //0.1s
          for(uint16_t i=0;i<500;i++){
            wait(200);
          }
        }
      }
      //int start = Serial.parseInt();
      //if(start == 33)
      //{
      //	Time_sec = Serial.parseInt();
      //	Time_min = Serial.parseInt();
      //	Time_hr  = Serial.parseInt();
      //	Time_day = Serial.parseInt();
      //	Time_mon = Serial.parseInt();
      //	Time_yr  = Serial.parseInt();
      //	Time_week= Serial.parseInt();
      //  WriteTime();
      //}

    }
  
	}
        
	return 0;
}
