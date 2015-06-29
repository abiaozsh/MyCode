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

struct _current_font
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
	uint8_t inverted;
};

class LCD5110
{
	public:
		LCD5110(int SCK, int MOSI, int DC, int RST, int CS);
		void	InitLCD(int contrast=LCD_CONTRAST);
		void	setContrast(int contrast);
		//void	enableSleep();
		//void	disableSleep();
		void	clrScr();
		void	invert(bool mode);
		void	drawBitmap(int x, int y, bitmapdatatype bitmap, int sx, int sy);
		void	drawBitmapBuff(int x, int y, byte *bitmap, int sx, int sy);

		regtype			*P_SCK, *P_MOSI, *P_DC, *P_RST, *P_CS;
		regsize			B_SCK, B_MOSI, B_DC, B_RST, B_CS;
		uint8_t			SCK_Pin, RST_Pin;			// Needed for for faster MCUs
		_current_font	cfont;
		boolean			_sleep;
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

  cfont.font=0;
  _sleep=false;
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

void LCD5110::clrScr()
{
  if (!_sleep)
  {
    _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
    _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
    for (int c=0; c<504; c++)
      _LCD_Write(0x00, LCD_DATA);
  }
}

void LCD5110::invert(bool mode)
{
  if (!_sleep)
  {
    if (mode==true)
      _LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYINVERTED, LCD_COMMAND);
    else
      _LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);
  }
}

void LCD5110::drawBitmap(int x, int y, bitmapdatatype bitmap, int sx, int sy)
{
  int starty, rows;

  if (!_sleep)
  {
    starty = y / 8;

    if (sy%8==0)
      rows=sy/8;  
    else
      rows=(sy/8)+1;

    for (int cy=0; cy<rows; cy++)
    {
      _LCD_Write(PCD8544_SETYADDR | (starty+cy), LCD_COMMAND);
      _LCD_Write(PCD8544_SETXADDR | x, LCD_COMMAND);
      for(int cx=0; cx<sx; cx++)
        _LCD_Write(bitmapbyte(cx+(cy*sx)), LCD_DATA);
    }      
    _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
    _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
  }
}

void LCD5110::drawBitmapBuff(int x, int y, byte *bitmap, int sx, int sy)
{
  int starty, rows;

  if (!_sleep)
  {
    starty = y / 8;

    if (sy%8==0)
      rows=sy/8;  
    else
      rows=(sy/8)+1;

    for (int cy=0; cy<rows; cy++)
    {
      _LCD_Write(PCD8544_SETYADDR | (starty+cy), LCD_COMMAND);
      _LCD_Write(PCD8544_SETXADDR | x, LCD_COMMAND);
      for(int cx=0; cx<sx; cx++)
        _LCD_Write(bitmap[cx+(cy*sx)], LCD_DATA);
    }      
    _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
    _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
  }
}


#define _SCK  5
#define _MOSI 6
#define _DC 7
#define _CS 8
#define _RST 9

LCD5110 myGLCD(_SCK,_MOSI,_DC,_RST,_CS);

byte buff[504];

void setup() {
  myGLCD.InitLCD();
  setPixel(0,0,1);
  setPixel(1,1,0);
  setPixel(1,0,0);
  setPixel(0,1,1);
  myGLCD.drawBitmapBuff(0, 0, buff, 84, 48);
}

void loop() {

}




//x:84
//y:48
void setPixel(byte x, byte y, byte val)
{
  int idx = x + (y >> 3) * 84;
  int shift = y & 7;
  if (val == 0)
  {
    buff[idx] &= ~(1 << shift);
  }
  else
  {
    buff[idx] |= (1 << shift);
  }
}

