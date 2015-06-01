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












#define _SCK  5
#define _MOSI 6
#define _DC 7
#define _CS 8
#define _RST 9

LCD5110 myGLCD(_SCK,_MOSI,_DC,_RST,_CS);

#define KEY_LEFT      11
#define KEY_RIGHT     10
#define KEY_DOWN      A1
#define KEY_DOWNFAST  A0
#define KEY_ROTATE    12

byte buff[504];
byte Board[200];
byte PosX = 0;
byte PosY = 0;
byte NextShapeNum = 0;
byte NowShapeNum = 0;
byte NowShapeRotateIdx = 0;

int state_LEFT = 0;
int state_RIGHT = 0;
int state_DOWN = 0;
int state_DOWNFAST = 0;
int state_ROTATE = 0;
unsigned long NextSecond = 0;

void setup() {
  myGLCD.InitLCD();
  InitImg();
  DrawInitScreen();
  while(true)
  {
    if(digitalRead(KEY_ROTATE)!=1)
    {
      break;
    }
  }
  
  randomSeed(micros());

  Init();

  NextSecond = millis()+1000;
}

void loop() {
  while(true)
  {
    if(digitalRead(KEY_LEFT)!=state_LEFT)
    {
      state_LEFT=digitalRead(KEY_LEFT);
      if(state_LEFT==0)
      {
        Move(1);
        break;
      }
    }
    if(digitalRead(KEY_RIGHT)!=state_RIGHT)
    {
      state_RIGHT=digitalRead(KEY_RIGHT);
      if(state_RIGHT==0)
      {
        Move(255);
        break;
      }
    }
    if(digitalRead(KEY_DOWN)!=state_DOWN)
    {
      state_DOWN=digitalRead(KEY_DOWN);
      if(state_DOWN==0)
      {
        slowdown();
        break;
      }
    }
    if(digitalRead(KEY_DOWNFAST)!=state_DOWNFAST)
    {
      state_DOWNFAST=digitalRead(KEY_DOWNFAST);
      if(state_DOWNFAST==0)
      {
        down();
        break;
      }
    }
    if(digitalRead(KEY_ROTATE)!=state_ROTATE)
    {
      state_ROTATE=digitalRead(KEY_ROTATE);
      if(state_ROTATE==0)
      {
        rotate();
        break;
      }
    }
    if(millis()>NextSecond)
    {
      NextSecond = millis()+1000;
      timing();
      break;
    }
  }

  DrawBoard();
  myGLCD.drawBitmapBuff(0, 0, buff, 84, 48);
}




