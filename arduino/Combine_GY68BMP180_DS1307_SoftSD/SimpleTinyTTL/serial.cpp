#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define DDR_Send DDRA
#define PORT_Send PORTA
#define BIT_Send _BV(7)

#define DDR_SCL  DDRA
#define PORT_SCL PORTA

#define DDR_SDA  DDRA
#define PORT_SDA PORTA
#define PIN_SDA  PINA

#define BIT_SCL _BV(2)
#define BIT_SDA _BV(3)

#define CUR_TIMING TIMING__8M_TCCR0B_1_115200
#define TCCR0B_Value 1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BMP085_ADDRESS 0x77  // I2C address of BMP085
#define OSS 3

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

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

void wait(uint16_t length)//125/ms
{
  TCCR1A = 0;
  TCCR1B = 3;//1MHz 125/ms
  TCCR1C = 0;
  TIMSK1 = 0;
  TCNT1 = 0;TIFR1 |= _BV(TOV1);//overflow flg reset
  while(currTick<length)
  {
    ;
  }
}

void dly()
{
  for(uint8_t i=0;i<6;i++)//6 is stable
  {
    volatile uint8_t v=0;
    v++;
  }
}

void i2c_writebit( uint8_t c )
{
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
uint8_t i2c_readbit()
{
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
  uint8_t c = PIN_SDA; // I2C_PIN;
  i2c_scl_lo();
  dly();
  return ( c & BIT_SDA) ? 1 : 0;
}

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

uint8_t i2c_beginTransmission(uint8_t address)
{
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 0); // clr read bit
  return rc;
}

void i2c_endTransmission()
{
  i2c_scl_hi();
  dly();
  i2c_sda_hi();
  dly();
}
uint8_t i2c_requestFrom(uint8_t address)
{
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 1); // set read bit
  return rc;
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
uint16_t bmp085ReadInt(uint8_t address)
{
  uint8_t msb, lsb;

  i2c_beginTransmission(BMP085_ADDRESS);
  i2c_write(address);
  i2c_endTransmission();

  i2c_requestFrom(BMP085_ADDRESS);

  msb = i2c_read();
  lsb = i2c_readLast();
  return ((uint16_t) msb<<8) | lsb;
}


// Read 1 byte from the BMP085 at 'address'
uint8_t bmp085Read(uint8_t address)
{
  i2c_beginTransmission(BMP085_ADDRESS);
  i2c_write(address);
  i2c_endTransmission();
  i2c_requestFrom(BMP085_ADDRESS);
  uint8_t msb = i2c_readLast();
  return msb;
}








// Read the uncompensated temperature value
unsigned int bmp085ReadUT(){
  unsigned int ut;

  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  i2c_beginTransmission(BMP085_ADDRESS);
  i2c_write(0xF4);
  i2c_write(0x2E);
  i2c_endTransmission();

  // Wait at least 4.5ms
  //delay(5);
  wait(125*5);

  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP(){

  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;

  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  i2c_beginTransmission(BMP085_ADDRESS);
  uint8_t d1 = 0xF4;
  uint8_t d2 = 0x34 + (OSS<<6);//
  i2c_write(d1);
  i2c_write(d2);
  i2c_endTransmission();

  // Wait for conversion, delay time dependent on OSS
  //delay(2 + (3<<OSS));//3*8 24+2 26ms
  wait(125*26);

  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  msb = bmp085Read(0xF6);
  lsb = bmp085Read(0xF7);
  xlsb = bmp085Read(0xF8);

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

  return up;
}








///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2___9600[] = {104,208, 56,160,  8,113,217, 65,169, 17};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_2__14400[] = { 69,138,208, 21, 91,160,230, 43,113,182};
PROGMEM prog_uint8_t TIMING__8M_TCCR0B_1_115200[] = { 69,138,208, 21, 91,160,230, 43,113,182};


void ClockInit();
void loop();
void TimerInit();
void SerialInit();
void SerialSend(uint8_t val);
void SendInt(uint32_t val);

int main(void) {
	ClockInit();
	SerialInit();
	TimerInit();
	loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

void loop() {
	for(;;)
	{
    uint16_t ac1 = bmp085ReadInt(0xAA);SerialSend('a');SerialSend(':');SendInt(ac1);SerialSend('|');
    uint16_t ac2 = bmp085ReadInt(0xAC);SerialSend('b');SerialSend(':');SendInt(ac2);SerialSend('|');
    uint16_t ac3 = bmp085ReadInt(0xAE);SerialSend('c');SerialSend(':');SendInt(ac3);SerialSend('|');
    uint16_t ac4 = bmp085ReadInt(0xB0);SerialSend('d');SerialSend(':');SendInt(ac4);SerialSend('|');
    uint16_t ac5 = bmp085ReadInt(0xB2);SerialSend('e');SerialSend(':');SendInt(ac5);SerialSend('|');
    uint16_t ac6 = bmp085ReadInt(0xB4);SerialSend('f');SerialSend(':');SendInt(ac6);SerialSend('|');
    uint16_t b1 = bmp085ReadInt(0xB6); SerialSend('g');SerialSend(':');SendInt(b1); SerialSend('|');
    uint16_t b2 = bmp085ReadInt(0xB8); SerialSend('h');SerialSend(':');SendInt(b2); SerialSend('|');
    uint16_t mb = bmp085ReadInt(0xBA); SerialSend('i');SerialSend(':');SendInt(mb); SerialSend('|');
    uint16_t mc = bmp085ReadInt(0xBC); SerialSend('j');SerialSend(':');SendInt(mc); SerialSend('|');
    uint16_t md = bmp085ReadInt(0xBE); SerialSend('k');SerialSend(':');SendInt(md); SerialSend('|');
    
    uint16_t t = bmp085ReadUT();       SerialSend('t');SerialSend(':');SendInt(t); SerialSend('|');
    uint16_t p = bmp085ReadUP();       SerialSend('p');SerialSend(':');SendInt(p); SerialSend('|');
    
    //0.1s
    wait(125*100);

	}
}


void TimerInit() {
	TCCR0A = 0;
	TIMSK0 = 0;
}

void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	//DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}

PROGMEM prog_uint32_t num10s[] = {
1000000000,
100000000,
10000000,
1000000,
100000,
10000,
1000,
100,
10,
1,
};

void SendInt(uint32_t val)
{
	uint32_t num = val;
	for(uint8_t idx = 0; idx < 10 ; idx++)
	{
		uint8_t outNum = 0;
		uint32_t CmpNum = pgm_read_dword_near(num10s + idx);
		for(uint8_t i = 0; i < 10 ; i++)
		{
			if(num>=CmpNum)
			{
				num -= CmpNum;
				outNum++;
			}
			else
			{
				break;
			}
		}
		SerialSend('0' + outNum);
	}
}

void SerialSend(uint8_t val)
{
	cli();
	TCCR0B = TCCR0B_Value;
	TCNT0 = 0;
	uint8_t timing;
	prog_uint8_t* pTiming = CUR_TIMING;
	PORT_Send &= ~BIT_Send;timing = pgm_read_byte_near(pTiming++);while(timing-TCNT0>0);//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit){PORT_Send |= BIT_Send;}else{PORT_Send &= ~BIT_Send;}chkbit<<=1;timing = pgm_read_byte_near(pTiming++);while(timing-TCNT0>0);
	}
	PORT_Send |= BIT_Send;timing = pgm_read_byte_near(pTiming);while(timing-TCNT0>0);
	sei();
}
