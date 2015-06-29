DS1302:00 2050-120-28 08:18:30

DS1302:00 2050-120-28 08:18:08

DS1302:00 2050-120-28 08:18:88

DS1302:00 2050-120-28 08:18:88

DS1302:00 2050-120-28 08:18:48

DS1302:00 2050-120-28 08:18:48

DS1302:00 2050-120-28 08:18:128

DS1302:00 2050-120-28 08:18:128

DS1302:00 2050-120-28 08:18:28

DS1302:00 2050-120-28 08:18:28

DS1302:00 2050-120-28 08:18:18

DS1302:00 2050-120-28 08:18:18

DS1302:00 2050-120-28 08:18:98

DS1302:00 2050-120-28 08:18:98

DS1302:00 2050-120-28 08:18:58



struct Time {
	uint8_t sec;
	uint8_t min;
	uint8_t hr;
	uint8_t date;
	uint8_t mon;
	uint8_t day;
	uint8_t yr;
};

void GetTime(Time* t);
void SetTime(Time* t);

//const int ce_pin   = 4;//5;  // Chip Enable
#define DDR_CE DDRD
#define PORT_CE PORTD
#define BIT_CE _BV(4)

//const int io_pin   = 3;//6;  // Input/Output
#define DDR_IO DDRD
#define PORT_IO PORTD
#define PIN_IO PIND
#define BIT_IO _BV(3)

//const int sclk_pin = 2;//7;  // Serial Clock
#define DDR_CLK DDRD
#define PORT_CLK PORTD
#define BIT_CLK _BV(2)

#define kClockBurstRead  0xBF
#define kClockBurstWrite 0xBE

uint8_t bcdToDec(const uint8_t bcd) {
	return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}

uint8_t decToBcd(const uint8_t dec) {
	const uint8_t tens = dec / 10;
	const uint8_t ones = dec % 10;
	return (tens << 4) | ones;
}

volatile int temp;
void wait()
{
	for(int i=0;i<100;i++)
	{
		temp++;
	}
}

void writeOut(const uint8_t value) {
	DDR_IO |= BIT_IO;//pinMode(io_pin, OUTPUT);
	wait();
	uint8_t val = value;
	for (uint8_t i = 0; i < 8; i++)  {
		if(val & 1)//digitalWrite(io_pin, !!(value & (1 << i)));
		{
			PORT_IO |= BIT_IO;
		}
		else
		{
			PORT_IO &= ~BIT_IO;
		}
		val = val>>1;
		wait();
		PORT_CLK |= BIT_CLK;//digitalWrite(sclk_pin, HIGH);
		wait();
		PORT_CLK &= ~BIT_CLK;//digitalWrite(sclk_pin, LOW);
		wait();
	}
	DDR_IO &= ~BIT_IO;
	wait();
}

uint8_t readIn() {
	uint8_t input_value = 0;
	wait();
	for (uint8_t i = 0; i < 8; i++) {
		if(PIN_IO & BIT_IO)//digitalWrite(io_pin, !!(value & (1 << i)));
		{
			input_value |= 1;  // Bits are read LSB first.
		}
		else
		{
			input_value |= 0;  // Bits are read LSB first.
		}
		input_value = input_value << 1;
		wait();
		PORT_CLK |= BIT_CLK;//digitalWrite(sclk_pin, HIGH);
		wait();
		PORT_CLK &= ~BIT_CLK;//digitalWrite(sclk_pin, LOW);
		wait();
	}
	return input_value;
}

void GetTime(Time* t) {
	DDR_CLK |= BIT_CLK;//pinMode(sclk_pin, OUTPUT);

	PORT_CLK &= ~BIT_CLK;//digitalWrite(sclk_pin, LOW);
	PORT_CE |= BIT_CE;//digitalWrite(ce_pin, HIGH);
	wait();//	delayMicroseconds(4);  // tCC

	writeOut(kClockBurstRead);
	t->sec = bcdToDec(readIn());
	t->min = bcdToDec(readIn());
	t->hr = bcdToDec(readIn());
	t->date = bcdToDec(readIn());
	t->mon = bcdToDec(readIn());
	t->day = readIn();
	t->yr = bcdToDec(readIn());
	PORT_CE &= ~BIT_CE;//digitalWrite(ce_pin, LOW);
	wait();
	
	DDR_CLK &= ~BIT_CLK;
}

void SetTime(Time* t) {
	DDR_CLK |= BIT_CLK;//pinMode(sclk_pin, OUTPUT);

	PORT_CLK &= ~BIT_CLK;//digitalWrite(sclk_pin, LOW);
	PORT_CE |= BIT_CE;//digitalWrite(ce_pin, HIGH);
	wait();//	delayMicroseconds(4);  // tCC

	writeOut(kClockBurstWrite);
	writeOut(decToBcd(t->sec));
	writeOut(decToBcd(t->min));
	writeOut(decToBcd(t->hr));
	writeOut(decToBcd(t->date));
	writeOut(decToBcd(t->mon));
	writeOut(t->day);
	writeOut(decToBcd(t->yr));
	writeOut(0);  // Write protection register.
	PORT_CE &= ~BIT_CE;//digitalWrite(ce_pin, LOW);
	wait();//	delayMicroseconds(4);  // tCC
}


void setup() {
	DDR_CE |= BIT_CE;
	PORT_CE &= ~BIT_CE;
	delay(5000);
	Serial.begin(9600);
	Serial.setTimeout(1000);
	pinMode(13, OUTPUT);
}

void loop() {
	digitalWrite(13, HIGH);
	Time t;

	GetTime(&t);

	char buf[60];
	snprintf(buf, sizeof(buf), "DS1302:%02d 20%02d-%02d-%02d %02d:%02d:%02d",t.day, t.yr, t.mon, t.date, t.hr, t.min, t.sec);

	// Print the formatted string to serial so we can see the time.
	Serial.println(buf);

	//delay(1000);

	digitalWrite(13, LOW);
	int start = Serial.parseInt();
	if(start == 33)
	{
		t.yr  = Serial.parseInt();
		t.mon = Serial.parseInt();
		t.date = Serial.parseInt();
		t.hr  = Serial.parseInt();
		t.min = Serial.parseInt();
		t.sec = Serial.parseInt();
		t.day= Serial.parseInt();


		SetTime(&t);
	}
}
//33,15,6,26,20,39,00,5,



