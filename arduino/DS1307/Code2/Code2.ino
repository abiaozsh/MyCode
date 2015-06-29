#include <Wire.h>

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
  Wire.beginTransmission(DS1307_CTRL_ID);
    Wire.write(0x00);
  Wire.endTransmission();

  // request the 7 bytes of data    (secs, min, hr, dow, date. mth, yr)
  Wire.requestFrom(DS1307_CTRL_ID, 7);
    // store data in raw bcd format
	DS1307_SEC=bcdToDec(Wire.read());// 0
	DS1307_MIN=bcdToDec(Wire.read());// 1
	DS1307_HR=bcdToDec(Wire.read());// 2
	DS1307_DOW=Wire.read();// 3
	DS1307_DATE=bcdToDec(Wire.read());// 4
	DS1307_MTH=bcdToDec(Wire.read());// 5
	DS1307_YR=bcdToDec(Wire.read());// 6
}

// update the data on the IC from the bcd formatted data in the buffer
void DS1307_save()
{
  Wire.beginTransmission(DS1307_CTRL_ID);
  Wire.write(0x00); // reset register pointer
    Wire.write(decToBcd(DS1307_SEC));
    Wire.write(decToBcd(DS1307_MIN));
    Wire.write(decToBcd(DS1307_HR));
    Wire.write(DS1307_DOW);
    Wire.write(decToBcd(DS1307_DATE));
    Wire.write(decToBcd(DS1307_MTH));
    Wire.write(decToBcd(DS1307_YR));
  Wire.endTransmission();
}


#include <Wire.h>
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup()
{
  delay(5000);
  Serial.begin(9600);
  Serial.setTimeout(1000);
  Wire.begin();
  //sbi(TWSR, TWPS0);
  //sbi(TWSR, TWPS1);
  //TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;

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

