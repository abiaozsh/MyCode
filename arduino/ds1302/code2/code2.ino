// Interface for the DS1302 timekeeping chip.
//
// Copyright (c) 2009, Matt Sparks
// All rights reserved.
//
// Distributed under the 2-clause BSD license.
#ifndef DS1302_H_
#define DS1302_H_

#include <stdint.h>

// Class representing a particular time and date.
class Time {
public:
  enum Day {
    kSunday    = 1,
    kMonday    = 2,
    kTuesday   = 3,
    kWednesday = 4,
    kThursday  = 5,
    kFriday    = 6,
    kSaturday  = 7
  };

  // Creates a Time object with a given time.
  //
  // Args:
  //   yr: year. Range: {2000, ..., 2099}.
  //   mon: month. Range: {1, ..., 12}.
  //   date: date (of the month). Range: {1, ..., 31}.
  //   hr: hour. Range: {0, ..., 23}.
  //   min: minutes. Range: {0, ..., 59}.
  //   sec: seconds. Range: {0, ..., 59}.
  //   day: day of the week. Sunday is 1. Range: {1, ..., 7}.
  Time(uint16_t yr, uint8_t mon, uint8_t date,
  uint8_t hr, uint8_t min, uint8_t sec,
  Day day);

  uint8_t sec;
  uint8_t min;
  uint8_t hr;
  uint8_t date;
  uint8_t mon;
  Day day;
  uint16_t yr;
};

// An interface to the Dallas Semiconductor DS1302 Real Time Clock (RTC) chip.
//
// Accessing and setting individual components of the time are not supported in
// this interface as doing so can lead to errors if the time changes as it is
// being read or modified. Instead, using DS1302::time() guarantees safe reads
// and writes using the DS1302's burst mode feature.
class DS1302 {
public:
  // Size of the DS1302's RAM storage, in bytes.
  static const int kRamSize = 31;

  // Prepares to interface with the chip on the given I/O pins.
  //
  // Args:
  //   ce_pin: CE pin number
  //   io_pin: IO pin number
  //   sclk_pin: SCLK pin number
  DS1302(uint8_t ce_pin, uint8_t io_pin, uint8_t sclk_pin);

  // Enables or disables write protection on the chip.
  //
  // While write protection is enabled, all attempts to write to the chip (e.g.,
  // setting the time) will have no effect.
  //
  // The DS1302 datasheet does not define the initial state of write protection,
  // so this method should be called at least once when initializing a device
  // for the first time.
  //
  // Args:
  //   enable: true to enable write protection.
  void writeProtect(bool enable);

  // Sets or clears Clock Halt flag on the chip.
  //
  // Enabling the Clock Halt flag disables the DS1302's clock oscillator and
  // places it into a low-power standby mode. While in this mode, the time does
  // not progress. The time can still be read from the chip while it is halted,
  // however.
  //
  // The DS1302 datasheet does not define the initial state of the Clock Halt
  // flag, so this method should be called at least once when initializing a
  // device for the first time.
  //
  // Args:
  //   value: true to set halt flag, false to clear.
  void halt(bool value);

  // Returns the current time and date in a Time object.
  //
  // Returns:
  //   Current time as Time object.
  Time time();

  // Sets the time and date to the instant specified in a given Time object.
  //
  // The time will not be set if write protection is enabled on the
  // chip. Setting the time with this function has no effect on the Clock Halt
  // flag.
  //
  // Args:
  //   t: Time instant.
  void time(Time t);

  // Writes a byte to RAM.
  //
  // The DS1302 has 31 bytes (kRamSize) of static RAM that can store arbitrary
  // data as long as the device has power.
  //
  // Writes to invalid addresses have no effect.
  //
  // Args:
  //   address: RAM address in [0, kRamSize).
  //   value: Byte to write to the RAM address.
  void writeRam(uint8_t address, uint8_t value);

  // Reads a byte from RAM.
  //
  // Reads of invalid addresses return 0.
  //
  // Args:
  //   address: RAM address in [0, kRamSize).
  //
  // Returns:
  //   Byte from RAM or 0 if the address is invalid.
  uint8_t readRam(uint8_t address);

  // Writes 'len' bytes into RAM from '*data', starting at RAM address 0.
  //
  // Args:
  //   data: Input data.
  //   len: Number of bytes of '*data' to read. Must be <= kRamSize.
  void writeRamBulk(const uint8_t* data, int len);

  // Reads 'len' bytes from RAM into '*data', starting at RAM address 0.
  //
  // Args:
  //   data: Output data.
  //   len: Number of bytes of RAM to read. Must be <= kRamSize.
  void readRamBulk(uint8_t* data, int len);

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

#endif  // DS1302_H_




#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <stdint.h>

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

Time::Time(const uint16_t yr, const uint8_t mon, const uint8_t date,
const uint8_t hr, const uint8_t min, const uint8_t sec,
const Day day) {
  this->yr   = yr;
  this->mon  = mon;
  this->date = date;
  this->hr   = hr;
  this->min  = min;
  this->sec  = sec;
  this->day  = day;
}

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

Time DS1302::time() {
  const SPISession s(ce_pin_, io_pin_, sclk_pin_);

  Time t(2099, 1, 1, 0, 0, 0, Time::kSunday);
  writeOut(kClockBurstRead);
  t.sec = bcdToDec(readIn() & 0x7F);
  t.min = bcdToDec(readIn());
  t.hr = hourFromRegisterValue(readIn());
  t.date = bcdToDec(readIn());
  t.mon = bcdToDec(readIn());
  t.day = static_cast<Time::Day>(bcdToDec(readIn()));
  t.yr = 2000 + bcdToDec(readIn());
  return t;
}

void DS1302::time(const Time t) {
  // We want to maintain the Clock Halt flag if it is set.
  const uint8_t ch_value = readRegister(kSecondReg) & 0x80;

  const SPISession s(ce_pin_, io_pin_, sclk_pin_);

  writeOut(kClockBurstWrite);
  writeOut(ch_value | decToBcd(t.sec));
  writeOut(decToBcd(t.min));
  writeOut(decToBcd(t.hr));
  writeOut(decToBcd(t.date));
  writeOut(decToBcd(t.mon));
  writeOut(decToBcd(static_cast<uint8_t>(t.day)));
  writeOut(decToBcd(t.yr - 2000));
  // All clock registers *and* the WP register have to be written for the time
  // to be set.
  writeOut(0);  // Write protection register.
}

void DS1302::writeRam(const uint8_t address, const uint8_t value) {
  if (address >= kRamSize) {
    return;
  }

  writeRegister(kRamAddress0 + address, value);
}

uint8_t DS1302::readRam(const uint8_t address) {
  if (address >= kRamSize) {
    return 0;
  }

  return readRegister(kRamAddress0 + address);
}

void DS1302::writeRamBulk(const uint8_t* const data, int len) {
  if (len <= 0) {
    return;
  }
  if (len > kRamSize) {
    len = kRamSize;
  }

  const SPISession s(ce_pin_, io_pin_, sclk_pin_);

  writeOut(kRamBurstWrite);
  for (int i = 0; i < len; ++i) {
    writeOut(data[i]);
  }
}

void DS1302::readRamBulk(uint8_t* const data, int len) {
  if (len <= 0) {
    return;
  }
  if (len > kRamSize) {
    len = kRamSize;
  }

  const SPISession s(ce_pin_, io_pin_, sclk_pin_);

  writeOut(kRamBurstRead);
  for (int i = 0; i < len; ++i) {
    data[i] = readIn();
  }
}


// Example sketch for interfacing with the DS1302 timekeeping chip.
//
// Copyright (c) 2009, Matt Sparks
// All rights reserved.
//
// http://quadpoint.org/projects/arduino-ds1302
#include <stdio.h>
#include <DS1302.h>

namespace {

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

  String dayAsString(const Time::Day day) {
    switch (day) {
      case Time::kSunday: 
      return "Sunday";
      case Time::kMonday: 
      return "Monday";
      case Time::kTuesday: 
      return "Tuesday";
      case Time::kWednesday: 
      return "Wednesday";
      case Time::kThursday: 
      return "Thursday";
      case Time::kFriday: 
      return "Friday";
      case Time::kSaturday: 
      return "Saturday";
    }
    return "(unknown day)";
  }

  void printTime() {
    // Get the current time and date from the chip.
    Time t = rtc.time();

    // Name the day of the week.
    const String day = dayAsString(t.day);

    // Format the time and date and insert into the temporary buffer.
    char buf[50];
    snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
    day.c_str(),
    t.yr, t.mon, t.date,
    t.hr, t.min, t.sec);

    // Print the formatted string to serial so we can see the time.
    Serial.println(buf);
  }

}  // namespace

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1000);
  // Initialize a new chip by turning off write protection and clearing the
  // clock halt flag. These methods needn't always be called. See the DS1302
  // datasheet for details.
  rtc.writeProtect(false);
  rtc.halt(false);

  // Make a new time object to set the date and time.
  // Sunday, September 22, 2013 at 01:38:50.


  // Set the time and date on the chip.
}

// Loop and print the time every second.
void loop() {
  printTime();
  //delay(1000);
  uint8_t Time_sec;
  uint8_t Time_min;
  uint8_t Time_hr;
  uint8_t Time_day;
  uint8_t Time_mon;
  uint16_t Time_yr;
  uint8_t Time_week;
  int start = Serial.parseInt();
  if(start == 33)
  {
    //pinMode(13, OUTPUT);
    //digitalWrite(13, HIGH);
    Time_sec = Serial.parseInt();
    Time_min = Serial.parseInt();
    Time_hr  = Serial.parseInt();
    Time_day = Serial.parseInt();
    Time_mon = Serial.parseInt();
    Time_yr  = Serial.parseInt();
    Time_week= Serial.parseInt();
    //Time t(Time_yr, Time_mon, Time_day, Time_hr, Time_min, Time_sec, Time::kSunday);
    Time t(2015, 9, 22, 1, 38, 50, Time::kSunday);
    t.sec = Time_sec;
    t.min = Time_min;
    t.hr  =Time_hr;
    t.date=Time_day;
    t.mon=Time_mon;
    t.yr=Time_yr;

    rtc.time(t);
  }

}



