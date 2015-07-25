#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/*
   0
1     2
   3
4     5
   6
dataB0 clkA1 stA2 oeB2  2A 2B
 sdaB0 sclB1            1B
adj 3 4 5 6 7 (digital in)5A // 上下左右调整 按下左右默认在分上 Page
light sense A0           1A
11

74HC595(1):1~7(7) down 0 alarm
74HC595(2):2~7(6) up 0:1 time index date index
74HC595(3) week
直接用6个8显示星期 或者用秒的最后一位表示星期
星期1 8
星期2 88
星期3 888
星期4 8888
星期5 88888
星期6 888888
星期天 

LED 10
LED 420 （10+41）
PNP 6 NPN 6
NPN 7
左上高，右下低

*/

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define PORT_DAT_ON  PORTB |=  _BV(0)
#define PORT_DAT_OFF PORTB &= ~_BV(0)

#define PORT_CLK_ON  PORTA |=  _BV(1)
#define PORT_CLK_OFF PORTA &= ~_BV(1)

#define PORT_STR_ON  PORTA |=  _BV(2)
#define PORT_STR_OFF PORTA &= ~_BV(2)

#define PORT_OE_ON   PORTB |=  _BV(2)
#define PORT_OE_OFF  PORTB &= ~_BV(2)

#define PIN_ADJ1 PINA &= ~_BV(4)
#define PIN_ADJ2 PINA &= ~_BV(5)
#define PIN_ADJ3 PINA &= ~_BV(6)
#define PIN_ADJ4 PINA &= ~_BV(7)

#define NUM_SENS 3

#define DS1307_CTRL_ID 0x68//B01101000  //DS1307

//scl0 sda1
#define BIT_SCL  _BV(1)
#define DDR_SCL  DDRB
#define PORT_SCL PORTB

#define BIT_SDA  _BV(0)
#define DDR_SDA  DDRB
#define PORT_SDA PORTB
#define PIN_SDA  PINB

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


PROGMEM prog_uint8_t SEGS[] = {
0x77,//0=01110111=
0x12,//1=00010010=
0x5D,//2=01011101=
0x5B,//3=01011011=
0x3A,//4=00111010=
0x6B,//5=01101011=
0x6F,//6=01101111=
0x52,//7=01010010=
0x7F,//8=01111111=
0x7B,//9=01111011=
};

void ClockInit();

void loop();
void Init();
void Page(uint8_t* vals);
void SendWord(uint8_t data);

void dly();
void i2c_SoftI2CMaster();
uint8_t i2c_beginTransmission(uint8_t address);
uint8_t i2c_requestFrom(uint8_t address);
uint8_t i2c_endTransmission(void);
void i2c_write(uint8_t* data, uint8_t quantity);
void i2c_writebit(uint8_t c);
uint8_t i2c_readbit(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write(uint8_t c);
uint8_t i2c_read(uint8_t ack);
uint8_t i2c_read();
uint8_t i2c_readLast();

uint8_t bcdToDec(const uint8_t bcd);
uint8_t decToBcd(const uint8_t dec);
void DS1307_read();
void DS1307_save();

int analogRead(uint8_t pin);


uint8_t DS1307_SEC;// 0
uint8_t DS1307_MIN;// 1
uint8_t DS1307_HR;// 2
uint8_t DS1307_DOW;// 3
uint8_t DS1307_DATE;// 4
uint8_t DS1307_MTH;// 5
uint8_t DS1307_YR;// 6


int main(void) {
	ClockInit();
	DDRA|=_BV(0);
	DDRA|=_BV(1);
	DDRA|=_BV(2);
	DDRA|=_BV(3);
	
	loop();
	
	//
	analogRead(1);
	DS1307_read();
	DS1307_save();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

void loop() {
	for(;;)
	{
		uint8_t temp[6];
		temp[0] = 8;
		temp[1] = 8;
		temp[2] = 8;
		temp[3] = 8;
		temp[4] = 8;
		temp[5] = 8;
		
		Init();
		Page(temp);
	}
}

void Init(){
	for(uint8_t i=0;i<16;i++)
	{
		PORT_DAT_OFF;
		PORT_CLK_ON; //shift clock up
		PORT_CLK_OFF; //shift clock down
	}
	PORT_STR_ON; //store clock up
	PORT_STR_OFF; //store clock down
	PORT_OE_ON;
}

void Page(uint8_t* vals){
	for(uint8_t w=0;w<6;w++)
	{
		for(uint8_t l=0;l<7;l++)
		{
			uint8_t t = pgm_read_byte_near(SEGS+vals[w]);
			SendWord(t&(1<<l));//位线
			SendWord(1<<w);//字线
			PORT_STR_ON; //store clock up
			PORT_STR_OFF; //store clock down
		}
	}
}
void SendWord(uint8_t data){
	PORT_CLK_OFF;
	PORT_STR_OFF;
	for(uint8_t i=0;i<8;i++)
	{
		if(data&0x80)
		{
			PORT_DAT_ON;
		}
		else
		{
			PORT_DAT_OFF;
		}
		data<<1;
		PORT_CLK_ON; //shift clock up
		PORT_CLK_OFF; //shift clock down
	}
}
int analogRead(uint8_t pin){
	uint8_t low, high;

	ADCSRB |= _BV(ADLAR);
  
	ADMUX = (pin & 0x07);

	// without a delay, we seem to read from the wrong channel
	//delay(1);

	// start the conversion
	sbi(ADCSRA, ADSC);

	// ADSC is cleared when the conversion finishes
	while (bit_is_set(ADCSRA, ADSC));

	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
	low  = ADCL;
	high = ADCH;

	
	// combine the two bytes
	return (high << 8) | low;
}
















void dly(){
  for(uint8_t i=0;i<200;i++)
  {
    volatile uint8_t v=0;
    v++;
  }
}
void i2c_SoftI2CMaster(){
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
}
uint8_t i2c_beginTransmission(uint8_t address){
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 0); // clr read bit
  return rc;
}
uint8_t i2c_requestFrom(uint8_t address){
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 1); // set read bit
  return rc;
}
uint8_t i2c_endTransmission(void){
  i2c_stop();
  //return ret; // FIXME
  return 0;
}
void i2c_write(uint8_t* data, uint8_t quantity){
  for(uint8_t i = 0; i < quantity; ++i){
    i2c_write(data[i]);
  }
}
void i2c_writebit(uint8_t c){
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
uint8_t i2c_readbit(void){
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
  uint8_t c = PIN_SDA; // I2C_PIN;
  i2c_scl_lo();
  dly();
  return ( c & BIT_SDA) ? 1 : 0;
}
void i2c_start(void){
  // set both to high at the same time
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
  i2c_sda_lo();
  dly();
  i2c_scl_lo();
  dly();
}
void i2c_stop(void){
  i2c_scl_hi();
  dly();
  i2c_sda_hi();
  dly();
}
uint8_t i2c_write(uint8_t c){
  for ( uint8_t i=0;i<8;i++) {
    i2c_writebit( c & 128 );
    c<<=1;
  }
  return i2c_readbit();
}
uint8_t i2c_read(uint8_t ack){
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
uint8_t i2c_read(){
  return i2c_read( I2C_ACK );
}
uint8_t i2c_readLast(){
  return i2c_read( I2C_NAK );
}








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
void DS1307_read(){
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
void DS1307_save(){
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


void setup()
{
  //delay(1000);
  //Serial.begin(9600);
  //Serial.setTimeout(1000);
  //i2c_SoftI2CMaster();
  //pinMode(13, OUTPUT);
}

void loopa()
{
  //digitalWrite(13, HIGH);
  //DS1307_read();
  //char buf[60];
  //snprintf(buf, sizeof(buf), "DS1307:%02d 20%02d-%02d-%02d %02d:%02d:%02d",DS1307_DOW, DS1307_YR, DS1307_MTH, DS1307_DATE, DS1307_HR, DS1307_MIN, DS1307_SEC);
  //Serial.println(buf);
  //if(DS1307_SEC!=80)
  //{
  //  digitalWrite(13, LOW);
  //}
  //int start = Serial.parseInt();
  //if(start == 33)
  //{
  //  DS1307_YR  = Serial.parseInt();
  //  DS1307_MTH = Serial.parseInt();
  //  DS1307_DATE = Serial.parseInt();
  //  DS1307_HR  = Serial.parseInt();
  //  DS1307_MIN = Serial.parseInt();
  //  DS1307_SEC = Serial.parseInt();
  //  DS1307_DOW= Serial.parseInt();
  //
  //  DS1307_save();
  //}
}
