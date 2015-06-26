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


enum Command {
	kClockBurstRead  = 0xBF,
	kClockBurstWrite = 0xBE
};

  // Establishes and terminates a three-wire SPI session.
class SPISession {
	public:
	SPISession()
	{
		digitalWrite(sclk_pin, LOW);
		digitalWrite(ce_pin, HIGH);
		delayMicroseconds(4);  // tCC
	}

	~SPISession() {
		digitalWrite(ce_pin, LOW);
		delayMicroseconds(4);  // tCWH
	}
};

uint8_t bcdToDec(const uint8_t bcd) {
	return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}

uint8_t decToBcd(const uint8_t dec) {
	const uint8_t tens = dec / 10;
	const uint8_t ones = dec % 10;
	return (tens << 4) | ones;
}

void DS1302Init() {
	pinMode(ce_pin, OUTPUT);
	pinMode(sclk_pin, OUTPUT);
}

void writeOut(const uint8_t value) {
	pinMode(io_pin, OUTPUT);
	// This assumes that shiftOut is 'slow' enough for the DS1302 to read the
	// bits. The datasheet specifies that SCLK must be in its high and low states
	// for at least 0.25us at 5V or 1us at 2V. Experimentally, a 16MHz Arduino
	// seems to spend ~4us high and ~12us low when shifting.
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
	const SPISession s;

	writeOut(kClockBurstRead);
	t->sec = bcdToDec(readIn());
	t->min = bcdToDec(readIn());
	t->hr = bcdToDec(readIn());
	t->date = bcdToDec(readIn());
	t->mon = bcdToDec(readIn());
	t->day = bcdToDec(readIn());
	t->yr = bcdToDec(readIn());
}

void SetTime(Time* t) {
	const SPISession s;

	writeOut(kClockBurstWrite);
	writeOut(decToBcd(t->sec));
	writeOut(decToBcd(t->min));
	writeOut(decToBcd(t->hr));
	writeOut(decToBcd(t->date));
	writeOut(decToBcd(t->mon));
	writeOut(decToBcd(t->day));
	writeOut(decToBcd(t->yr));
	// All clock registers *and* the WP register have to be written for the time
	// to be set.
	writeOut(0);  // Write protection register.
}


void printTime() {
	// Get the current time and date from the chip.
	Time t;

	GetTime(&t);

	// Format the time and date and insert into the temporary buffer.
	char buf[50];
	snprintf(buf, sizeof(buf), "%02d 20%02d-%02d-%02d %02d:%02d:%02d",t.day, t.yr, t.mon, t.date, t.hr, t.min, t.sec);

	// Print the formatted string to serial so we can see the time.
	Serial.println(buf);
}

void setup() {
	Serial.begin(9600);
	Serial.setTimeout(1000);
	DS1302Init();
}

void loop() {
	printTime();
	//delay(1000);
	uint8_t Time_sec;
	uint8_t Time_min;
	uint8_t Time_hr;
	uint8_t Time_day;
	uint8_t Time_mon;
	uint8_t Time_yr;
	uint8_t Time_week;
	int start = Serial.parseInt();
	if(start == 33)
	{
		//pinMode(13, OUTPUT);
		//digitalWrite(13, HIGH);
		Time_yr  = Serial.parseInt();
		Time_mon = Serial.parseInt();
		Time_day = Serial.parseInt();
		Time_hr  = Serial.parseInt();
		Time_min = Serial.parseInt();
		Time_sec = Serial.parseInt();
		Time_week= Serial.parseInt();

		Time t;
		t.sec = Time_sec;
		t.min = Time_min;
		t.hr  = Time_hr;
		t.date= Time_day;
		t.mon = Time_mon;
		t.yr  = Time_yr;

		SetTime(&t);
	}
}



