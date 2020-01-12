#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


//1000000 / 9600 = 104.16666666666666666666666666667
//1000000 / 9600 = 156.25
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
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}
void TimerInit()
{
  // COM1A1 COM1A0 COM1B1 COM1B0 – – WGM11 WGM10
//CS02 CS01 CS00 Description
//0   0 0 0 No clock source (Timer/Counter stopped)
//1 8Mhz      0 0 1 clkI/O/(No prescaling)
//2 1Mhz      0 1 0 clkI/O/8 (From prescaler)
//3 1/8Mhz    0 1 1 clkI/O/64 (From prescaler)
//4 1/32Mhz   1 0 0 clkI/O/256 (From prescaler)
//5 1/128Mhz  1 0 1 clkI/O/1024 (From prescaler)
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










#define DDR_SCL  DDRC
#define PORT_SCL PORTC

#define DDR_SDA  DDRC
#define PORT_SDA PORTC
#define PIN_SDA  PINC

#define BIT_SCL _BV(0)
#define BIT_SDA _BV(1)


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

//000 : FSR = ±6.144 V(1)
//001 : FSR = ±4.096 V(1)
//010 : FSR = ±2.048 V (default)
//011 : FSR = ±1.024 V
//100 : FSR = ±0.512 V
//101 : FSR = ±0.256 V
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










/*
11.11 V
1.111 V
1111. mV
111.1 mV

11.11 A
1.111 A
111.1 mA
1111 uA
*/

volatile uint8_t _digi0;
volatile uint8_t _digi0Dot;
volatile uint8_t _digi1;
volatile uint8_t _digi1Dot;
volatile uint8_t _digi2;
volatile uint8_t _digi2Dot;
volatile uint8_t _digi3;
volatile uint8_t _digi3Dot;
volatile uint8_t _sign;

#define EN   (PINC & _BV(0))
#define DATA (PINC & _BV(1))
#define CLK  (PINC & _BV(2))
//PC3 4 5 衰减等级

//默认高电平，低电平有效
#define HIGH_RANGE (!(PINC & _BV(5)))
#define MID_RANGE  (!(PINC & _BV(4)))
#define LOW_RANGE  (!(PINC & _BV(3)))

// 
#define V_A (!(PINC & _BV(2)))
// (!(PINZ & _BV(Z)))
#define GHIGH (!(PINB & _BV(5)))
//电压/电流 通过数据口获得

#define CAL_V_LOW_RANGE_GHIGH  1.0
#define CAL_V_LOW_RANGE_G1     1.0
#define CAL_V_MID_RANGE_G1     1.12486
#define CAL_V_HIGH_RANGE_G1    1.031

#define CAL_A_LOW_RANGE_G20    1.0
#define CAL_A_LOW_RANGE_G1     1.0
#define CAL_A_MID_RANGE_G20    1.0
#define CAL_A_MID_RANGE_G1     1.0
#define CAL_A_HIGH_RANGE_G20   1.0
#define CAL_A_HIGH_RANGE_G1    1.0

volatile uint8_t doGetData;


void wait(uint8_t ticks)
{
	TCNT0 = 0;//timer reset
	while(TCNT0<ticks);
}

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

  //      A11-B2  
  //F10-B3      B07-D2
  //      G05-B7
  //E01-B0      C04-D5
  //      D02-D7      DP03-D6
  DDRB |= _BV(2);//A11
  DDRD |= _BV(2);//B07
  DDRD |= _BV(5);//C04
  DDRD |= _BV(7);//D02
  DDRB |= _BV(0);//E01
  DDRB |= _BV(3);//F10
  DDRB |= _BV(7);//G05
  DDRD |= _BV(6);//DP03

  DDRB |= _BV(1);//digit1
  DDRD |= _BV(4);//digit2
  DDRD |= _BV(3);//digit3
  DDRB |= _BV(6);//digit4
  //digit5??
  DDRB |= _BV(4);//sign
  
  uint8_t casev = 0;

  ADS1115(ADS1115_DEFAULT_ADDRESS);
  
  ADS1115_set_data_rate(ADS1115_DATA_RATE_8_SPS);
  ADS1115_set_mode(ADS1115_MODE_CONTINUOUS);
  
  
//#define ADS1115_MUX_DIFF_AIN0_AIN1 0
//#define ADS1115_MUX_DIFF_AIN0_AIN3 0x1 << ADS1115_MUX_SHIFT
//#define ADS1115_MUX_DIFF_AIN1_AIN3 0x2 << ADS1115_MUX_SHIFT
//#define ADS1115_MUX_DIFF_AIN2_AIN3 0x3 << ADS1115_MUX_SHIFT
//#define ADS1115_MUX_GND_AIN0 0x4 << ADS1115_MUX_SHIFT
//#define ADS1115_MUX_GND_AIN1 0x5 << ADS1115_MUX_SHIFT
//#define ADS1115_MUX_GND_AIN2 0x6 << ADS1115_MUX_SHIFT
//#define ADS1115_MUX_GND_AIN3 0x7 << ADS1115_MUX_SHIFT
//#define ADS1115_MUX_MASK 0x7 << ADS1115_MUX_SHIFT
//000 : AINP = AIN0 and AINN = AIN1 (default)
//001 : AINP = AIN0 and AINN = AIN3
//010 : AINP = AIN1 and AINN = AIN3
//011 : AINP = AIN2 and AINN = AIN3
  //ADS1115_set_mux(ADS1115_MUX_DIFF_AIN2_AIN3);//voltage


//000 : FSR = ±6.144 V(1)
//001 : FSR = ±4.096 V(1)
//010 : FSR = ±2.048 V (default)
//011 : FSR = ±1.024 V
//100 : FSR = ±0.512 V
//101 : FSR = ±0.256 V
//#define ADS1115_PGA_TWO_THIRDS 0
//#define ADS1115_PGA_ONE 0x1 << ADS1115_PGA_SHIFT
//#define ADS1115_PGA_TWO 0x2 << ADS1115_PGA_SHIFT
//#define ADS1115_PGA_FOUR 0x3 << ADS1115_PGA_SHIFT
//#define ADS1115_PGA_EIGHT 0x4 << ADS1115_PGA_SHIFT
//#define ADS1115_PGA_SIXTEEN 0x5 << ADS1115_PGA_SHIFT
//#define ADS1115_PGA_MASK 0x7 << ADS1115_PGA_SHIFT
  //ADS1115_set_pga(ADS1115_PGA_TWO);//±2.048 V (default)
  //ADS1115_set_pga(ADS1115_PGA_SIXTEEN);//±0.256 V
  
  
  uint16_t mux = 0;
  uint16_t pga = 0;
  
  uint16_t oldmux = 0;
  uint16_t oldpga = 0;
  
  uint8_t currentLv = 0;//0.256v
  
  ADS1115_trigger_sample();
  
  while (true) {
    while(!doGetData);
    doGetData = 0;
    int32_t val_req = ADS1115_read_sample();
    uint8_t sign = 0;

    uint8_t ovf = 0;
    if(val_req<0){
      val_req = -val_req;
      sign = 1;
      if(val_req==32768){
        ovf=1;
      }
    }else{
      if(val_req==32767){
        ovf=1;
      }
    }
    
    ///////////////////////////
    SerialSend(sign?'-':'+');
    SendInt(val_req);
    //SerialSend(' ');
    //SerialSend('0'+LOW_RANGE);
    //SerialSend('0'+MID_RANGE);
    //SerialSend('0'+HIGH_RANGE);
    //SerialSend('0'+V_A);
    //SerialSend('0'+GHIGH);
    //SerialSend('-');
    //SendInt(mux);
    //SerialSend('-');
    //SendInt(pga);
    SerialSend('\r');
    SerialSend('\n');
    
    float val = (float)val_req; //max +- 32k / 1.024v

    uint8_t digi0=0;
    uint8_t digi1=0;
    uint8_t digi2=0;
    uint8_t digi3=0;
    uint8_t digi0Dot=0;
    uint8_t digi1Dot=0;
    uint8_t digi2Dot=0;
    uint8_t digi3Dot=0;
    
    val = val / (32.0*1024.0); //v 1~0~-1
    
    if(V_A){
      //A
      if(LOW_RANGE){
        mux = ADS1115_MUX_DIFF_AIN0_AIN3;
        pga = ADS1115_PGA_SIXTEEN;//±0.256V
        //10OHM 0.256V  max 25.6mA
        val = val * 25.6 * CAL_A_LOW_RANGE_G1;
        //0.110A
        if(val>=10){
          //xx.xx mA
          val = val * 0.1;
          digi1Dot = 1;
        }else{
          //x.xxx mA
          digi0Dot = 1;
        }
      }else if(MID_RANGE){
        mux = ADS1115_MUX_DIFF_AIN0_AIN3;
        pga = ADS1115_PGA_SIXTEEN;//±0.256V
        //1 OHM 0.256V max 256mA
        val = val * 2.56 * CAL_A_MID_RANGE_G1;
        if(val>=1){
          //xxx.x mA
          digi2Dot = 1;
        }else{
          //xx.xx mA
          digi1Dot = 1;
          val = val * 10;
        }
      }else if(HIGH_RANGE){
        mux = ADS1115_MUX_DIFF_AIN1_AIN3;
        //0.025 OHM 0.256V to*20  max 10.24A
        float orival = val;
        if(currentLv==0){
          val = val * 10.24 * CAL_A_HIGH_RANGE_G1;//±0.256 V 10.24A
        }else if(currentLv==1){
          val = val * 20.48 * CAL_A_HIGH_RANGE_G1;//±0.512 V 20.48A
        }else if(currentLv==2){
          val = val * 40.96 * CAL_A_HIGH_RANGE_G1;//±1.024 V 40.96A
        }

        // 10A
        if(val>=10){
          //xx.xx A
          val = val * 0.1;
          digi1Dot = 1;
        }else{
          //x.xxx A
          digi0Dot = 1;
        }

        if(ovf && currentLv<2){
          currentLv++;
        }
        if(orival<0.1 && currentLv>0){
          currentLv--;
        }
          
        if(currentLv==0){
          pga = ADS1115_PGA_SIXTEEN;//±0.256 V 10.24A
        }else if(currentLv==1){
          pga = ADS1115_PGA_EIGHT;//±0.512 V 20.48A
        }else if(currentLv==2){
          pga = ADS1115_PGA_FOUR;//±1.024 V 40.96A
        }
        
      }
      
    }else{
      //V
      mux = ADS1115_MUX_DIFF_AIN2_AIN3;
      
      if(LOW_RANGE){
        if(GHIGH){
          pga = ADS1115_PGA_SIXTEEN;//±0.256 V
          val = val * 0.256 * CAL_V_LOW_RANGE_GHIGH;
          if(val>=0.1){
            //xxx.x mV
            val = val * 10;
            digi2Dot = 1;
          }else{
            //xx.xx mV
            val = val * 100;
            digi1Dot = 1;
          }
        }else{
          //max 2.048V   *1
          pga = ADS1115_PGA_TWO;//±2.048 V (default)
          val = val * 2.048 * CAL_V_LOW_RANGE_G1;
          if(val>=1){
            //x.xxx V
            digi0Dot = 1;
          }else{
            //xxx.x mV
            val = val * 10;
            digi2Dot = 1;
          }
        }
      }else if(MID_RANGE){
        //max 20.48V to *10
        pga = ADS1115_PGA_TWO;//±2.048 V (default)
        val = val * 20.48 * CAL_V_MID_RANGE_G1;
        if(val>=10){
          //xx.xx V
          val = val * 0.1;
          digi1Dot = 1;
        }else{
          //x.xxx V
          digi0Dot = 1;
        }
      }else if(HIGH_RANGE){
        //max 204.8V to *100
        pga = ADS1115_PGA_TWO;//±2.048 V (default)
        val = val * 204.8 * CAL_V_HIGH_RANGE_G1;
        if(val>=100){
          //xxx.x V
          val = val * 0.01;
          digi2Dot = 1;
        }else{
          //xx.xx V
          val = val * 0.1;
          digi1Dot = 1;
        }
      }
    }
    
    if(!ovf){
      digi0 = (uint8_t)val;
      val -= digi0;
      val = val * 10;
      digi1 = (uint8_t)val;
      val -= digi1;
      val = val * 10;
      digi2 = (uint8_t)val;
      val -= digi2;
      val = val * 10;
      digi3 = (uint8_t)val;
    }else{
      digi0 = 9;
      digi1 = 9;
      digi2 = 9;
      digi3 = 9;
    }
    
    if(mux!=oldmux){
      ADS1115_set_mux(mux);//voltage
      ADS1115_trigger_sample();
      oldmux=mux;
    }
    if(pga!=oldpga){
      ADS1115_set_pga(pga);//±2.048 V (default)
      ADS1115_trigger_sample();
      oldpga=pga;
    }

    _digi0    = digi0;
    _digi0Dot = digi0Dot;
    _digi1    = digi1;
    _digi1Dot = digi1Dot;
    _digi2    = digi2;
    _digi2Dot = digi2Dot;
    _digi3    = digi3;
    _digi3Dot = digi3Dot;
    _sign     = sign;
  }
}

volatile uint8_t cur_digit = 0;

ISR(TIMER1_COMPA_vect){
  TCNT1 = 0;
  if(cur_digit==0){
    doGetData = 1;
  }
  
  PORTB |= _BV(2);//A11
  PORTD |= _BV(2);//B07
  PORTD |= _BV(5);//C04
  PORTD |= _BV(7);//D02
  PORTB |= _BV(0);//E01
  PORTB |= _BV(3);//F10
  PORTB |= _BV(7);//G05
  PORTD |= _BV(6);//DP03

  PORTB&=~_BV(1);//digit1
  PORTD&=~_BV(4);//digit2
  PORTD&=~_BV(3);//digit3
  PORTB&=~_BV(6);//digit4
  
  PORTB |= _BV(4);//sign
  
  uint8_t cur_digit_val = 0;
  uint8_t cur_digit_dot = 0;
  if(cur_digit==0){
    PORTB |= _BV(1);
    cur_digit_val = _digi0;
    cur_digit_dot = _digi0Dot;
  }
  else if(cur_digit==1){
    PORTD |= _BV(4);
    cur_digit_val = _digi1;
    cur_digit_dot = _digi1Dot;
  }
  else if(cur_digit==2){
    PORTD |= _BV(3);
    cur_digit_val = _digi2;
    cur_digit_dot = _digi2Dot;
  }
  else if(cur_digit==3){
    PORTB |= _BV(6);
    cur_digit_val = _digi3;
    cur_digit_dot = _digi3Dot;
  }
  
  if(_sign){
    PORTB&=~_BV(4);
  }
  
  uint8_t segA=0;
  uint8_t segB=0;
  uint8_t segC=0;
  uint8_t segD=0;
  uint8_t segE=0;
  uint8_t segF=0;
  uint8_t segG=0;

  //      A11-B2  
  //F10-B3      B07-D2
  //      G05-B7
  //E01-B0      C04-D5
  //      D02-D7      DP03-D6
  if(cur_digit_val==0){
    segA=1;segB=1;segC=1;segD=1;segE=1;segF=1;
  }
  if(cur_digit_val==1){
    segB=1;segC=1;
  }
  if(cur_digit_val==2){
    segA=1;segB=1;segG=1;segE=1;segD=1;
  }
  if(cur_digit_val==3){
    segA=1;segB=1;segG=1;segC=1;segD=1;
  }
  if(cur_digit_val==4){
    segF=1;segG=1;segB=1;segC=1;
  }
  if(cur_digit_val==5){
    segA=1;segF=1;segG=1;segC=1;segD=1;
  }
  if(cur_digit_val==6){
    segA=1;segF=1;segG=1;segC=1;segD=1;segE=1;
  }
  if(cur_digit_val==7){
    segA=1;segB=1;segC=1;
  }
  if(cur_digit_val==8){
    segA=1;segB=1;segC=1;segD=1;segE=1;segF=1;segG=1;
  }
  if(cur_digit_val==9){
    segA=1;segB=1;segC=1;segD=1;segF=1;segG=1;
  }

  if(segA){
    PORTB &= ~_BV(2);//A11
  }
  if(segB){
    PORTD &= ~_BV(2);//B07
  }
  if(segC){
    PORTD &= ~_BV(5);//C04
  }
  if(segD){
    PORTD &= ~_BV(7);//D02
  }
  if(segE){
    PORTB &= ~_BV(0);//E01
  }
  if(segF){
    PORTB &= ~_BV(3);//F10
  }
  if(segG){
    PORTB &= ~_BV(7);//G05
  }

  if(cur_digit_dot){
    PORTD &= ~_BV(6);//DP03
  }
  
  cur_digit++;
  if(cur_digit==4){
    cur_digit=0;
  }
}
