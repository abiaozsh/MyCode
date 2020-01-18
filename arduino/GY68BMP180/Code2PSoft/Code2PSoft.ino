

void dly()
{
	for(uint8_t i=0;i<200;i++)
	{
		volatile uint8_t v=0;
		v++;
	}
}
#define BIT_SCL  _BV(1)
#define DDR_SCL  DDRC
#define PORT_SCL PORTC

#define BIT_SDA  _BV(0)
#define DDR_SDA  DDRC
#define PORT_SDA PORTC
#define PIN_SDA  PINC

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
} else {
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


void setup(){
  Serial.begin(9600);
  Serial.println();
  i2c_SoftI2CMaster();
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

