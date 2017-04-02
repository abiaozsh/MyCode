
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <avr/io.h>




void dly()
{
  for(uint8_t i=0;i<6;i++)//6 is stable
  {
    volatile uint8_t v=0;
    v++;
  }
}

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

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



uint8_t BIT_SCL = _BV(0);
uint8_t BIT_SDA = _BV(1);

#define DDR_SCL  DDRC
#define PORT_SCL PORTC

#define DDR_SDA  DDRC
#define PORT_SDA PORTC
#define PIN_SDA  PINC


//
void i2c_SoftI2CMaster()
{
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
}
//
uint8_t i2c_beginTransmission(uint8_t address)
{
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 0); // clr read bit
  return rc;
}
//
uint8_t i2c_requestFrom(uint8_t address)
{
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 1); // set read bit
  return rc;
}
//
//
//
uint8_t i2c_endTransmission(void)
{
  i2c_stop();
  //return ret; // FIXME
  return 0;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
void i2c_write(uint8_t* data, uint8_t quantity)
{
  for(uint8_t i = 0; i < quantity; ++i){
    i2c_write(data[i]);
  }
}

//--------------------------------------------------------------------
void i2c_writebit( uint8_t c )
{
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
//
uint8_t i2c_readbit(void)
{
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
  uint8_t c = PIN_SDA; // I2C_PIN;
  i2c_scl_lo();
  dly();
  return ( c & BIT_SDA) ? 1 : 0;
}
// Send a START Condition
//
void i2c_start(void)
{
  // set both to high at the same time
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
  i2c_sda_lo();
  dly();
  i2c_scl_lo();
  dly();
}
void i2c_repstart(void)
{
  // set both to high at the same time (releases drive on both lines)
  i2c_sda_hi();
  i2c_scl_hi();
  i2c_scl_lo(); // force SCL low
  dly();
  i2c_sda_release(); // release SDA
  dly();
  i2c_scl_release(); // release SCL
  dly();
  i2c_sda_lo(); // force SDA low
  dly();
}
// Send a STOP Condition
//
void i2c_stop(void)
{
  i2c_scl_hi();
  dly();
  i2c_sda_hi();
  dly();
}
// write a byte to the I2C slave device
//
uint8_t i2c_write( uint8_t c )
{
  for ( uint8_t i=0;i<8;i++) {
    i2c_writebit( c & 128 );
    c<<=1;
  }
  return i2c_readbit();
}
// read a byte from the I2C slave device
//
uint8_t i2c_read( uint8_t ack )
{
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

//
uint8_t i2c_read()
{
  return i2c_read( I2C_ACK );
}
//
uint8_t i2c_readLast()
{
  return i2c_read( I2C_NAK );
}















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

int temperature; //MUST be called first
long pressure;
float altitude; //Uncompensated caculation - in Meters 

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
int bmp085GetTemperature(unsigned int ut){
  long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  long temp = ((b5 + 8)>>4);

  return (int)temp;
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
  i2c_beginTransmission(BMP085_ADDRESS);
  i2c_write(address);
  i2c_endTransmission();
  i2c_requestFrom(BMP085_ADDRESS);
  uint8_t msb = i2c_readLast();
  return msb;
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;

  i2c_beginTransmission(BMP085_ADDRESS);
  i2c_write(address);
  i2c_endTransmission();

  i2c_requestFrom(BMP085_ADDRESS);

  msb = i2c_read();
  lsb = i2c_readLast();
  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT(){
  unsigned int ut;

  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  i2c_beginTransmission(BMP085_ADDRESS);
  uint8_t d1 = 0xF4;
  uint8_t d2 = 0x2E;
  i2c_write(d1);
  i2c_write(d2);
  i2c_endTransmission();

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
  i2c_beginTransmission(BMP085_ADDRESS);
  uint8_t d1 = 0xF4;
  uint8_t d2 = 0x34 + (OSS<<6);
  i2c_write(d1);
  i2c_write(d2);
  i2c_endTransmission();

  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));

  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  msb = bmp085Read(0xF6);
  lsb = bmp085Read(0xF7);
  xlsb = bmp085Read(0xF8);

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

  return up;
}

float calcAltitude(float pressure){

  float A = pressure/101325;
  float B = 1/5.25588;
  float C = pow(A,B);
  C = 1 - C;
  C = C /0.0000225577;
  return C;
}








void getBMP180()
{
  //BIT_SCL = _BV(7);//BMP180
  //BIT_SDA = _BV(6);//BMP180
  temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  pressure = bmp085GetPressure(bmp085ReadUP());
  altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 
}







// A1
#define I_SCK  DDRC |=  _BV(1)
#define H_SCK PORTC |=  _BV(1)
#define L_SCK PORTC &= ~_BV(1)

// A2
#define I_MOSI  DDRC |=  _BV(2)
#define H_MOSI PORTC |=  _BV(2)
#define L_MOSI PORTC &= ~_BV(2)

// A3
#define I_DC  DDRC |=  _BV(3)
#define H_DC PORTC |=  _BV(3)
#define L_DC PORTC &= ~_BV(3)

// 9
#define I_CS  DDRB |=  _BV(1)
#define H_CS PORTB |=  _BV(1)
#define L_CS PORTB &= ~_BV(1)

// 8
#define I_RST  DDRB |=  _BV(0)
#define H_RST PORTB |=  _BV(0)
#define L_RST PORTB &= ~_BV(0)

#define LCD_COMMAND 0
#define LCD_DATA 1

// PCD8544 Commandset
// ------------------
// General commands
#define PCD8544_EXTENDEDINSTRUCTION	0x01
#define PCD8544_DISPLAYNORMAL		0x04
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
  //  32
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //! 33
  0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x33,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
  //" 34
  0x00,0x00,0x10,0x00,0x0C,0x00,0x06,0x00,0x10,0x00,0x0C,0x00,0x06,0x00,0x00,0x00,
  //# 35
  0x40,0x04,0xC0,0x3F,0x78,0x04,0x40,0x04,0xC0,0x3F,0x78,0x04,0x40,0x04,0x00,0x00,
  //$ 36
  0x00,0x00,0x70,0x18,0x88,0x20,0xFC,0xFF,0x08,0x21,0x30,0x1E,0x00,0x00,0x00,0x00,
  //% 37
  0xF0,0x00,0x08,0x21,0xF0,0x1C,0x00,0x03,0xE0,0x1E,0x18,0x21,0x00,0x1E,0x00,0x00,
  //& 38
  0x00,0x1E,0xF0,0x21,0x08,0x23,0x88,0x24,0x70,0x19,0x00,0x27,0x00,0x21,0x00,0x10,
  //' 39
  0x10,0x00,0x16,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //( 40
  0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x07,0x18,0x18,0x04,0x20,0x02,0x40,0x00,0x00,
  //) 41
  0x00,0x00,0x02,0x40,0x04,0x20,0x18,0x18,0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,
  //* 42
  0x40,0x02,0x40,0x02,0x80,0x01,0xF0,0x0F,0x80,0x01,0x40,0x02,0x40,0x02,0x00,0x00,
  //+ 43
  0x00,0x01,0x00,0x01,0x00,0x01,0xF0,0x1F,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x00,
  //, 44
  0x00,0x80,0x00,0xB0,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //- 45
  0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
  //. 46
  0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  /// 47
  0x00,0x00,0x00,0x60,0x00,0x18,0x00,0x06,0x80,0x01,0x60,0x00,0x18,0x00,0x04,0x00,
  //0 48
  0x00,0x00,0xE0,0x0F,0x10,0x10,0x08,0x20,0x08,0x20,0x10,0x10,0xE0,0x0F,0x00,0x00,
  //1 49
  0x00,0x00,0x10,0x20,0x10,0x20,0xF8,0x3F,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x00,
  //2 50
  0x00,0x00,0x70,0x30,0x08,0x28,0x08,0x24,0x08,0x22,0x88,0x21,0x70,0x30,0x00,0x00,
  //3 51
  0x00,0x00,0x30,0x18,0x08,0x20,0x88,0x20,0x88,0x20,0x48,0x11,0x30,0x0E,0x00,0x00,
  //4 52
  0x00,0x00,0x00,0x07,0xC0,0x04,0x20,0x24,0x10,0x24,0xF8,0x3F,0x00,0x24,0x00,0x00,
  //5 53
  0x00,0x00,0xF8,0x19,0x08,0x21,0x88,0x20,0x88,0x20,0x08,0x11,0x08,0x0E,0x00,0x00,
  //6 54
  0x00,0x00,0xE0,0x0F,0x10,0x11,0x88,0x20,0x88,0x20,0x18,0x11,0x00,0x0E,0x00,0x00,
  //7 55
  0x00,0x00,0x38,0x00,0x08,0x00,0x08,0x3F,0xC8,0x00,0x38,0x00,0x08,0x00,0x00,0x00,
  //8 56
  0x00,0x00,0x70,0x1C,0x88,0x22,0x08,0x21,0x08,0x21,0x88,0x22,0x70,0x1C,0x00,0x00,
  //9 57
  0x00,0x00,0xE0,0x00,0x10,0x31,0x08,0x22,0x08,0x22,0x10,0x11,0xE0,0x0F,0x00,0x00,
  //: 58
  0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x30,0xC0,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
  //; 59
  0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //< 60
  0x00,0x00,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x08,0x10,0x10,0x08,0x20,0x00,0x00,
  //= 61
  0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x00,0x00,
  //> 62
  0x00,0x00,0x08,0x20,0x10,0x10,0x20,0x08,0x40,0x04,0x80,0x02,0x00,0x01,0x00,0x00,
  //? 63
  0x00,0x00,0x70,0x00,0x48,0x00,0x08,0x30,0x08,0x36,0x08,0x01,0xF0,0x00,0x00,0x00,
  //@ 64
  0xC0,0x07,0x30,0x18,0xC8,0x27,0x28,0x24,0xE8,0x23,0x10,0x14,0xE0,0x0B,0x00,0x00,
  //A 65
  0x00,0x20,0x00,0x3C,0xC0,0x23,0x38,0x02,0xE0,0x02,0x00,0x27,0x00,0x38,0x00,0x20,
  //B 66
  0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x20,0x88,0x20,0x70,0x11,0x00,0x0E,0x00,0x00,
  //C 67
  0xC0,0x07,0x30,0x18,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x10,0x38,0x08,0x00,0x00,
  //D 68
  0x08,0x20,0xF8,0x3F,0x08,0x20,0x08,0x20,0x08,0x20,0x10,0x10,0xE0,0x0F,0x00,0x00,
  //E 69
  0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x20,0xE8,0x23,0x08,0x20,0x10,0x18,0x00,0x00,
  //F 70
  0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x00,0xE8,0x03,0x08,0x00,0x10,0x00,0x00,0x00,
  //G 71
  0xC0,0x07,0x30,0x18,0x08,0x20,0x08,0x20,0x08,0x22,0x38,0x1E,0x00,0x02,0x00,0x00,
  //H 72
  0x08,0x20,0xF8,0x3F,0x08,0x21,0x00,0x01,0x00,0x01,0x08,0x21,0xF8,0x3F,0x08,0x20,
  //I 73
  0x00,0x00,0x08,0x20,0x08,0x20,0xF8,0x3F,0x08,0x20,0x08,0x20,0x00,0x00,0x00,0x00,
  //J 74
  0x00,0xC0,0x00,0x80,0x08,0x80,0x08,0x80,0xF8,0x7F,0x08,0x00,0x08,0x00,0x00,0x00,
  //K 75
  0x08,0x20,0xF8,0x3F,0x88,0x20,0xC0,0x01,0x28,0x26,0x18,0x38,0x08,0x20,0x00,0x00,
  //L 76
  0x08,0x20,0xF8,0x3F,0x08,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x30,0x00,0x00,
  //M 77
  0x08,0x20,0xF8,0x3F,0xF8,0x00,0x00,0x3F,0xF8,0x00,0xF8,0x3F,0x08,0x20,0x00,0x00,
  //N 78
  0x08,0x20,0xF8,0x3F,0x30,0x20,0xC0,0x00,0x00,0x07,0x08,0x18,0xF8,0x3F,0x08,0x00,
  //O 79
  0xE0,0x0F,0x10,0x10,0x08,0x20,0x08,0x20,0x08,0x20,0x10,0x10,0xE0,0x0F,0x00,0x00,
  //P 80
  0x08,0x20,0xF8,0x3F,0x08,0x21,0x08,0x01,0x08,0x01,0x08,0x01,0xF0,0x00,0x00,0x00,
  //Q 81
  0xE0,0x0F,0x10,0x18,0x08,0x24,0x08,0x24,0x08,0x38,0x10,0x50,0xE0,0x4F,0x00,0x00,
  //R 82
  0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x00,0x88,0x03,0x88,0x0C,0x70,0x30,0x00,0x20,
  //S 83
  0x08,0x20,0x18,0x30,0x68,0x2C,0x80,0x03,0x80,0x03,0x68,0x2C,0x18,0x30,0x08,0x20,
  //T 84
  0x18,0x00,0x08,0x00,0x08,0x20,0xF8,0x3F,0x08,0x20,0x08,0x00,0x18,0x00,0x00,0x00,
  //U 85
  0x08,0x00,0xF8,0x1F,0x08,0x20,0x00,0x20,0x00,0x20,0x08,0x20,0xF8,0x1F,0x08,0x00,
  //V 86
  0x08,0x00,0x78,0x00,0x88,0x07,0x00,0x38,0x00,0x0E,0xC8,0x01,0x38,0x00,0x08,0x00,
  //W 87
  0xF8,0x03,0x08,0x3C,0x00,0x07,0xF8,0x00,0x00,0x07,0x08,0x3C,0xF8,0x03,0x00,0x00,
  //X 88
  0x08,0x20,0x18,0x30,0x68,0x2C,0x80,0x03,0x80,0x03,0x68,0x2C,0x18,0x30,0x08,0x20,
  //Y 89
  0x08,0x00,0x38,0x00,0xC8,0x20,0x00,0x3F,0xC8,0x20,0x38,0x00,0x08,0x00,0x00,0x00,
  //Z 90
  0x10,0x20,0x08,0x38,0x08,0x26,0x08,0x21,0xC8,0x20,0x38,0x20,0x08,0x18,0x00,0x00,
  //[ 91
  0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x7F,0x02,0x40,0x02,0x40,0x02,0x40,0x00,0x00,
  //\ 92
  0x00,0x00,0x0C,0x00,0x30,0x00,0xC0,0x01,0x00,0x06,0x00,0x38,0x00,0xC0,0x00,0x00,
  //] 93
  0x00,0x00,0x02,0x40,0x02,0x40,0x02,0x40,0xFE,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,
  //^ 94
  0x00,0x00,0x00,0x00,0x04,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x00,0x00,
  //_ 95
  0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
  //` 96
  0x00,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //a 97
  0x00,0x00,0x00,0x19,0x80,0x24,0x80,0x22,0x80,0x22,0x80,0x22,0x00,0x3F,0x00,0x20,
  //b 98
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


void _LCD_Write(unsigned char data, unsigned char mode){   
  L_CS;
  if (mode==LCD_COMMAND)
    L_DC;
  else
    H_DC;

  for (unsigned char c=0; c<8; c++)
  {
    if (data & 0x80)
      H_MOSI;
    else
      L_MOSI;
    data = data<<1;
    L_SCK;
    asm ("nop");
    H_SCK;
  }
  H_CS;
}
void _InitLCD(){
  I_SCK;
  I_MOSI;
  I_DC;
  I_RST;
  I_CS;
  //resetLCD;
  H_DC;
  H_MOSI;
  H_SCK;
  H_CS;
  L_RST;
  delay(10);
  H_RST;

  _LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
  _LCD_Write(PCD8544_SETVOP | LCD_CONTRAST, LCD_COMMAND);
  _LCD_Write(PCD8544_SETTEMP | LCD_TEMP, LCD_COMMAND);
  _LCD_Write(PCD8544_SETBIAS | LCD_BIAS, LCD_COMMAND);
  _LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
  _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
  _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
  for (int c=0; c<504; c++)
    _LCD_Write(0x00, LCD_DATA);
  _LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);

  //_contrast=LCD_CONTRAST;
}
void setContrast(int contrast){
  if (contrast>0x7F)
    contrast=0x7F;
  if (contrast<0)
    contrast=0;
  _LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
  _LCD_Write(PCD8544_SETVOP | contrast, LCD_COMMAND);
  _LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
  //_contrast=contrast;
}
//Pos:0,1,2
void drawLine(char* line, uint8_t Pos){
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
    _LCD_Write(PCD8544_SETYADDR | (Pos*2+1), LCD_COMMAND);
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




void setup(){
  Serial.begin(115200);
  DDRD |= _BV(3);
  DDRB |= _BV(5);
  //BIT_SCL = _BV(7);//BMP180
  //BIT_SDA = _BV(6);//BMP180
  i2c_SoftI2CMaster();

  _InitLCD();
}

int TotalT = 0;
long TotalP = 0;
float TotalA = 0;
float baseA = 0;

char line[11];

void loop()
{
  uint32_t t0;
  
  {
    //BIT_SCL = _BV(7);//BMP180
    //BIT_SDA = _BV(6);//BMP180
    bmp085Calibration();
  }
    TotalT = 0;
    TotalP = 0;
    TotalA = 0;

  for(uint8_t i = 0;i<10;i++){
    PORTB &= ~_BV(5);
    getBMP180();
    PORTB |= _BV(5);
    TotalT += temperature;
    TotalP += pressure;
    TotalA += altitude;
    t0 = millis();
    while(millis() - t0<100);
  }
  Serial.println(TotalT);
  Serial.println(TotalP);
  Serial.println(TotalA);
  Serial.println(",");
}













