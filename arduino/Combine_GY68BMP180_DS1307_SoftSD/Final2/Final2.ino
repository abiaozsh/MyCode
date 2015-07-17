#include <SD.h>


void dly()
{
  for(uint8_t i=0;i<100;i++)
  {
    volatile uint8_t v=0;
    v++;
  }
}
#define DDR_SCL  DDRD
#define PORT_SCL PORTD

#define DDR_SDA  DDRD
#define PORT_SDA PORTD
#define PIN_SDA  PIND

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



uint8_t BIT_SCL = 0;
uint8_t BIT_SDA = 0;

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
//float altitude; //Uncompensated caculation - in Meters 

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







uint8_t DS1307_SEC;// 0
uint8_t DS1307_MIN;// 1
uint8_t DS1307_HR;// 2
uint8_t DS1307_DOW;// 3
uint8_t DS1307_DATE;// 4
uint8_t DS1307_MTH;// 5
uint8_t DS1307_YR;// 6

#define DS1307_CTRL_ID 0x68 //B01101000  //DS1307

uint8_t bcdToDec(const uint8_t bcd) {
  return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}

uint8_t decToBcd(const uint8_t dec) {
  const uint8_t tens = dec / 10;
  const uint8_t ones = dec % 10;
  return (tens << 4) | ones;
}

// Aquire data from the RTC chip in BCD format
// refresh the buffer
void DS1307_read()
{
  BIT_SCL = _BV(5);//DS1307
  BIT_SDA = _BV(4);//DS1307
  // use the Wire lib to connect to tho rtc
  // reset the resgiter pointer to zero
  i2c_beginTransmission(DS1307_CTRL_ID);
  i2c_write(0x00);
  i2c_endTransmission();

  // request the 7 bytes of data    (secs, min, hr, dow, date. mth, yr)
  i2c_requestFrom(DS1307_CTRL_ID);
  // store data in raw bcd format
  DS1307_SEC=bcdToDec(i2c_read());// 0
  DS1307_MIN=bcdToDec(i2c_read());// 1
  DS1307_HR=bcdToDec(i2c_read());// 2
  DS1307_DOW=i2c_read();// 3
  DS1307_DATE=bcdToDec(i2c_read());// 4
  DS1307_MTH=bcdToDec(i2c_read());// 5
  DS1307_YR=bcdToDec(i2c_readLast());// 6
}

// update the data on the IC from the bcd formatted data in the buffer
void DS1307_save()
{
  BIT_SCL = _BV(5);//DS1307
  BIT_SDA = _BV(4);//DS1307
  i2c_beginTransmission(DS1307_CTRL_ID);
  i2c_write(0x00); // reset register pointer
  i2c_write(decToBcd(DS1307_SEC));
  i2c_write(decToBcd(DS1307_MIN));
  i2c_write(decToBcd(DS1307_HR));
  i2c_write(DS1307_DOW);
  i2c_write(decToBcd(DS1307_DATE));
  i2c_write(decToBcd(DS1307_MTH));
  i2c_write(decToBcd(DS1307_YR));
  i2c_endTransmission();
}





File myFile;


void getBMP180()
{
  BIT_SCL = _BV(7);//BMP180
  BIT_SDA = _BV(6);//BMP180
  temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  pressure = bmp085GetPressure(bmp085ReadUP());
  //altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 
}


void setup(){
  Serial.begin(115200);
  pinMode(3, OUTPUT);
  BIT_SCL = _BV(7);//BMP180
  BIT_SDA = _BV(6);//BMP180
  i2c_SoftI2CMaster();

  BIT_SCL = _BV(5);//DS1307
  BIT_SDA = _BV(4);//DS1307
  i2c_SoftI2CMaster();

  DS1307_read();

  if(DS1307_SEC == 80)
  {
    while(true)
    {
      Serial.println("Time error:");
      int cmd = Serial.parseInt();
      if(cmd == 29)
      {
        //29,15,7,17,20,39,00,5,
        DS1307_YR  = Serial.parseInt();
        DS1307_MTH = Serial.parseInt();
        DS1307_DATE = Serial.parseInt();
        DS1307_HR  = Serial.parseInt();
        DS1307_MIN = Serial.parseInt();
        DS1307_SEC = Serial.parseInt();
        DS1307_DOW= Serial.parseInt();

        DS1307_save();

        DS1307_read();
        char buf[30];
        //                          2015-01-01 12:00:00
        snprintf(buf, sizeof(buf), "20%02d-%02d-%02d %02d:%02d:%02d",DS1307_YR, DS1307_MTH, DS1307_DATE, DS1307_HR, DS1307_MIN, DS1307_SEC);
        Serial.println(buf);
        break;
      }
      //if(digitalRead(2)==LOW)
      //{
      //  break;
      //}
    }
  }
  digitalWrite(3, HIGH);
  while (!SD.begin()) {
    Serial.println("SD not ready.");
    delay(1000);
  }
  digitalWrite(3, LOW);
}

int a_t[50];
long a_p[50];

void loop()
{
  BIT_SCL = _BV(7);//BMP180
  BIT_SDA = _BV(6);//BMP180
  bmp085Calibration();

  DS1307_read();
  char buf[20];
  snprintf(buf, sizeof(buf), "%02d%02d%02d.TXT",DS1307_YR, DS1307_MTH, DS1307_DATE);
  Serial.print(buf);
  char buf2[10];
  snprintf(buf2, sizeof(buf2), "%02d:%02d:%02d",DS1307_HR, DS1307_MIN, DS1307_SEC);
  Serial.println(buf2);
  
  for(int i=0;i<50;i++)
  {
    uint32_t t0 = millis();
    digitalWrite(3, HIGH);
    getBMP180();
    a_t[i] = temperature;
    a_p[i] = pressure;
    Serial.print("t:");
    Serial.print(temperature);
    Serial.print(",p:");
    Serial.print(pressure);
    Serial.println();
    digitalWrite(3, LOW);
    while(millis() - t0<200);
  }

  digitalWrite(3, HIGH);
  myFile = SD.openSimple(buf, O_WRITE|O_CREAT, 1);
  if (myFile) {
    myFile.println(buf2);
    for(int i=0;i<50;i++)
    {
      myFile.print(a_t[i]);
      myFile.print(",");
      myFile.println(a_p[i]);
    }
    //myFile.print(",");
    //myFile.print(altitude);
    myFile.println();
    myFile.close();
    digitalWrite(3, LOW);
  }
  else {
    Serial.println("error.");
  }
}












