

void dly()
{
	for(uint8_t i=0;i<200;i++)
	{
		volatile uint8_t v=0;
		v++;
	}
}
#define BIT_SCL  _BV(5)
#define DDR_SCL  DDRC
#define PORT_SCL PORTC

#define BIT_SDA  _BV(4)
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

















uint8_t DS1307_SEC;// 0
uint8_t DS1307_MIN;// 1
uint8_t DS1307_HR;// 2
uint8_t DS1307_DOW;// 3
uint8_t DS1307_DATE;// 4
uint8_t DS1307_MTH;// 5
uint8_t DS1307_YR;// 6

#define DS1307_CTRL_ID B1101000  //DS1307

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


#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup()
{
  delay(5000);
  Serial.begin(9600);
  Serial.setTimeout(1000);
  i2c_SoftI2CMaster();
  pinMode(13, OUTPUT);
}

void loop()
{
  digitalWrite(13, HIGH);
  DS1307_read();

  	char buf[60];
	snprintf(buf, sizeof(buf), "DS1307:%02d 20%02d-%02d-%02d %02d:%02d:%02d",DS1307_DOW, DS1307_YR, DS1307_MTH, DS1307_DATE, DS1307_HR, DS1307_MIN, DS1307_SEC);
	Serial.println(buf);
	digitalWrite(13, LOW);
	int start = Serial.parseInt();
	if(start == 33)
	{
		DS1307_YR  = Serial.parseInt();
		DS1307_MTH = Serial.parseInt();
		DS1307_DATE = Serial.parseInt();
		DS1307_HR  = Serial.parseInt();
		DS1307_MIN = Serial.parseInt();
		DS1307_SEC = Serial.parseInt();
		DS1307_DOW= Serial.parseInt();

		DS1307_save();
	}
}

//33,15,6,26,20,39,00,5,

