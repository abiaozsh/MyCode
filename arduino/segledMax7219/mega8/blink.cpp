#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


PROGMEM prog_uint8_t conv[] = {
  //  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
//~0xC0,~0xF9,~0xA4,~0xB0,~0x99,~0x92,~0x82,~0xF8,~0x80,~0x90,~0x88,~0x83,~0xC6,~0xA1,~0x86,~0x8E
   0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47
};

#define Max7219_pinCLKH PORTB |=  _BV(0)
#define Max7219_pinCLKL PORTB &= ~_BV(0)
#define Max7219_pinCLKO DDRB  |=  _BV(0)
#define Max7219_pinCSH  PORTD |=  _BV(7)
#define Max7219_pinCSL  PORTD &= ~_BV(7)
#define Max7219_pinCSO  DDRD  |=  _BV(7)
#define Max7219_pinDINH PORTB |=  _BV(2)
#define Max7219_pinDINL PORTB &= ~_BV(2)
#define Max7219_pinDINO DDRB  |=  _BV(2)


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
  Write_Max7219(0x0a, 0x03);       //亮度 
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
	while(TCNT0<ticks);
}

int main()
{
  TCCR1A = 0;
//0 0 0 No clock source. (Timer/Counter stopped)
//0 0 1 clkI/O/1 (No prescaling)
//0 1 0 clkI/O/8 (From prescaler)
//0 1 1 clkI/O/64 (From prescaler)
//1 0 0 clkI/O/256 (From prescaler)
//1 0 1 clkI/O/1024 (From prescaler)
  TCCR1B = 3;//1/8 Mhz
  TCNT1 = 0;
  //TIMSK |= _BV(OCIE1A);
  OCR1A = 375;//8us * 375  3ms  333t/s(83.3hz refresh) count 41.6: 8 times/s
  SerialInit();
  TimerInit();
  Init_MAX7219();

    uint8_t val = 0;
  while (true) {
    if(!(PINC&_BV(3))){
      SetData_Max7219(val,val+1,val+2,val+3);
      val++;
      for(uint16_t i=0;i<2500;i++){
        wait(250);
      }
    }
  }
}
