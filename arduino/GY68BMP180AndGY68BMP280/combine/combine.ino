
#define DDR_SCL  DDRC
#define PORT_SCL PORTC

#define DDR_SDA  DDRC
#define PORT_SDA PORTC
#define PIN_SDA  PINC

#define BIT_SCL_280 _BV(2)
#define BIT_SDA_280 _BV(3)

#define BIT_SCL_180 _BV(1)
#define BIT_SDA_180 _BV(0)

volatile uint8_t is280=1;

void i2c_SoftI2CMaster()
{
i2c_sda_hi();
i2c_scl_hi();
dly();
}

void i2c_scl_release(){
  if(is280){
    DDR_SCL &= ~BIT_SCL_280;
  }else{
    DDR_SCL &= ~BIT_SCL_180;
  }
}

void i2c_sda_release(){
  if(is280){
    DDR_SDA &= ~BIT_SDA_280;
  }else{
    DDR_SDA &= ~BIT_SDA_180;
  }
}
// sets SCL low and drives output
void i2c_scl_lo(){
  if(is280){
    PORT_SCL &= ~BIT_SCL_280; DDR_SCL |= BIT_SCL_280;
  }else{
    PORT_SCL &= ~BIT_SCL_180; DDR_SCL |= BIT_SCL_180;
  }
}
// sets SDA low and drives output
void i2c_sda_lo(){
  if(is280){
    PORT_SDA &= ~BIT_SDA_280; DDR_SDA |= BIT_SDA_280;
  }else{
    PORT_SDA &= ~BIT_SDA_180; DDR_SDA |= BIT_SDA_180;
  }
}
// set SCL high and to input (releases pin) (i.e. change to input,turnon pullup)
void i2c_scl_hi(){
  if(is280){
    DDR_SCL &=~ BIT_SCL_280;
  }else{
    DDR_SCL &=~ BIT_SCL_180;
  }
}
// set SDA high and to input (releases pin) (i.e. change to input,turnon pullup)
void i2c_sda_hi(){
  if(is280){
    DDR_SDA &=~ BIT_SDA_280;
  }else{
    DDR_SDA &=~ BIT_SDA_180;
  }
}

#define I2C_ACK 1
#define I2C_NAK 0

void dly()
{
  for(uint8_t i=0;i<16;i++)//6 is stable
  {
    volatile uint8_t v=0;
    v++;
  }
}

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
uint8_t i2c_readbit()
{
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
  uint8_t c = PIN_SDA; // I2C_PIN;
  i2c_scl_lo();
  dly();
  if(is280){
    return ( c & BIT_SDA_280) ? 1 : 0;
  }else{
    return ( c & BIT_SDA_180) ? 1 : 0;
  }
  
}

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

uint8_t i2c_beginTransmission(uint8_t address)
{
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 0); // clr read bit
  return rc;
}

void i2c_endTransmission()
{
  i2c_scl_hi();
  dly();
  i2c_sda_hi();
  dly();
}
uint8_t i2c_requestFrom(uint8_t address)
{
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 1); // set read bit
  return rc;
}







 #include "Arduino.h"


/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
   // #define BMP280_ADDRESS                (0x77)
	 #define BMP280_ADDRESS                (0x76)
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
    enum
    {
      BMP280_REGISTER_DIG_T1              = 0x88,
      BMP280_REGISTER_DIG_T2              = 0x8A,
      BMP280_REGISTER_DIG_T3              = 0x8C,

      BMP280_REGISTER_DIG_P1              = 0x8E,
      BMP280_REGISTER_DIG_P2              = 0x90,
      BMP280_REGISTER_DIG_P3              = 0x92,
      BMP280_REGISTER_DIG_P4              = 0x94,
      BMP280_REGISTER_DIG_P5              = 0x96,
      BMP280_REGISTER_DIG_P6              = 0x98,
      BMP280_REGISTER_DIG_P7              = 0x9A,
      BMP280_REGISTER_DIG_P8              = 0x9C,
      BMP280_REGISTER_DIG_P9              = 0x9E,

      BMP280_REGISTER_CHIPID             = 0xD0,
      BMP280_REGISTER_VERSION            = 0xD1,
      BMP280_REGISTER_SOFTRESET          = 0xE0,

      BMP280_REGISTER_CAL26              = 0xE1,  // R calibration stored in 0xE1-0xF0

      BMP280_REGISTER_CONTROL            = 0xF4,
      BMP280_REGISTER_CONFIG             = 0xF5,
      BMP280_REGISTER_PRESSUREDATA       = 0xF7,
      BMP280_REGISTER_TEMPDATA           = 0xFA,
    };

/*=========================================================================*/

/*=========================================================================
    CALIBRATION DATA
    -----------------------------------------------------------------------*/
    typedef struct
    {
      uint16_t dig_T1;
      int16_t  dig_T2;
      int16_t  dig_T3;

      uint16_t dig_P1;
      int16_t  dig_P2;
      int16_t  dig_P3;
      int16_t  dig_P4;
      int16_t  dig_P5;
      int16_t  dig_P6;
      int16_t  dig_P7;
      int16_t  dig_P8;
      int16_t  dig_P9;

      uint8_t  dig_H1;
      int16_t  dig_H2;
      uint8_t  dig_H3;
      int16_t  dig_H4;
      int16_t  dig_H5;
      int8_t   dig_H6;
    } bmp280_calib_data;
/*=========================================================================*/

/*
class Adafruit_BMP280_Unified : public Adafruit_Sensor
{
  public:
    Adafruit_BMP280_Unified(int32_t sensorID = -1);
  
    bool  begin(uint8_t addr = BMP280_ADDRESS);
    void  getTemperature(float *temp);
    void  getPressure(float *pressure);
    float pressureToAltitude(float seaLevel, float atmospheric, float temp);
    float seaLevelForAltitude(float altitude, float atmospheric, float temp);
    void  getEvent(sensors_event_t*);
    void  getSensor(sensor_t*);

  private:
    uint8_t   _i2c_addr;
    int32_t   _sensorID;
};

*/

class Adafruit_BMP280
{
  public:
    Adafruit_BMP280(void);
    Adafruit_BMP280(int8_t cspin);
    Adafruit_BMP280(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin);

    bool  begin(uint8_t addr = BMP280_ADDRESS);
    float readTemperature(void);
    float readPressure(void);

    //  float pressureToAltitude(float seaLevel, float atmospheric, float temp);
    // float seaLevelForAltitude(float altitude, float atmospheric, float temp);

  private:

    void readCoefficients(void);

    void      write8(byte reg, byte value);
    uint8_t   read8(byte reg);
    uint16_t  read16(byte reg);
    int16_t   readS16(byte reg);
    uint16_t  read16_LE(byte reg); // little endian
    int16_t   readS16_LE(byte reg); // little endian

    uint8_t   _i2caddr;
    int32_t   _sensorID;
    int32_t t_fine;


};







static bmp280_calib_data bmp280_calib; 

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/


Adafruit_BMP280::Adafruit_BMP280() 
{ }



bool Adafruit_BMP280::begin(uint8_t a) {
  _i2caddr = a;

    // i2c
    //Wire.begin();

  if (read8(BMP280_REGISTER_CHIPID) != 0x58)
    return false;

  readCoefficients();
  write8(BMP280_REGISTER_CONTROL, 0x3F);
  return true;
}

/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over I2C/SPI
*/
/**************************************************************************/
void Adafruit_BMP280::write8(byte reg, byte value)
{
    i2c_beginTransmission((uint8_t)_i2caddr);
    i2c_write((uint8_t)reg);
    i2c_write((uint8_t)value);
    i2c_endTransmission();
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
uint8_t Adafruit_BMP280::read8(byte reg)
{
  uint8_t value;
  
    i2c_beginTransmission((uint8_t)_i2caddr);
    i2c_write((uint8_t)reg);
    i2c_endTransmission();
    i2c_requestFrom((uint8_t)_i2caddr);
    value = i2c_readLast();
    i2c_endTransmission();
  return value;
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit value over I2C
*/
/**************************************************************************/
uint16_t Adafruit_BMP280::read16(byte reg)
{
  uint16_t value;

    i2c_beginTransmission((uint8_t)_i2caddr);
    i2c_write((uint8_t)reg);
    i2c_endTransmission();
    i2c_requestFrom((uint8_t)_i2caddr);
    uint8_t v1 = i2c_read();
    uint8_t v2 = i2c_readLast();
    value = (v1 << 8) | v2;
    i2c_endTransmission();

  return value;
}

uint16_t Adafruit_BMP280::read16_LE(byte reg) {
  uint16_t temp = read16(reg);
  return (temp >> 8) | (temp << 8);
  
}

/**************************************************************************/
/*!
    @brief  Reads a signed 16 bit value over I2C
*/
/**************************************************************************/
int16_t Adafruit_BMP280::readS16(byte reg)
{
  return (int16_t)read16(reg);

}

int16_t Adafruit_BMP280::readS16_LE(byte reg)
{
  return (int16_t)read16_LE(reg);

}

/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/
void Adafruit_BMP280::readCoefficients(void)
{
    bmp280_calib.dig_T1 = read16_LE(BMP280_REGISTER_DIG_T1);
    bmp280_calib.dig_T2 = readS16_LE(BMP280_REGISTER_DIG_T2);
    bmp280_calib.dig_T3 = readS16_LE(BMP280_REGISTER_DIG_T3);

    bmp280_calib.dig_P1 = read16_LE(BMP280_REGISTER_DIG_P1);
    bmp280_calib.dig_P2 = readS16_LE(BMP280_REGISTER_DIG_P2);
    bmp280_calib.dig_P3 = readS16_LE(BMP280_REGISTER_DIG_P3);
    bmp280_calib.dig_P4 = readS16_LE(BMP280_REGISTER_DIG_P4);
    bmp280_calib.dig_P5 = readS16_LE(BMP280_REGISTER_DIG_P5);
    bmp280_calib.dig_P6 = readS16_LE(BMP280_REGISTER_DIG_P6);
    bmp280_calib.dig_P7 = readS16_LE(BMP280_REGISTER_DIG_P7);
    bmp280_calib.dig_P8 = readS16_LE(BMP280_REGISTER_DIG_P8);
    bmp280_calib.dig_P9 = readS16_LE(BMP280_REGISTER_DIG_P9);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float Adafruit_BMP280::readTemperature(void)
{
  int32_t var1, var2;
  
  int32_t adc_T = read16(BMP280_REGISTER_TEMPDATA);
  adc_T <<= 8;
  adc_T |= read8(BMP280_REGISTER_TEMPDATA+2);
  adc_T >>= 4;

  var1  = ((((adc_T>>3) - ((int32_t)bmp280_calib.dig_T1 <<1))) * 
	   ((int32_t)bmp280_calib.dig_T2)) >> 11;

  var2  = (((((adc_T>>4) - ((int32_t)bmp280_calib.dig_T1)) * 
	     ((adc_T>>4) - ((int32_t)bmp280_calib.dig_T1))) >> 12) * 
	   ((int32_t)bmp280_calib.dig_T3)) >> 14;

  t_fine = var1 + var2;

  float T  = (t_fine * 5 + 128) >> 8;
  return T/100;
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float Adafruit_BMP280::readPressure(void) {
  int64_t var1, var2, p;
  
  int32_t adc_P = read16(BMP280_REGISTER_PRESSUREDATA);
  adc_P <<= 8;
  adc_P |= read8(BMP280_REGISTER_PRESSUREDATA+2);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)bmp280_calib.dig_P6;
  var2 = var2 + ((var1*(int64_t)bmp280_calib.dig_P5)<<17);
  var2 = var2 + (((int64_t)bmp280_calib.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)bmp280_calib.dig_P3)>>8) +
    ((var1 * (int64_t)bmp280_calib.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)bmp280_calib.dig_P1)>>33;
  
  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)bmp280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)bmp280_calib.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280_calib.dig_P7)<<4);
  return (float)p/256;
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












Adafruit_BMP280 bme; // I2C
  
void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));
  
  is280=0;
  i2c_SoftI2CMaster();
  bmp085Calibration();
  is280=1;
  
  if (!bme.begin()) {  
    Serial.println("Could not find!");
    while (1);
  }
}
  
void loop() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.print(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure());
    Serial.print(" Pa      ");
    delay(100);
    
    is280=0;
    float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
    float pressure = bmp085GetPressure(bmp085ReadUP());
    float altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 
    Serial.print(temperature);
    Serial.print(",");
    Serial.print(pressure);
    Serial.print(",");
    Serial.print(altitude);
    Serial.println();
    is280=1;
    delay(100);

    
}
