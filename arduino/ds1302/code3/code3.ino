
// Class representing a particular time and date.
struct Time {
  uint8_t sec;
  uint8_t min;
  uint8_t hr;
  uint8_t date;
  uint8_t mon;
  uint8_t day;
  uint8_t yr;
};

// An interface to the Dallas Semiconductor DS1302 Real Time Clock (RTC) chip.
//
// Accessing and setting individual components of the time are not supported in
// this interface as doing so can lead to errors if the time changes as it is
// being read or modified. Instead, using DS1302::time() guarantees safe reads
// and writes using the DS1302's burst mode feature.
class DS1302 {
public:

  // Prepares to interface with the chip on the given I/O pins.
  //
  // Args:
  //   ce_pin: CE pin number
  //   io_pin: IO pin number
  //   sclk_pin: SCLK pin number
  DS1302(uint8_t ce_pin, uint8_t io_pin, uint8_t sclk_pin);
  void writeProtect(bool enable);
  void halt(bool value);

  // Returns the current time and date in a Time object.
  //
  // Returns:
  //   Current time as Time object.
  void getTime(Time* t);

  // Sets the time and date to the instant specified in a given Time object.
  //
  // The time will not be set if write protection is enabled on the
  // chip. Setting the time with this function has no effect on the Clock Halt
  // flag.
  //
  // Args:
  //   t: Time instant.
  void setTime(Time* t);

  // Reads register byte value.
  //
  // Args:
  //   reg: register number
  //
  // Returns:
  //   register value
  uint8_t readRegister(uint8_t reg);

  // Writes byte into register.
  //
  // Args:
  //   reg: register number
  //   value: byte to write
  void writeRegister(uint8_t reg, uint8_t value);

private:
  uint8_t ce_pin_;
  uint8_t io_pin_;
  uint8_t sclk_pin_;

  // Shifts out a value to the IO pin.
  //
  // Side effects: sets io_pin_ as OUTPUT.
  //
  // Args:
  //   value: byte to shift out
  void writeOut(uint8_t value);

  // Reads in a byte from the IO pin.
  //
  // Side effects: sets io_pin_ to INPUT.
  //
  // Returns:
  //   byte read in
  uint8_t readIn();
};




namespace {

  enum Register {
    kSecondReg       = 0,
    kMinuteReg       = 1,
    kHourReg         = 2,
    kDateReg         = 3,
    kMonthReg        = 4,
    kDayReg          = 5,
    kYearReg         = 6,
    kWriteProtectReg = 7,

    // The RAM register space follows the clock register space.
    kRamAddress0     = 32
  };

  enum Command {
    kClockBurstRead  = 0xBF,
    kClockBurstWrite = 0xBE,
    kRamBurstRead    = 0xFF,
    kRamBurstWrite   = 0xFE
  };

  // Establishes and terminates a three-wire SPI session.
  class SPISession {
public:
    SPISession(const int ce_pin, const int io_pin, const int sclk_pin)
: 
      ce_pin_(ce_pin), io_pin_(io_pin), sclk_pin_(sclk_pin) {
        digitalWrite(sclk_pin_, LOW);
        digitalWrite(ce_pin_, HIGH);
        delayMicroseconds(4);  // tCC
      }
    ~SPISession() {
      digitalWrite(ce_pin_, LOW);
      delayMicroseconds(4);  // tCWH
    }

private:
    const int ce_pin_;
    const int io_pin_;
    const int sclk_pin_;
  };

  // Returns the decoded decimal value from a binary-coded decimal (BCD) byte.
  // Assumes 'bcd' is coded with 4-bits per digit, with the tens place digit in
  // the upper 4 MSBs.
  uint8_t bcdToDec(const uint8_t bcd) {
    return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
  }

  // Returns the binary-coded decimal of 'dec'. Inverse of bcdToDec.
  uint8_t decToBcd(const uint8_t dec) {
    const uint8_t tens = dec / 10;
    const uint8_t ones = dec % 10;
    return (tens << 4) | ones;
  }

  // Returns the hour in 24-hour format from the hour register value.
  uint8_t hourFromRegisterValue(const uint8_t value) {
    uint8_t adj;
    if (value & 128)  // 12-hour mode
      adj = 12 * ((value & 32) >> 5);
    else           // 24-hour mode
    adj = 10 * ((value & (32 + 16)) >> 4);
    return (value & 15) + adj;
  }

}  // namespace

DS1302::DS1302(const uint8_t ce_pin, const uint8_t io_pin,
const uint8_t sclk_pin) {
  ce_pin_ = ce_pin;
  io_pin_ = io_pin;
  sclk_pin_ = sclk_pin;

  pinMode(ce_pin, OUTPUT);
  pinMode(sclk_pin, OUTPUT);
}

void DS1302::writeOut(const uint8_t value) {
  pinMode(io_pin_, OUTPUT);
  // This assumes that shiftOut is 'slow' enough for the DS1302 to read the
  // bits. The datasheet specifies that SCLK must be in its high and low states
  // for at least 0.25us at 5V or 1us at 2V. Experimentally, a 16MHz Arduino
  // seems to spend ~4us high and ~12us low when shifting.
  shiftOut(io_pin_, sclk_pin_, LSBFIRST, value);
}

uint8_t DS1302::readIn() {
  uint8_t input_value = 0;
  uint8_t bit = 0;
  pinMode(io_pin_, INPUT);

  // Bits from the DS1302 are output on the falling edge of the clock
  // cycle. This method is called after a previous call to writeOut() or
  // readIn(), which will have already set the clock low.
  for (int i = 0; i < 8; ++i) {
    bit = digitalRead(io_pin_);
    input_value |= (bit << i);  // Bits are read LSB first.

    // See the note in writeOut() about timing. digitalWrite() is slow enough to
    // not require extra delays for tCH and tCL.
    digitalWrite(sclk_pin_, HIGH);
    digitalWrite(sclk_pin_, LOW);
  }

  return input_value;
}

uint8_t DS1302::readRegister(const uint8_t reg) {
  const SPISession s(ce_pin_, io_pin_, sclk_pin_);

  const uint8_t cmd_byte = (0x81 | (reg << 1));
  writeOut(cmd_byte);
  return readIn();
}

void DS1302::writeRegister(const uint8_t reg, const uint8_t value) {
  const SPISession s(ce_pin_, io_pin_, sclk_pin_);

  const uint8_t cmd_byte = (0x80 | (reg << 1));
  writeOut(cmd_byte);
  writeOut(value);
}

void DS1302::writeProtect(const bool enable) {
  writeRegister(kWriteProtectReg, (enable << 7));
}

void DS1302::halt(const bool enable) {
  uint8_t sec = readRegister(kSecondReg);
  sec &= ~(1 << 7);
  sec |= (enable << 7);
  writeRegister(kSecondReg, sec);
}

void DS1302::getTime(Time* t) {
  const SPISession s(ce_pin_, io_pin_, sclk_pin_);

  writeOut(kClockBurstRead);
  t->sec = bcdToDec(readIn() & 0x7F);
  t->min = bcdToDec(readIn());
  t->hr = hourFromRegisterValue(readIn());
  t->date = bcdToDec(readIn());
  t->mon = bcdToDec(readIn());
  t->day = bcdToDec(readIn());
  t->yr = bcdToDec(readIn());
}

void DS1302::setTime(Time* t) {
  // We want to maintain the Clock Halt flag if it is set.
  const uint8_t ch_value = readRegister(kSecondReg) & 0x80;

  const SPISession s(ce_pin_, io_pin_, sclk_pin_);

  writeOut(kClockBurstWrite);
  writeOut(ch_value | decToBcd(t->sec));
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


// Set the appropriate digital I/O pin connections. These are the pin
// assignments for the Arduino as well for as the DS1302 chip. See the DS1302
// datasheet:
//
//   http://datasheets.maximintegrated.com/en/ds/DS1302.pdf
const int kCePin   = 4;//5;  // Chip Enable
const int kIoPin   = 3;//6;  // Input/Output
const int kSclkPin = 2;//7;  // Serial Clock

// Create a DS1302 object.
DS1302 rtc(kCePin, kIoPin, kSclkPin);

void printTime() {
	// Get the current time and date from the chip.
	Time t;

	rtc.getTime(&t);

	// Format the time and date and insert into the temporary buffer.
	char buf[50];
	snprintf(buf, sizeof(buf), "%02d 20%02d-%02d-%02d %02d:%02d:%02d",t.day, t.yr, t.mon, t.date, t.hr, t.min, t.sec);

	// Print the formatted string to serial so we can see the time.
	Serial.println(buf);
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1000);
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
		rtc.writeProtect(false);
		rtc.halt(false);
		Time_sec = Serial.parseInt();
		Time_min = Serial.parseInt();
		Time_hr  = Serial.parseInt();
		Time_day = Serial.parseInt();
		Time_mon = Serial.parseInt();
		Time_yr  = Serial.parseInt();
		Time_week= Serial.parseInt();

		Time t;
		t.sec = Time_sec;
		t.min = Time_min;
		t.hr  = Time_hr;
		t.date= Time_day;
		t.mon = Time_mon;
		t.yr  = Time_yr;

		rtc.setTime(&t);
	}
}



