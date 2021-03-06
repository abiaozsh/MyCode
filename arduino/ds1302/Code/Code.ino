/////////////////////////////////////////////////////////////////////////////////////////
#define DDR_sclk DDRD
#define PORT_sclk PORTD
#define BIT_sclk _BV(2)

#define DDR_io DDRD
#define PIN_io PIND
#define PORT_io PORTD
#define BIT_io _BV(3)

#define DDR_ce DDRD
#define PORT_ce PORTD
#define BIT_ce _BV(4)


uint8_t Time_sec;
uint8_t Time_min;
uint8_t Time_hr;
uint8_t Time_day;
uint8_t Time_mon;
uint8_t Time_yr;
uint8_t Time_week;
/////////////////////////////////////////////////////////////////////////////////////////

uint8_t IsInit = 1;

void setup()
{
	Serial.begin(9600);
	Serial.setTimeout(1000);
	PortInit();
}

void loop()
{
	ReadTime();
	Serial.print(Time_yr);  Serial.print(" ");
	Serial.print(Time_mon); Serial.print(" ");
	Serial.print(Time_day); Serial.print(" ");
	Serial.print(Time_hr);  Serial.print(" ");
	Serial.print(Time_min); Serial.print(" ");
	Serial.print(Time_sec); Serial.print(" ");
	Serial.println();

	Serial.print((Time_yr & 0xF0)>>4);
	Serial.print((Time_yr & 0x0F));
	Serial.print("/");
	Serial.print((Time_mon & 0xF0)>>4);
	Serial.print((Time_mon & 0x0F));
	Serial.print("/");
	Serial.print((Time_day & 0xF0)>>4);
	Serial.print((Time_day & 0x0F));
	Serial.print(" ");
	Serial.print((Time_hr & 0xF0)>>4);
	Serial.print((Time_hr & 0x0F));
	Serial.print(":");
	Serial.print((Time_min & 0xF0)>>4);
	Serial.print((Time_min & 0x0F));
	Serial.print(":");
	Serial.print((Time_sec & 0xF0)>>4);
	Serial.print((Time_sec & 0x0F));
	Serial.println();

	int start = Serial.parseInt();
	if(start == 33)
	{
		Time_sec = Serial.parseInt();
		Time_min = Serial.parseInt();
		Time_hr  = Serial.parseInt();
		Time_day = Serial.parseInt();
		Time_mon = Serial.parseInt();
		Time_yr  = Serial.parseInt();
		Time_week= Serial.parseInt();
	}
}

void PortInit()
{
  DDR_ce |= BIT_ce;
  DDR_sclk |= BIT_sclk;
}


void writeOut(uint8_t value) {
  DDR_io |= BIT_io;
  uint8_t val = value;
  PORT_sclk &= ~BIT_sclk;
  wait();
  for (uint8_t i = 0; i < 8; i++)  {
    if(val & (1 << i))
    {
      PORT_io |= BIT_io;
    }
    else
    {
      PORT_io &= ~BIT_io;
    }
    wait();
    PORT_sclk |= BIT_sclk;
    wait();
    PORT_sclk &= ~BIT_sclk;
  }
  DDR_io &= ~BIT_io;
}

uint8_t readIn() {
  uint8_t input_value = 0;
  uint8_t bit = 0;
  PORT_sclk &= ~BIT_sclk;
  wait();
  for (int i = 0; i < 8; ++i) {
    if(PIN_io & BIT_io)
    {
      bit = 1;
    }
    else
    {
      bit = 0;
    }
    input_value |= (bit << i);
    wait();
    PORT_sclk |= BIT_sclk;
    wait();
    PORT_sclk &= ~BIT_sclk;
  }

  return input_value;
}

#define kClockBurstRead 0xBF
void ReadTime() {
  PORT_sclk &= ~BIT_sclk;
  PORT_ce |= BIT_ce;
  wait();
  writeOut(kClockBurstRead);
  Time_sec = readIn();
  Time_min = readIn();
  Time_hr = readIn();
  Time_week = readIn();
  Time_mon = readIn();
  Time_day = readIn();
  Time_yr = readIn();
  PORT_ce &= ~BIT_ce;
  wait();
}

#define kClockBurstWrite 0xBE
void WriteTime() {
  PORT_sclk &= ~BIT_sclk;
  PORT_ce |= BIT_ce;
  wait();
  writeOut(kClockBurstWrite);
  writeOut(Time_sec);
  writeOut(Time_min);
  writeOut(Time_hr);
  writeOut(Time_week);
  writeOut(Time_mon);
  writeOut(Time_day);
  writeOut(Time_yr);
  PORT_ce &= ~BIT_ce;
  wait();
}
void wait()
{
  for(int i=0;i<10;i++)
  {
    volatile int t=0;
    t++;
  }
}

