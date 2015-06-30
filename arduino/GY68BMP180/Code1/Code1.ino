#include <Wire.h>
#define BMP085_ADDRESS 0x77  // I2C address of BMP085
const unsigned char OSS = 3;  // Oversampling Setting ultra high resolution
// Calibration values
int ac1;
int ac2;
int ac3;
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1;
int b2;
int mb;
int mc;
int md;
long b5; 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
  boolean			_sleep;
  int				_contrast;

  void	_LCD_Write(unsigned char data, unsigned char mode);
  void	_print_char(unsigned char c, int x, int row);
  void	_convert_float(char *buf, double num, int width, byte prec);
};



/*
Temperature: 30.50deg C
 
 Pressure: 100274 Pa
 
 Altitude: 87.86 M
 */


#define _SCK  10
#define _MOSI 11
#define _DC 12
#define _CS 13
#define _RST A0

LCD5110 myGLCD(_SCK,_MOSI,_DC,_RST,_CS);
uint8_t buff[504];


uint8_t line0[10];
uint8_t line1[10];
uint8_t line2[10];
uint8_t line3[10];

PROGMEM prog_uint8_t Font[] = {
B00000000,
B01111110,
B10000001,
B01111110,//0
B00000000,
B10000001,
B11111111,
B00000001,//1
B00000000,
B10000111,
B10001001,
B01110001,//2
B00000000,
B10001001,
B10001001,
B01110111,//3
B00000000,
B10001001,
B10001001,
B01110111,//3
};


float basepressure;

void setup(){
  Serial.begin(9600);
  Wire.begin();
  myGLCD.InitLCD();
  bmp085Calibration();
  delay(1000);
  float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  basepressure = bmp085GetPressure(bmp085ReadUP());
}

void loop()
{
  float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  float pressure = bmp085GetPressure(bmp085ReadUP());
  float altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 
  Serial.print("Temperature: ");
  Serial.print(temperature, 2); //display 2 decimal places
  Serial.println("deg C");
  Serial.print("Pressure: ");
  Serial.print(pressure, 0); //whole number only.
  Serial.println(" Pa");
  Serial.print("Altitude: ");
  Serial.print(altitude, 2); //display 2 decimal places
  Serial.println(" M");
  Serial.println();//line break
  delay(500); //wait a second and get values again.
  
  printline();
  myGLCD.drawBitmapBuff(0, 0, buff, 84, 48);

}

void printline()
{
//x:84
//y:48
//  setPixel(0,0,1);
//  setPixel(0,1,1);
//  setPixel(1,1,0);
//  setPixel(1,0,0);

//x:84
//y:0~6
//void setLine(byte x, byte y, byte val)

}


// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Calculate temperature in deg C
float bmp085GetTemperature(unsigned int ut){
  long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  float temp = ((b5 + 8)>>4);
  temp = temp /10;

  return temp;
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up){
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;

  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  long temp = p;
  return temp;
}

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  unsigned char data;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;

  return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();

  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT(){
  unsigned int ut;

  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();

  // Wait at least 4.5ms
  delay(5);

  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP(){

  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;

  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();

  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));

  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  msb = bmp085Read(0xF6);
  lsb = bmp085Read(0xF7);
  xlsb = bmp085Read(0xF8);

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

  return up;
}

void writeRegister(int deviceAddress, byte address, byte val) {
  Wire.beginTransmission(deviceAddress); // start transmission to device 
  Wire.write(address);       // send register address
  Wire.write(val);         // send value to write
  Wire.endTransmission();     // end transmission
}

int readRegister(int deviceAddress, byte address){

  int v;
  Wire.beginTransmission(deviceAddress);
  Wire.write(address); // register to read
  Wire.endTransmission();

  Wire.requestFrom(deviceAddress, 1); // read a byte

  while(!Wire.available()) {
    // waiting
  }

  v = Wire.read();
  return v;
}

float calcAltitude(float pressure){

  float A = pressure/101325;
  float B = 1/5.25588;
  float C = pow(A,B);
  C = 1 - C;
  C = C /0.0000225577;

  return C;
}






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


//x:84
//y:0~6
void setLine(byte x, byte y, byte val)
{
  int idx = x + y * 84;
  buff[idx] = val;
}


