#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define TCCR0_Value_9600 2
#define CNT_Value_9600 104
#define CNT_1_5_9600 156

#define TCCR0_Value TCCR0_Value_9600
#define CNT_Value CNT_Value_9600
#define CNT_1_5 CNT_1_5_9600

//pd0 rxd		usb txd
//pd1 txd		usb rxd
#define DDR_Send DDRD
#define PORT_Send PORTD
#define BIT_Send _BV(1)
#define DDR_Recv DDRD
#define PIN_Recv PIND
#define BIT_Recv _BV(0)


void SerialInit(){
  DDR_Send |= BIT_Send;
  DDR_Recv &= ~BIT_Recv;
  PORT_Send |= BIT_Send;
}

void TimerInit(){
  TCCR0 = TCCR0_Value;
}

void SerialSend(uint8_t val){
	cli();
	TCCR0 = TCCR0_Value;
	TCNT0 = 0;
	PORT_Send &= ~BIT_Send;
  while(TCNT0<CNT_Value);TCNT0 = 0;//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit)
    {
      PORT_Send |= BIT_Send;
    }else{
      PORT_Send &= ~BIT_Send;
    }
    chkbit<<=1;
    while(TCNT0<CNT_Value);TCNT0 = 0;
	}
	PORT_Send |= BIT_Send;
  while(TCNT0<CNT_Value);TCNT0 = 0;//stopbit
	sei();
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

void SendUInt(uint32_t val){
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

void SendInt(int32_t val){
  if(val<0){
    val=-val;
    SerialSend('-');
  }
  SendUInt(val);
}

void wait(uint8_t ticks){
	TCNT0 = 0;//timer reset
	while(TCNT0<ticks);
}







#define DDR_SCL  DDRC
#define PORT_SCL PORTC

#define DDR_SDA  DDRC
#define PORT_SDA PORTC
#define PIN_SDA  PINC

#define BIT_SCL _BV(5)
#define BIT_SDA _BV(4)


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







#define ADS1115_COMP_QUEUE_SHIFT 0
#define ADS1115_COMP_LATCH_SHIFT 2
#define ADS1115_COMP_POLARITY_SHIFT 3
#define ADS1115_COMP_MODE_SHIFT 4
#define ADS1115_DATA_RATE_SHIFT 5
#define ADS1115_MODE_SHIFT 8
#define ADS1115_PGA_SHIFT 9
#define ADS1115_MUX_SHIFT 12

#define ADS1115_COMP_QUEUE_AFTER_ONE 0
#define ADS1115_COMP_QUEUE_AFTER_TWO 0x1 << ADS1115_COMP_QUEUE_SHIFT
#define ADS1115_COMP_QUEUE_AFTER_FOUR 0x2 << ADS1115_COMP_QUEUE_SHIFT
#define ADS1115_COMP_QUEUE_DISABLE 0x3 << ADS1115_COMP_QUEUE_SHIFT
#define ADS1115_COMP_QUEUE_MASK 0x3 << ADS1115_COMP_QUEUE_SHIFT

#define ADS1115_COMP_LATCH_NO 0
#define ADS1115_COMP_LATCH_YES 1 << ADS1115_COMP_LATCH_SHIFT
#define ADS1115_COMP_LATCH_MASK 1 << ADS1115_COMP_LATCH_SHIFT

#define ADS1115_COMP_POLARITY_ACTIVE_LOW 0
#define ADS1115_COMP_POLARITY_ACTIVE_HIGH 1 << ADS1115_COMP_POLARITY_SHIFT
#define ADS1115_COMP_POLARITY_MASK 1 << ADS1115_COMP_POLARITY_SHIFT

#define ADS1115_COMP_MODE_WINDOW 0
#define ADS1115_COMP_MODE_HYSTERESIS 1 << ADS1115_COMP_MODE_SHIFT
#define ADS1115_COMP_MODE_MASK 1 << ADS1115_COMP_MODE_SHIFT

#define ADS1115_DATA_RATE_8_SPS 0
#define ADS1115_DATA_RATE_16_SPS 0x1 << ADS1115_DATA_RATE_SHIFT
#define ADS1115_DATA_RATE_32_SPS 0x2 << ADS1115_DATA_RATE_SHIFT
#define ADS1115_DATA_RATE_64_SPS 0x3 << ADS1115_DATA_RATE_SHIFT
#define ADS1115_DATA_RATE_128_SPS 0x4 << ADS1115_DATA_RATE_SHIFT
#define ADS1115_DATA_RATE_250_SPS 0x5 << ADS1115_DATA_RATE_SHIFT
#define ADS1115_DATA_RATE_475_SPS 0x6 << ADS1115_DATA_RATE_SHIFT
#define ADS1115_DATA_RATE_860_SPS 0x7 << ADS1115_DATA_RATE_SHIFT
#define ADS1115_DATA_RATE_MASK 0x7 << ADS1115_DATA_RATE_SHIFT

#define ADS1115_MODE_CONTINUOUS 0
#define ADS1115_MODE_SINGLE_SHOT 1 << ADS1115_MODE_SHIFT
#define ADS1115_MODE_MASK 1 << ADS1115_MODE_SHIFT

//000 : FSR = ¡À6.144 V(1)
//001 : FSR = ¡À4.096 V(1)
//010 : FSR = ¡À2.048 V (default)
//011 : FSR = ¡À1.024 V
//100 : FSR = ¡À0.512 V
//101 : FSR = ¡À0.256 V
#define ADS1115_PGA_TWO_THIRDS 0
#define ADS1115_PGA_ONE 0x1 << ADS1115_PGA_SHIFT
#define ADS1115_PGA_TWO 0x2 << ADS1115_PGA_SHIFT
#define ADS1115_PGA_FOUR 0x3 << ADS1115_PGA_SHIFT
#define ADS1115_PGA_EIGHT 0x4 << ADS1115_PGA_SHIFT
#define ADS1115_PGA_SIXTEEN 0x5 << ADS1115_PGA_SHIFT
#define ADS1115_PGA_MASK 0x7 << ADS1115_PGA_SHIFT

#define ADS1115_MUX_DIFF_AIN0_AIN1 0
#define ADS1115_MUX_DIFF_AIN0_AIN3 0x1 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_DIFF_AIN1_AIN3 0x2 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_DIFF_AIN2_AIN3 0x3 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_GND_AIN0 0x4 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_GND_AIN1 0x5 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_GND_AIN2 0x6 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_GND_AIN3 0x7 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_MASK 0x7 << ADS1115_MUX_SHIFT

#define ADS1115_DEFAULT_ADDRESS 0x48

#define SAMPLE_BIT (0x8000)

#define ADS1115_REGISTER_CONVERSION 0
#define ADS1115_REGISTER_CONFIG 1
#define ADS1115_REGISTER_LOW_THRESH 2
#define ADS1115_REGISTER_HIGH_THRESH 3


uint8_t m_address;
uint16_t m_config;
int m_voltage_range;


void set_config(uint16_t val, uint16_t mask) {
  m_config = (m_config & ~mask) | val;
}

void ADS1115_set_comp_queue(uint16_t val) {
  set_config(val, ADS1115_COMP_QUEUE_MASK);
}
void ADS1115_set_comp_latching(uint16_t val) {
  set_config(val, ADS1115_COMP_LATCH_MASK);
}
void ADS1115_set_comp_polarity(uint16_t val) {
  set_config(val, ADS1115_COMP_POLARITY_MASK);
}
void ADS1115_set_comp_mode(uint16_t val) {
  set_config(val, ADS1115_COMP_MODE_MASK);
}
void ADS1115_set_data_rate(uint16_t val) {
  set_config(val, ADS1115_DATA_RATE_MASK);
}
void ADS1115_set_mode(uint16_t val) {
  set_config(val, ADS1115_MODE_MASK);
}
void ADS1115_set_pga(uint16_t val) {
  set_config(val, ADS1115_PGA_MASK); 
  m_voltage_range = val >> ADS1115_PGA_SHIFT;
}
void ADS1115_set_mux(uint16_t val) {
  set_config(val, ADS1115_MUX_MASK);
}

uint8_t write_register(uint8_t reg, uint16_t val)
{
  i2c_beginTransmission(m_address);
  i2c_write(reg);
  i2c_write(val>>8);
  i2c_write(val & 0xFF);
  i2c_endTransmission();
  return 0;
}

uint16_t read_register(uint8_t reg)
{
  i2c_beginTransmission(m_address);
  i2c_write(reg);
  i2c_endTransmission();

  i2c_requestFrom(m_address);

  uint16_t val;

  val = i2c_read() << 8;
  val |= i2c_readLast();
  return val;
}

uint8_t ADS1115_trigger_sample()
{
  return write_register(ADS1115_REGISTER_CONFIG, m_config | SAMPLE_BIT);
}

uint8_t ADS1115_reset()
{
	i2c_beginTransmission(0);
	i2c_write(0x6);
	i2c_endTransmission();
  return 0;
}

bool ADS1115_is_sample_in_progress()
{
	uint16_t val = read_register(ADS1115_REGISTER_CONFIG);
	return (val & SAMPLE_BIT) == 0;
}

int16_t ADS1115_read_sample()
{
  return read_register(ADS1115_REGISTER_CONVERSION);
}

void ADS1115(uint8_t address)
{
  m_address = address;
  m_config = ADS1115_COMP_QUEUE_AFTER_ONE |
             ADS1115_COMP_LATCH_NO |
             ADS1115_COMP_POLARITY_ACTIVE_LOW |
             ADS1115_COMP_MODE_WINDOW |
             ADS1115_DATA_RATE_128_SPS |
             ADS1115_MODE_SINGLE_SHOT |
             ADS1115_MUX_GND_AIN0;
  ADS1115_set_pga(ADS1115_PGA_ONE);
}


volatile uint8_t doGetData;

int main()
{
  TCCR1A = 0;
//0 0 0 No clock source. (Timer/Counter stopped)
//0 0 1 clkI/O/1 (No prescaling)
//0 1 0 clkI/O/8 (From prescaler)
//0 1 1 clkI/O/64 (From prescaler)
//1 0 0 clkI/O/256 (From prescaler)
//1 0 1 clkI/O/1024 (From prescaler)
  TCCR1B = 3;//1/8 Mhz
  TCNT1 = 0;
  TIMSK |= _BV(OCIE1A);
  OCR1A = 375;//8us * 375  3ms  333t/s(83.3hz refresh) count 41.6: 8 times/s

  SerialInit();
  TimerInit();
  sei();

  ADS1115(ADS1115_DEFAULT_ADDRESS);
  
  ADS1115_set_data_rate(ADS1115_DATA_RATE_16_SPS);
  //ADS1115_set_data_rate(ADS1115_DATA_RATE_8_SPS);
  ADS1115_set_mode(ADS1115_MODE_CONTINUOUS);
  ADS1115_set_mux(ADS1115_MUX_DIFF_AIN0_AIN1);
  ADS1115_set_pga(ADS1115_PGA_TWO);
  ADS1115_trigger_sample();
  
  while(true){
    //while(!doGetData);
    //doGetData = 0;
    
    
    int16_t val = ADS1115_read_sample();
    SendInt(val);
    SerialSend('\r');
    SerialSend('\n');
    //for(uint16_t i=0;i<100;i++){
    //  wait(250);
    //}
  }
}
uint8_t count = 0;//41

ISR(TIMER1_COMPA_vect){
  TCNT1 = 0;
  count++;
  if(count==41){//41
    count==0;
    doGetData = 1;
  }
}