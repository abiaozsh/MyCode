#include <util/delay.h>
class SoftI2CMaster
{
	private:
	// per object data
	uint8_t _sclPin;
	uint8_t _sdaPin;
	uint8_t _sclBitMask;
	uint8_t _sdaBitMask;
	volatile uint8_t *_sclPortReg;
	volatile uint8_t *_sdaPortReg;
	volatile uint8_t *_sclDirReg;
	volatile uint8_t *_sdaDirReg;
	uint8_t usePullups;
	// private methods
	void i2c_writebit( uint8_t c );
	uint8_t i2c_readbit(void);
	void i2c_init(void);
	void i2c_start(void);
	void i2c_repstart(void);
	void i2c_stop(void);
	uint8_t i2c_write( uint8_t c );
	uint8_t i2c_read( uint8_t ack );
	public:
	// public methods
	SoftI2CMaster();
	SoftI2CMaster(uint8_t sclPin, uint8_t sdaPin);
	SoftI2CMaster(uint8_t sclPin, uint8_t sdaPin, uint8_t usePullups);
	void setPins(uint8_t sclPin, uint8_t sdaPin, uint8_t usePullups);
	uint8_t beginTransmission(uint8_t address);
	uint8_t beginTransmission(int address);
	uint8_t endTransmission(void);
	uint8_t write(uint8_t);
	void write(uint8_t*, uint8_t);
	void write(int);
	void write(char*);
	uint8_t requestFrom(int address);
	uint8_t requestFrom(uint8_t address);
	uint8_t read( uint8_t ack );
	uint8_t read();
	uint8_t readLast();
};







#include <string.h>
#define i2cbitdelay 50
#define I2C_ACK 1
#define I2C_NAK 0
#define i2c_scl_release() *_sclDirReg &=~ _sclBitMask
#define i2c_sda_release() *_sdaDirReg &=~ _sdaBitMask
// sets SCL low and drives output
#define i2c_scl_lo() *_sclPortReg &=~ _sclBitMask; *_sclDirReg |= _sclBitMask;
// sets SDA low and drives output
#define i2c_sda_lo() *_sdaPortReg &=~ _sdaBitMask; *_sdaDirReg |= _sdaBitMask;
// set SCL high and to input (releases pin) (i.e. change to input,turnon pullup)
#define i2c_scl_hi() *_sclDirReg &=~ _sclBitMask; if(usePullups) { *_sclPortReg |= _sclBitMask; }
// set SDA high and to input (releases pin) (i.e. change to input,turnon pullup)
#define i2c_sda_hi() *_sdaDirReg &=~ _sdaBitMask; if(usePullups) { *_sdaPortReg |= _sdaBitMask; }
//
// Constructor
//
SoftI2CMaster::SoftI2CMaster()
{
// do nothing, use setPins() later
}
//
SoftI2CMaster::SoftI2CMaster(uint8_t sclPin, uint8_t sdaPin)
{
setPins(sclPin, sdaPin, true);
i2c_init();
}
//
SoftI2CMaster::SoftI2CMaster(uint8_t sclPin, uint8_t sdaPin, uint8_t pullups)
{
setPins(sclPin, sdaPin, pullups);
i2c_init();
}
//
// Turn Arduino pin numbers into PORTx, DDRx, and PINx
//
void SoftI2CMaster::setPins(uint8_t sclPin, uint8_t sdaPin, uint8_t pullups)
{
uint8_t port;
usePullups = pullups;
_sclPin = sclPin;
_sdaPin = sdaPin;
_sclBitMask = digitalPinToBitMask(sclPin);
_sdaBitMask = digitalPinToBitMask(sdaPin);
port = digitalPinToPort(sclPin);
_sclPortReg = portOutputRegister(port);
_sclDirReg = portModeRegister(port);
port = digitalPinToPort(sdaPin);
_sdaPortReg = portOutputRegister(port);
_sdaDirReg = portModeRegister(port);
}
//
//
//
uint8_t SoftI2CMaster::beginTransmission(uint8_t address)
{
i2c_start();
uint8_t rc = i2c_write((address<<1) | 0); // clr read bit
return rc;
}
//
uint8_t SoftI2CMaster::requestFrom(uint8_t address)
{
i2c_start();
uint8_t rc = i2c_write((address<<1) | 1); // set read bit
return rc;
}
//
uint8_t SoftI2CMaster::requestFrom(int address)
{
return requestFrom( (uint8_t) address);
}
//
uint8_t SoftI2CMaster::beginTransmission(int address)
{
return beginTransmission((uint8_t)address);
}
//
//
//
uint8_t SoftI2CMaster::endTransmission(void)
{
i2c_stop();
//return ret; // FIXME
return 0;
}
// must be called in:
// slave tx event callback
// or after beginTransmission(address)
uint8_t SoftI2CMaster::write(uint8_t data)
{
return i2c_write(data);
}
// must be called in:
// slave tx event callback
// or after beginTransmission(address)
void SoftI2CMaster::write(uint8_t* data, uint8_t quantity)
{
for(uint8_t i = 0; i < quantity; ++i){
write(data[i]);
}
}
// must be called in:
// slave tx event callback
// or after beginTransmission(address)
void SoftI2CMaster::write(char* data)
{
write((uint8_t*)data, strlen(data));
}
// must be called in:
// slave tx event callback
// or after beginTransmission(address)
void SoftI2CMaster::write(int data)
{
write((uint8_t)data);
}
//--------------------------------------------------------------------
void SoftI2CMaster::i2c_writebit( uint8_t c )
{
if ( c > 0 ) {
i2c_sda_hi();
} else {
i2c_sda_lo();
}
i2c_scl_hi();
_delay_us(i2cbitdelay);
i2c_scl_lo();
_delay_us(i2cbitdelay);
if ( c > 0 ) {
i2c_sda_lo();
}
_delay_us(i2cbitdelay);
}
//
uint8_t SoftI2CMaster::i2c_readbit(void)
{
i2c_sda_hi();
i2c_scl_hi();
_delay_us(i2cbitdelay);
uint8_t port = digitalPinToPort(_sdaPin);
volatile uint8_t* pinReg = portInputRegister(port);
uint8_t c = *pinReg; // I2C_PIN;
i2c_scl_lo();
_delay_us(i2cbitdelay);
return ( c & _sdaBitMask) ? 1 : 0;
}
// Inits bitbanging port, must be called before using the functions below
//
void SoftI2CMaster::i2c_init(void)
{
//I2C_PORT &=~ (_BV( I2C_SDA ) | _BV( I2C_SCL ));
//*_sclPortReg &=~ (_sdaBitMask | _sclBitMask);
i2c_sda_hi();
i2c_scl_hi();
_delay_us(i2cbitdelay);
}
// Send a START Condition
//
void SoftI2CMaster::i2c_start(void)
{
// set both to high at the same time
//I2C_DDR &=~ (_BV( I2C_SDA ) | _BV( I2C_SCL ));
//*_sclDirReg &=~ (_sdaBitMask | _sclBitMask);
i2c_sda_hi();
i2c_scl_hi();
_delay_us(i2cbitdelay);
i2c_sda_lo();
_delay_us(i2cbitdelay);
i2c_scl_lo();
_delay_us(i2cbitdelay);
}
void SoftI2CMaster::i2c_repstart(void)
{
// set both to high at the same time (releases drive on both lines)
//I2C_DDR &=~ (_BV( I2C_SDA ) | _BV( I2C_SCL ));
//*_sclDirReg &=~ (_sdaBitMask | _sclBitMask);
i2c_sda_hi();
i2c_scl_hi();
i2c_scl_lo(); // force SCL low
_delay_us(i2cbitdelay);
i2c_sda_release(); // release SDA
_delay_us(i2cbitdelay);
i2c_scl_release(); // release SCL
_delay_us(i2cbitdelay);
i2c_sda_lo(); // force SDA low
_delay_us(i2cbitdelay);
}
// Send a STOP Condition
//
void SoftI2CMaster::i2c_stop(void)
{
i2c_scl_hi();
_delay_us(i2cbitdelay);
i2c_sda_hi();
_delay_us(i2cbitdelay);
}
// write a byte to the I2C slave device
//
uint8_t SoftI2CMaster::i2c_write( uint8_t c )
{
for ( uint8_t i=0;i<8;i++) {
i2c_writebit( c & 128 );
c<<=1;
}
return i2c_readbit();
}
// read a byte from the I2C slave device
//
uint8_t SoftI2CMaster::i2c_read( uint8_t ack )
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
_delay_us(i2cbitdelay);
return res;
}
// FIXME: this isn't right, surely
uint8_t SoftI2CMaster::read( uint8_t ack )
{
return i2c_read( ack );
}
//
uint8_t SoftI2CMaster::read()
{
return i2c_read( I2C_ACK );
}
//
uint8_t SoftI2CMaster::readLast()
{
return i2c_read( I2C_NAK );
}



const byte sclPin = A5; // digital pin 7 wired to 'c' on BlinkM
const byte sdaPin = A4; // digital pin 6 wired to 'd' on BlinkM



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

SoftI2CMaster i2c = SoftI2CMaster(sclPin,sdaPin,false);

void setup(){
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  bmp085Calibration();
  Serial.println();
  delay(1000);
}

void loop()
{
  float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  float pressure = bmp085GetPressure(bmp085ReadUP());
  float altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(pressure);
  Serial.print(",");
  Serial.print(altitude);
  Serial.println();
  delay(200);
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
  i2c.beginTransmission(BMP085_ADDRESS);
  i2c.write(address);
  i2c.endTransmission();
  i2c.requestFrom(BMP085_ADDRESS);
  uint8_t msb = i2c.read();
  Serial.print(msb);
  Serial.print(",");
  return msb;
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;

  i2c.beginTransmission(BMP085_ADDRESS);
  i2c.write(address);
  i2c.endTransmission();

  i2c.requestFrom(BMP085_ADDRESS);

  msb = i2c.read();
  lsb = i2c.read();
  Serial.print(msb);
  Serial.print(",");
  Serial.print(lsb);
  Serial.print(",");

  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT(){
  unsigned int ut;

  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  i2c.beginTransmission(BMP085_ADDRESS);
  i2c.write(0xF4);
  i2c.write(0x2E);
  i2c.endTransmission();

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
  i2c.beginTransmission(BMP085_ADDRESS);
  i2c.write(0xF4);
  i2c.write(0x34 + (OSS<<6));
  i2c.endTransmission();

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

