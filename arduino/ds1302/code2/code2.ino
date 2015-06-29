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

const int ce_pin   = 4;//5;  // Chip Enable
const int io_pin   = 3;//6;  // Input/Output
const int sclk_pin = 2;//7;  // Serial Clock

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

void writeOut(const uint8_t value) {
	pinMode(io_pin, OUTPUT);
	shiftOuta(io_pin, sclk_pin, LSBFIRST, value);
}

void shiftOuta(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST)
			digitalWrite(dataPin, !!(val & (1 << i)));
		else	
			digitalWrite(dataPin, !!(val & (1 << (7 - i))));
			
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}

uint8_t readIn() {
	uint8_t input_value = 0;
	uint8_t bit = 0;
	pinMode(io_pin, INPUT);

	// Bits from the DS1302 are output on the falling edge of the clock
	// cycle. This method is called after a previous call to writeOut() or
	// readIn(), which will have already set the clock low.
	for (int i = 0; i < 8; ++i) {
		bit = digitalRead(io_pin);
		input_value |= (bit << i);  // Bits are read LSB first.

		// See the note in writeOut() about timing. digitalWrite() is slow enough to
		// not require extra delays for tCH and tCL.
		digitalWrite(sclk_pin, HIGH);
		digitalWrite(sclk_pin, LOW);
	}

	return input_value;
}

void GetTime(Time* t) {
	pinMode(ce_pin, OUTPUT);
	pinMode(sclk_pin, OUTPUT);

	digitalWrite(sclk_pin, LOW);
		digitalWrite(ce_pin, HIGH);
		delayMicroseconds(4);  // tCC

	writeOut(kClockBurstRead);
	t->sec = bcdToDec(readIn());
	t->min = bcdToDec(readIn());
	t->hr = bcdToDec(readIn());
	t->date = bcdToDec(readIn());
	t->mon = bcdToDec(readIn());
	t->day = readIn();
	t->yr = bcdToDec(readIn());
		digitalWrite(ce_pin, LOW);
		delayMicroseconds(4);  // tCWH
}

void SetTime(Time* t) {
	pinMode(ce_pin, OUTPUT);
	pinMode(sclk_pin, OUTPUT);

	digitalWrite(sclk_pin, LOW);
		digitalWrite(ce_pin, HIGH);
		delayMicroseconds(4);  // tCC

	writeOut(kClockBurstWrite);
	writeOut(decToBcd(t->sec));
	writeOut(decToBcd(t->min));
	writeOut(decToBcd(t->hr));
	writeOut(decToBcd(t->date));
	writeOut(decToBcd(t->mon));
	writeOut(t->day);
	writeOut(decToBcd(t->yr));
	// All clock registers *and* the WP register have to be written for the time
	// to be set.
	writeOut(0);  // Write protection register.
		digitalWrite(ce_pin, LOW);
		delayMicroseconds(4);  // tCWH
}


void setup() {
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



