#define cbi(reg, bitmask) *reg &= ~bitmask
#define sbi(reg, bitmask) *reg |= bitmask
#define pulseClock cbi(P_SCK, B_SCK); asm ("nop"); sbi(P_SCK, B_SCK)
#define resetLCD sbi(P_DC, B_DC); sbi(P_MOSI, B_MOSI); sbi(P_SCK, B_SCK); sbi(P_CS, B_CS); cbi(P_RST, B_RST); delay(10); sbi(P_RST, B_RST)

#define fontbyte(x) pgm_read_byte(&cfont.font[x])  
#define bitmapbyte(x) pgm_read_byte(&bitmap[x])  

#define regtype volatile uint8_t
#define regsize volatile uint8_t
#define bitmapdatatype uint8_t*

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define LCD_COMMAND 0
#define LCD_DATA 1

// PCD8544 Commandset
// ------------------
// General commands
#define PCD8544_POWERDOWN			0x04
#define PCD8544_ENTRYMODE			0x02
#define PCD8544_EXTENDEDINSTRUCTION	0x01
#define PCD8544_DISPLAYBLANK		0x00
#define PCD8544_DISPLAYNORMAL		0x04
#define PCD8544_DISPLAYALLON		0x01
#define PCD8544_DISPLAYINVERTED		0x05
// Normal instruction set
#define PCD8544_FUNCTIONSET			0x20
#define PCD8544_DISPLAYCONTROL		0x08
#define PCD8544_SETYADDR			0x40
#define PCD8544_SETXADDR			0x80
// Extended instruction set
#define PCD8544_SETTEMP				0x04
#define PCD8544_SETBIAS				0x10
#define PCD8544_SETVOP				0x80
// Display presets
#define LCD_BIAS					0x03	// Range: 0-7 (0x00-0x07)
#define LCD_TEMP					0x02	// Range: 0-3 (0x00-0x03)
#define LCD_CONTRAST				0x46	// Range: 0-127 (0x00-0x7F)

PROGMEM prog_uint8_t font[] = {
// 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//!
0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x33,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
//"
0x00,0x00,0x10,0x00,0x0C,0x00,0x06,0x00,0x10,0x00,0x0C,0x00,0x06,0x00,0x00,0x00,
//#
0x40,0x04,0xC0,0x3F,0x78,0x04,0x40,0x04,0xC0,0x3F,0x78,0x04,0x40,0x04,0x00,0x00,
//$
0x00,0x00,0x70,0x18,0x88,0x20,0xFC,0xFF,0x08,0x21,0x30,0x1E,0x00,0x00,0x00,0x00,
//%
0xF0,0x00,0x08,0x21,0xF0,0x1C,0x00,0x03,0xE0,0x1E,0x18,0x21,0x00,0x1E,0x00,0x00,
//&
0x00,0x1E,0xF0,0x21,0x08,0x23,0x88,0x24,0x70,0x19,0x00,0x27,0x00,0x21,0x00,0x10,
//'
0x10,0x00,0x16,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//(
0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x07,0x18,0x18,0x04,0x20,0x02,0x40,0x00,0x00,
//)
0x00,0x00,0x02,0x40,0x04,0x20,0x18,0x18,0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,
//*
0x40,0x02,0x40,0x02,0x80,0x01,0xF0,0x0F,0x80,0x01,0x40,0x02,0x40,0x02,0x00,0x00,
//+
0x00,0x01,0x00,0x01,0x00,0x01,0xF0,0x1F,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x00,
//,
0x00,0x80,0x00,0xB0,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//-
0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
//.
0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
///
0x00,0x00,0x00,0x60,0x00,0x18,0x00,0x06,0x80,0x01,0x60,0x00,0x18,0x00,0x04,0x00,
//0
0x00,0x00,0xE0,0x0F,0x10,0x10,0x08,0x20,0x08,0x20,0x10,0x10,0xE0,0x0F,0x00,0x00,
//1
0x00,0x00,0x10,0x20,0x10,0x20,0xF8,0x3F,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x00,
//2
0x00,0x00,0x70,0x30,0x08,0x28,0x08,0x24,0x08,0x22,0x88,0x21,0x70,0x30,0x00,0x00,
//3
0x00,0x00,0x30,0x18,0x08,0x20,0x88,0x20,0x88,0x20,0x48,0x11,0x30,0x0E,0x00,0x00,
//4
0x00,0x00,0x00,0x07,0xC0,0x04,0x20,0x24,0x10,0x24,0xF8,0x3F,0x00,0x24,0x00,0x00,
//5
0x00,0x00,0xF8,0x19,0x08,0x21,0x88,0x20,0x88,0x20,0x08,0x11,0x08,0x0E,0x00,0x00,
//6
0x00,0x00,0xE0,0x0F,0x10,0x11,0x88,0x20,0x88,0x20,0x18,0x11,0x00,0x0E,0x00,0x00,
//7
0x00,0x00,0x38,0x00,0x08,0x00,0x08,0x3F,0xC8,0x00,0x38,0x00,0x08,0x00,0x00,0x00,
//8
0x00,0x00,0x70,0x1C,0x88,0x22,0x08,0x21,0x08,0x21,0x88,0x22,0x70,0x1C,0x00,0x00,
//9
0x00,0x00,0xE0,0x00,0x10,0x31,0x08,0x22,0x08,0x22,0x10,0x11,0xE0,0x0F,0x00,0x00,
//:
0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x30,0xC0,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
//;
0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//<
0x00,0x00,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x08,0x10,0x10,0x08,0x20,0x00,0x00,
//=
0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x00,0x00,
//>
0x00,0x00,0x08,0x20,0x10,0x10,0x20,0x08,0x40,0x04,0x80,0x02,0x00,0x01,0x00,0x00,
//?
0x00,0x00,0x70,0x00,0x48,0x00,0x08,0x30,0x08,0x36,0x08,0x01,0xF0,0x00,0x00,0x00,
//@
0xC0,0x07,0x30,0x18,0xC8,0x27,0x28,0x24,0xE8,0x23,0x10,0x14,0xE0,0x0B,0x00,0x00,
//A
0x00,0x20,0x00,0x3C,0xC0,0x23,0x38,0x02,0xE0,0x02,0x00,0x27,0x00,0x38,0x00,0x20,
//B
0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x20,0x88,0x20,0x70,0x11,0x00,0x0E,0x00,0x00,
//C
0xC0,0x07,0x30,0x18,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x10,0x38,0x08,0x00,0x00,
//D
0x08,0x20,0xF8,0x3F,0x08,0x20,0x08,0x20,0x08,0x20,0x10,0x10,0xE0,0x0F,0x00,0x00,
//E
0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x20,0xE8,0x23,0x08,0x20,0x10,0x18,0x00,0x00,
//F
0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x00,0xE8,0x03,0x08,0x00,0x10,0x00,0x00,0x00,
//G
0xC0,0x07,0x30,0x18,0x08,0x20,0x08,0x20,0x08,0x22,0x38,0x1E,0x00,0x02,0x00,0x00,
//H
0x08,0x20,0xF8,0x3F,0x08,0x21,0x00,0x01,0x00,0x01,0x08,0x21,0xF8,0x3F,0x08,0x20,
//I
0x00,0x00,0x08,0x20,0x08,0x20,0xF8,0x3F,0x08,0x20,0x08,0x20,0x00,0x00,0x00,0x00,
//J
0x00,0xC0,0x00,0x80,0x08,0x80,0x08,0x80,0xF8,0x7F,0x08,0x00,0x08,0x00,0x00,0x00,
//K
0x08,0x20,0xF8,0x3F,0x88,0x20,0xC0,0x01,0x28,0x26,0x18,0x38,0x08,0x20,0x00,0x00,
//L
0x08,0x20,0xF8,0x3F,0x08,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x30,0x00,0x00,
//M
0x08,0x20,0xF8,0x3F,0xF8,0x00,0x00,0x3F,0xF8,0x00,0xF8,0x3F,0x08,0x20,0x00,0x00,
//N
0x08,0x20,0xF8,0x3F,0x30,0x20,0xC0,0x00,0x00,0x07,0x08,0x18,0xF8,0x3F,0x08,0x00,
//O
0xE0,0x0F,0x10,0x10,0x08,0x20,0x08,0x20,0x08,0x20,0x10,0x10,0xE0,0x0F,0x00,0x00,
//P
0x08,0x20,0xF8,0x3F,0x08,0x21,0x08,0x01,0x08,0x01,0x08,0x01,0xF0,0x00,0x00,0x00,
//Q
0xE0,0x0F,0x10,0x18,0x08,0x24,0x08,0x24,0x08,0x38,0x10,0x50,0xE0,0x4F,0x00,0x00,
//R
0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x00,0x88,0x03,0x88,0x0C,0x70,0x30,0x00,0x20,
//S
0x08,0x20,0x18,0x30,0x68,0x2C,0x80,0x03,0x80,0x03,0x68,0x2C,0x18,0x30,0x08,0x20,
//T
0x18,0x00,0x08,0x00,0x08,0x20,0xF8,0x3F,0x08,0x20,0x08,0x00,0x18,0x00,0x00,0x00,
//U
0x08,0x00,0xF8,0x1F,0x08,0x20,0x00,0x20,0x00,0x20,0x08,0x20,0xF8,0x1F,0x08,0x00,
//V
0x08,0x00,0x78,0x00,0x88,0x07,0x00,0x38,0x00,0x0E,0xC8,0x01,0x38,0x00,0x08,0x00,
//W
0xF8,0x03,0x08,0x3C,0x00,0x07,0xF8,0x00,0x00,0x07,0x08,0x3C,0xF8,0x03,0x00,0x00,
//X
0x08,0x20,0x18,0x30,0x68,0x2C,0x80,0x03,0x80,0x03,0x68,0x2C,0x18,0x30,0x08,0x20,
//Y
0x08,0x00,0x38,0x00,0xC8,0x20,0x00,0x3F,0xC8,0x20,0x38,0x00,0x08,0x00,0x00,0x00,
//Z
0x10,0x20,0x08,0x38,0x08,0x26,0x08,0x21,0xC8,0x20,0x38,0x20,0x08,0x18,0x00,0x00,
//[
0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x7F,0x02,0x40,0x02,0x40,0x02,0x40,0x00,0x00,
//\
0x00,0x00,0x0C,0x00,0x30,0x00,0xC0,0x01,0x00,0x06,0x00,0x38,0x00,0xC0,0x00,0x00,
//]
0x00,0x00,0x02,0x40,0x02,0x40,0x02,0x40,0xFE,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,
//^
0x00,0x00,0x00,0x00,0x04,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x00,0x00,
//_
0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
//`
0x00,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
// 
0x00,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//a
0x00,0x00,0x00,0x19,0x80,0x24,0x80,0x22,0x80,0x22,0x80,0x22,0x00,0x3F,0x00,0x20,
//b
0x08,0x00,0xF8,0x3F,0x00,0x11,0x80,0x20,0x80,0x20,0x00,0x11,0x00,0x0E,0x00,0x00,
//c
0x00,0x00,0x00,0x0E,0x00,0x11,0x80,0x20,0x80,0x20,0x80,0x20,0x00,0x11,0x00,0x00,
//d
0x00,0x00,0x00,0x0E,0x00,0x11,0x80,0x20,0x80,0x20,0x88,0x10,0xF8,0x3F,0x00,0x20,
//e
0x00,0x00,0x00,0x1F,0x80,0x22,0x80,0x22,0x80,0x22,0x80,0x22,0x00,0x13,0x00,0x00,
//f
0x00,0x00,0x80,0x20,0x80,0x20,0xF0,0x3F,0x88,0x20,0x88,0x20,0x88,0x00,0x18,0x00,
//g
0x00,0x00,0x00,0x6B,0x80,0x94,0x80,0x94,0x80,0x94,0x80,0x93,0x80,0x60,0x00,0x00,
//h
0x08,0x20,0xF8,0x3F,0x00,0x21,0x80,0x00,0x80,0x00,0x80,0x20,0x00,0x3F,0x00,0x20,
//i
0x00,0x00,0x80,0x20,0x98,0x20,0x98,0x3F,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x00,
//j
0x00,0x00,0x00,0xC0,0x00,0x80,0x80,0x80,0x98,0x80,0x98,0x7F,0x00,0x00,0x00,0x00,
//k
0x08,0x20,0xF8,0x3F,0x00,0x24,0x00,0x02,0x80,0x2D,0x80,0x30,0x80,0x20,0x00,0x00,
//l
0x00,0x00,0x08,0x20,0x08,0x20,0xF8,0x3F,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x00,
//m
0x80,0x20,0x80,0x3F,0x80,0x20,0x80,0x00,0x80,0x3F,0x80,0x20,0x80,0x00,0x00,0x3F,
//n
0x80,0x20,0x80,0x3F,0x00,0x21,0x80,0x00,0x80,0x00,0x80,0x20,0x00,0x3F,0x00,0x20,
//o
0x00,0x00,0x00,0x1F,0x80,0x20,0x80,0x20,0x80,0x20,0x80,0x20,0x00,0x1F,0x00,0x00,
//p
0x80,0x80,0x80,0xFF,0x00,0xA1,0x80,0x20,0x80,0x20,0x00,0x11,0x00,0x0E,0x00,0x00,
//q
0x00,0x00,0x00,0x0E,0x00,0x11,0x80,0x20,0x80,0x20,0x80,0xA0,0x80,0xFF,0x00,0x80,
//r
0x80,0x20,0x80,0x20,0x80,0x3F,0x00,0x21,0x80,0x20,0x80,0x00,0x80,0x01,0x00,0x00,
//s
0x00,0x00,0x00,0x33,0x80,0x24,0x80,0x24,0x80,0x24,0x80,0x24,0x80,0x19,0x00,0x00,
//t
0x00,0x00,0x80,0x00,0x80,0x00,0xE0,0x1F,0x80,0x20,0x80,0x20,0x00,0x00,0x00,0x00,
//u
0x80,0x00,0x80,0x1F,0x00,0x20,0x00,0x20,0x00,0x20,0x80,0x10,0x80,0x3F,0x00,0x20,
//v
0x80,0x00,0x80,0x01,0x80,0x0E,0x00,0x30,0x00,0x08,0x80,0x06,0x80,0x01,0x80,0x00,
//w
0x80,0x0F,0x80,0x30,0x00,0x0C,0x80,0x03,0x00,0x0C,0x80,0x30,0x80,0x0F,0x80,0x00,
//x
0x00,0x00,0x80,0x20,0x80,0x31,0x00,0x2E,0x80,0x0E,0x80,0x31,0x80,0x20,0x00,0x00,
//y
0x80,0x80,0x80,0x81,0x80,0x8E,0x00,0x70,0x00,0x18,0x80,0x06,0x80,0x01,0x80,0x00,
//z
0x00,0x00,0x80,0x21,0x80,0x30,0x80,0x2C,0x80,0x22,0x80,0x21,0x80,0x30,0x00,0x00,
//{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x7C,0x3F,0x02,0x40,0x02,0x40,
//|
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
//}
0x00,0x00,0x02,0x40,0x02,0x40,0x7C,0x3F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//~
0x06,0x00,0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x04,0x00,0x03,0x00,
//
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

class LCD5110
{
public:
  LCD5110(int SCK, int MOSI, int DC, int RST, int CS);
  void InitLCD(int contrast=LCD_CONTRAST);
  void setContrast(int contrast);
  void drawLine(uint8_t* line, uint8_t Pos);

  regtype			*P_SCK, *P_MOSI, *P_DC, *P_RST, *P_CS;
  regsize			B_SCK, B_MOSI, B_DC, B_RST, B_CS;
  uint8_t			SCK_Pin, RST_Pin;			// Needed for for faster MCUs
  int				_contrast;

  void	_LCD_Write(unsigned char data, unsigned char mode);
  void	_print_char(unsigned char c, int x, int row);
  void	_convert_float(char *buf, double num, int width, byte prec);
};


LCD5110::LCD5110(int SCK, int MOSI, int DC, int RST, int CS)
{ 
  P_SCK	= portOutputRegister(digitalPinToPort(SCK));
  B_SCK	= digitalPinToBitMask(SCK);
  P_MOSI	= portOutputRegister(digitalPinToPort(MOSI));
  B_MOSI	= digitalPinToBitMask(MOSI);
  P_DC	= portOutputRegister(digitalPinToPort(DC));
  B_DC	= digitalPinToBitMask(DC);
  P_RST	= portOutputRegister(digitalPinToPort(RST));
  B_RST	= digitalPinToBitMask(RST);
  P_CS	= portOutputRegister(digitalPinToPort(CS));
  B_CS	= digitalPinToBitMask(CS);
  pinMode(SCK,OUTPUT);
  pinMode(MOSI,OUTPUT);
  pinMode(DC,OUTPUT);
  pinMode(RST,OUTPUT);
  pinMode(CS,OUTPUT);
  SCK_Pin=SCK;
  RST_Pin=RST;
}

void LCD5110::_LCD_Write(unsigned char data, unsigned char mode)
{   
  cbi(P_CS, B_CS);

  if (mode==LCD_COMMAND)
    cbi(P_DC, B_DC);
  else
    sbi(P_DC, B_DC);

  for (unsigned char c=0; c<8; c++)
  {
    if (data & 0x80)
      sbi(P_MOSI, B_MOSI);
    else
      cbi(P_MOSI, B_MOSI);
    data = data<<1;
    pulseClock;
  }

  sbi(P_CS, B_CS);
}

void LCD5110::InitLCD(int contrast)
{
  if (contrast>0x7F)
    contrast=0x7F;
  if (contrast<0)
    contrast=0;

  resetLCD;

  _LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
  _LCD_Write(PCD8544_SETVOP | contrast, LCD_COMMAND);
  _LCD_Write(PCD8544_SETTEMP | LCD_TEMP, LCD_COMMAND);
  _LCD_Write(PCD8544_SETBIAS | LCD_BIAS, LCD_COMMAND);
  _LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
  _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
  _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
  for (int c=0; c<504; c++)
    _LCD_Write(0x00, LCD_DATA);
  _LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);

  _contrast=contrast;
}

void LCD5110::setContrast(int contrast)
{
  if (contrast>0x7F)
    contrast=0x7F;
  if (contrast<0)
    contrast=0;
  _LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
  _LCD_Write(PCD8544_SETVOP | contrast, LCD_COMMAND);
  _LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
  _contrast=contrast;
}

//Pos:0,1,2
void LCD5110::drawLine(uint8_t* line, uint8_t Pos)
{
  {
    _LCD_Write(PCD8544_SETYADDR | Pos*2, LCD_COMMAND);
    _LCD_Write(PCD8544_SETXADDR | 0, LCD_COMMAND);
    for(uint8_t i=0;i<10;i++)
    {
      int chr = line[i];
      if(chr<32)
      {
        chr = 0;
      }
      else
      {
        chr -= 32;
      }
      chr*=16;
      for(uint8_t cx=0; cx<16; cx+=2)
      {
        _LCD_Write(pgm_read_byte_near(font+chr+cx), LCD_DATA);
      }
    }
  }
  {
    _LCD_Write(PCD8544_SETYADDR | Pos*2+1, LCD_COMMAND);
    _LCD_Write(PCD8544_SETXADDR | 0, LCD_COMMAND);
    for(uint8_t i=0;i<10;i++)
    {
      int chr = line[i];
      if(chr<32)
      {
        chr = 0;
      }
      else
      {
        chr -= 32;
      }
      chr*=16;
      for(uint8_t cx=1; cx<17; cx+=2)
      {
        _LCD_Write(pgm_read_byte_near(font+chr+cx), LCD_DATA);
      }
    }
  }

  _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
  _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
}


#define _SCK  A1//10
#define _MOSI A2//11
#define _DC A3//12
#define _CS 9//13
#define _RST 8//A0

LCD5110 myGLCD(_SCK,_MOSI,_DC,_RST,_CS);

uint8_t line[10];

void setup() {
  myGLCD.InitLCD();
}

void loop() {

  line[0] = 'a';
  line[1] = 'b';
  line[2] = 'c';
  line[3] = 'd';
  line[4] = 'e';
  line[5] = 'f';
  line[6] = 'g';
  myGLCD.drawLine(line,0);
  line[0] = 'A';
  line[1] = 'B';
  line[2] = 'C';
  line[3] = 'D';
  line[4] = 'E';
  line[5] = 'F';
  line[6] = 'G';
  myGLCD.drawLine(line,1);
  line[0] = '1';
  line[1] = '2';
  line[2] = '3';
  line[3] = '4';
  line[4] = '5';
  line[5] = '6';
  line[6] = '7';
  myGLCD.drawLine(line,2);
delay(100);
}





