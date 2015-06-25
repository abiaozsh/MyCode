/*
  DS1307.h - library for DS1307 rtc
*/

// ensure this library description is only included once
#ifndef DS1307_h
#define DS1307_h

// include types & constants of Wiring core API
#include <WConstants.h>

// include types & constants of Wire ic2 lib
#include <Wire.h>

#define DS1307_SEC 0
#define DS1307_MIN 1
#define DS1307_HR 2
#define DS1307_DOW 3
#define DS1307_DATE 4
#define DS1307_MTH 5
#define DS1307_YR 6

#define DS1307_BASE_YR 2000

#define DS1307_SQW1HZ  7
#define DS1307_SQW4KHZ 8
#define DS1307_SQW8KHZ 9
#define DS1307_SQW32KHZ 10

#define DS1307_CTRL_ID B1101000  //DS1307

 // Define register bit masks
#define DS1307_CLOCKHALT B10000000

#define DS1307_LO_BCD  B00001111
#define DS1307_HI_BCD  B11110000

#define DS1307_HI_SEC  B01110000
#define DS1307_HI_MIN  B01110000
#define DS1307_HI_HR   B00110000
#define DS1307_LO_DOW  B00000111
#define DS1307_HI_DATE B00110000
#define DS1307_HI_MTH  B00110000
#define DS1307_HI_YR   B11110000


// Prescaler
#define DS1307_LOW_BIT		B00000000
#define DS1307_HIGH_BIT		B10000000
#define DS1307_SQW1HZ_BIT	B00010000
#define DS1307_SQW4KHZ_BIT	B00010001
#define DS1307_SQW8KHZ_BIT	B00010010
#define DS1307_SQW32KHZ_BIT	B00010011

// library interface description
class DS1307
{
  // user-accessible "public" interface
  public:
    DS1307();
    void get(int *, boolean);
    int get(int, boolean);
	void set(int, int);
    void start(void);
    void stop(void);
    void SetOutput(int c);
	int GetOutput(void);
	
  // library-accessible "private" interface
  private:
    byte rtc_bcd[7]; // used prior to read/set ds1307 registers;
	void read(void);
	void save(void);
};

extern DS1307 RTC;

#endif

extern "C" {
#include <Wire.h>
}
DS1307::DS1307()
{
  Wire.begin();
}

DS1307 RTC=DS1307();

// PRIVATE FUNCTIONS

// Aquire data from the RTC chip in BCD format
// refresh the buffer
void DS1307::read(void)
{
  // use the Wire lib to connect to tho rtc
  // reset the resgiter pointer to zero
  Wire.beginTransmission(DS1307_CTRL_ID);
    Wire.write(0x00);
  Wire.endTransmission();

  // request the 7 bytes of data    (secs, min, hr, dow, date. mth, yr)
  Wire.requestFrom(DS1307_CTRL_ID, 7);
  for(int i=0; i<7; i++)
  {
    // store data in raw bcd format
    rtc_bcd[i]=Wire.read();
  }
}

// update the data on the IC from the bcd formatted data in the buffer
void DS1307::save(void)
{
  Wire.beginTransmission(DS1307_CTRL_ID);
  Wire.write(0x00); // reset register pointer
  for(int i=0; i<7; i++)
  {
    Wire.write(rtc_bcd[i]);
  }
  Wire.endTransmission();
}


// PUBLIC FUNCTIONS
void DS1307::get(int *rtc, boolean refresh)   // Aquire data from buffer and convert to int, refresh buffer if required
{
  if(refresh) read();
  for(int i=0;i<7;i++)  // cycle through each component, create array of data
  {
	rtc[i]=get(i, 0);
  }
}

int DS1307::get(int c, boolean refresh)  // aquire individual RTC item from buffer, return as int, refresh buffer if required
{
  if(refresh) read();
  int v=-1;
  switch(c)
  {
  case DS1307_SEC:
    v=(10*((rtc_bcd[DS1307_SEC] & DS1307_HI_SEC)>>4))+(rtc_bcd[DS1307_SEC] & DS1307_LO_BCD);
	break;
  case DS1307_MIN:
    v=(10*((rtc_bcd[DS1307_MIN] & DS1307_HI_MIN)>>4))+(rtc_bcd[DS1307_MIN] & DS1307_LO_BCD);
	break;
  case DS1307_HR:
    v=(10*((rtc_bcd[DS1307_HR] & DS1307_HI_HR)>>4))+(rtc_bcd[DS1307_HR] & DS1307_LO_BCD);
	break;
  case DS1307_DOW:
    v=rtc_bcd[DS1307_DOW] & DS1307_LO_DOW;
	break;
  case DS1307_DATE:
    v=(10*((rtc_bcd[DS1307_DATE] & DS1307_HI_DATE)>>4))+(rtc_bcd[DS1307_DATE] & DS1307_LO_BCD);
	break;
  case DS1307_MTH:
    v=(10*((rtc_bcd[DS1307_MTH] & DS1307_HI_MTH)>>4))+(rtc_bcd[DS1307_MTH] & DS1307_LO_BCD);
	break;
  case DS1307_YR:
    v=(10*((rtc_bcd[DS1307_YR] & DS1307_HI_YR)>>4))+(rtc_bcd[DS1307_YR] & DS1307_LO_BCD)+DS1307_BASE_YR;
	break;
  } // end switch
  return v;
}

void DS1307::set(int c, int v)  // Update buffer, then update the chip
{
  switch(c)
  {
  case DS1307_SEC:
    if(v<60 && v>-1)
    {
	//preserve existing clock state (running/stopped)
	int state=rtc_bcd[DS1307_SEC] & DS1307_CLOCKHALT;
	rtc_bcd[DS1307_SEC]=state | ((v / 10)<<4) + (v % 10);
    }
    break;
  case DS1307_MIN:
    if(v<60 && v>-1)
    {
	rtc_bcd[DS1307_MIN]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS1307_HR:
  // TODO : AM/PM  12HR/24HR
    if(v<24 && v>-1)
    {
	rtc_bcd[DS1307_HR]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS1307_DOW:
    if(v<8 && v>-1)
    {
	rtc_bcd[DS1307_DOW]=v;
    }
    break;
  case DS1307_DATE:
    if(v<31 && v>-1)
    {
	rtc_bcd[DS1307_DATE]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS1307_MTH:
    if(v<13 && v>-1)
    {
	rtc_bcd[DS1307_MTH]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS1307_YR:
    if(v<13 && v>-1)
    {
	rtc_bcd[DS1307_YR]=((v / 10)<<4) + (v % 10);
    }
    break;
  } // end switch
  save();
}

void DS1307::stop(void)
{
	// set the ClockHalt bit high to stop the rtc
	// this bit is part of the seconds byte
    rtc_bcd[DS1307_SEC]=rtc_bcd[DS1307_SEC] | DS1307_CLOCKHALT;
    save();
}

void DS1307::start(void)
{
	// unset the ClockHalt bit to start the rtc
	// TODO : preserve existing seconds
    rtc_bcd[DS1307_SEC]=rtc_bcd[DS1307_SEC] & (~DS1307_CLOCKHALT);
	save();
}


void DS1307::SetOutput(int c)  
{
  int out;
  switch(c)
  {
  case HIGH :
	out=DS1307_HIGH_BIT;
  break;
  case LOW :
	out=DS1307_LOW_BIT;
  break;
  case DS1307_SQW1HZ :
	out=DS1307_SQW1HZ_BIT;
  break;
  case DS1307_SQW4KHZ :
	out=DS1307_SQW4KHZ_BIT;
  break;
  case DS1307_SQW8KHZ :
	out=DS1307_SQW8KHZ_BIT;
  break;
  case DS1307_SQW32KHZ :
	out=DS1307_SQW32KHZ_BIT;
  break;
  default: 
	out=DS1307_LOW_BIT;
  }
  
  Wire.beginTransmission(DS1307_CTRL_ID);
  Wire.write(0x07); 
  Wire.write(out);
  Wire.endTransmission();
}

int DS1307::GetOutput(void)  
{
  Wire.beginTransmission(DS1307_CTRL_ID);
  Wire.write(0x07);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_CTRL_ID, 1);
  int out=Wire.read();
  
  int c=-1;
  switch(out)
  {
  case DS1307_HIGH_BIT :
	c=HIGH;
  break;
  case DS1307_LOW_BIT :
	c=LOW;
  break;
  case DS1307_SQW1HZ_BIT :
	c=DS1307_SQW1HZ;
  break;
  case DS1307_SQW4KHZ_BIT :
	c=DS1307_SQW4KHZ;
  break;
  case DS1307_SQW8KHZ_BIT :
	c=DS1307_SQW8KHZ;
  break;
  case DS1307_SQW32KHZ_BIT :
	c=DS1307_SQW32KHZ;
  break;
  }
  return c;
}


#include <Wire.h>

int rtc[7];
byte rr[7];
int ledPin =  13;
void setup()
{
  DDRC|=_BV(2) |_BV(3);  // POWER:Vcc Gnd
  PORTC |=_BV(3);  // VCC PINC3
  pinMode(ledPin, OUTPUT);  
  Serial.begin(9600);
  RTC.get(rtc,true);
  if(rtc[6]<12){
    RTC.stop();
    RTC.set(DS1307_SEC,1);
    RTC.set(DS1307_MIN,27);
    RTC.set(DS1307_HR,01);
    RTC.set(DS1307_DOW,7);
    RTC.set(DS1307_DATE,12);
    RTC.set(DS1307_MTH,2);
    RTC.set(DS1307_YR,12);
    RTC.start();
  }
  //RTC.SetOutput(LOW);
  //RTC.SetOutput(HIGH);
  //RTC.SetOutput(DS1307_SQW1HZ);
  //RTC.SetOutput(DS1307_SQW4KHZ);
  //RTC.SetOutput(DS1307_SQW8KHZ);
  RTC.SetOutput(DS1307_SQW32KHZ);
}

void loop()
{
  int i;
  RTC.get(rtc,true);

  for(i=0; i<7; i++)
  {
    Serial.print(rtc[i]);
    Serial.print(" ");
  }
  Serial.println();
	digitalWrite(ledPin, HIGH); 
	delay(500);
	digitalWrite(ledPin, LOW);
	delay(500);
 if (Serial.available() > 6) {
     for(i=0;i<7;i++){
       rr[i]=BCD2DEC(Serial.read());
     }
     Serial.println("SET TIME:");
       RTC.stop();
    RTC.set(DS1307_SEC,rr[6]);
    RTC.set(DS1307_MIN,rr[5]);
    RTC.set(DS1307_HR,rr[4]);
    RTC.set(DS1307_DOW,rr[3]);
    RTC.set(DS1307_DATE,rr[2]);
    RTC.set(DS1307_MTH,rr[1]);
    RTC.set(DS1307_YR,rr[0]);
    RTC.start();
 }
}
char BCD2DEC(char var){
  if (var>9){
     var=(var>>4)*10+(var&0x0f);
  }
  return var;
}
