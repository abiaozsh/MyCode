#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


//D1
#define DDR_Send DDRD
#define PORT_Send PORTD
#define BIT_Send _BV(1)

//D0
#define DDR_Recv DDRD
#define PIN_Recv PIND
#define BIT_Recv _BV(0)


//8000000 / 115200 = 69.4
//8000000 / 115200 * 1.5 = 104.16666666666666666666666666667
#define TCCR0_Value_115200 1
#define CNT_Value_115200 64
#define CNT_1_5_115200 96


//1000000 / 9600 = 104.16666666666666666666666666667
//1000000 / 9600 = 156.25
#define TCCR0_Value_9600 2
#define CNT_Value_9600 104
#define CNT_1_5_9600 156


#define TCCR0_Value TCCR0_Value_115200
#define CNT_Value CNT_Value_115200
#define CNT_1_5 CNT_1_5_115200


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
uint8_t SerialRead()
{
	cli();
	uint8_t val = 0;
	TCCR0 = TCCR0_Value;
	while(PIN_Recv&BIT_Recv){
	}
	TCNT0 = 0;
	while(TCNT0<CNT_1_5);TCNT0 = 0;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);
		while(TCNT0<CNT_Value);TCNT0 = 0;
	}
	sei();
	return val;
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














uint8_t errCode = 0;

inline void _dly()
{
  for(uint8_t i=0;i<20;i++)//5 is good
  {
    asm volatile("nop");
  }
}

//B0
void SPI_MOSI_INIT(){PORTB &= ~_BV(0);_dly();}
void SPI_MOSI_HIGH(){DDRB &= ~_BV(0);_dly();}
void SPI_MOSI_LOW(){DDRB |= _BV(0);_dly();}

//B2
void SPI_MISO_INIT(){PORTB &= ~_BV(2);DDRB &= ~_BV(2);_dly();}
uint8_t SPI_MISO_GET(){return (PINB) & _BV(2);}

//D7
void SPI_SCK_INIT(){PORTD &= ~_BV(7);_dly();}
void SPI_SCK_HIGH(){DDRD &= ~_BV(7);_dly();}
void SPI_SCK_LOW(){DDRD |= _BV(7);_dly();}

//D6
void SPI_CHIP_SELECT_INIT(){PORTD &= ~_BV(6);_dly();}
void SPI_CHIP_SELECT_HIGH(){DDRD &= ~_BV(6);_dly();}
void SPI_CHIP_SELECT_LOW(){DDRD |= _BV(6);_dly();}


uint8_t const CMD0 = 0X00;
uint8_t const CMD1 = 0X01;
uint8_t const CMD8 = 0X08;
uint8_t const CMD9 = 0X09;
uint8_t const CMD10 = 0X0A;
uint8_t const CMD13 = 0X0D;
uint8_t const CMD17 = 0X11;
uint8_t const CMD24 = 0X18;
uint8_t const CMD25 = 0X19;
uint8_t const CMD32 = 0X20;
uint8_t const CMD33 = 0X21;
uint8_t const CMD38 = 0X26;
uint8_t const CMD55 = 0X37;
uint8_t const CMD58 = 0X3A;
uint8_t const ACMD41 = 0X29;
//------------------------------------------------------------------------------
uint8_t const R1_READY_STATE = 0X00;
uint8_t const R1_IDLE_STATE = 0X01;
uint8_t const R1_ILLEGAL_COMMAND = 0X04;
uint8_t const DATA_START_BLOCK = 0XFE;
uint8_t const STOP_TRAN_TOKEN = 0XFD;
uint8_t const WRITE_MULTIPLE_TOKEN = 0XFC;
uint8_t const DATA_RES_MASK = 0X1F;
uint8_t const DATA_RES_ACCEPTED = 0X05;
//------------------------------------------------------------------------------
uint16_t const SD_INIT_TIMEOUT = 1000;
uint16_t const SD_READ_TIMEOUT = 300;
uint16_t const SD_WRITE_TIMEOUT = 600;
//------------------------------------------------------------------------------
// card types
uint8_t const SD_CARD_TYPE_SD1 = 1;
uint8_t const SD_CARD_TYPE_SD2 = 2;
uint8_t const SD_CARD_TYPE_SDHC = 3;




uint8_t const CACHE_FOR_READ = 0;
uint8_t const CACHE_FOR_WRITE = 1;

uint8_t  SDdata[512];


//------------------------------------------------------------------------------
uint8_t spiRec(void) {
  uint8_t data = 0;
  // no interrupts during byte receive - about 8 us
  cli();
  // output pin high - like sending 0XFF
  SPI_MOSI_HIGH();
  
  for (uint8_t i = 0; i < 8; i++) {
    SPI_SCK_HIGH();

    data <<= 1;

    if (SPI_MISO_GET()) data |= 1;

    SPI_SCK_LOW();
  }
  // enable interrupts
  sei();
  return data;
}
//------------------------------------------------------------------------------
void spiSend(uint8_t data) {
  // no interrupts during byte send - about 8 us
  cli();
  for (uint8_t i = 0; i < 8; i++) {
    SPI_SCK_LOW();
    if(data & 0X80)
    {
      SPI_MOSI_HIGH();
    }
    else
    {
      SPI_MOSI_LOW();
    }
    data <<= 1;
    SPI_SCK_HIGH();
  }
  SPI_SCK_LOW();
  // enable interrupts
  sei();
}



uint8_t  Sd2Card_status_;
uint8_t  Sd2Card_type_;

//------------------------------------------------------------------------------
// wait for card to go not busy
uint8_t Sd2Card_waitNotBusy(uint16_t timeoutMillis) {
  while(1)
  {
    if (spiRec() == 0XFF) return true;
  }
  return false;
}

//------------------------------------------------------------------------------
// send command and return error code.  Return zero for OK
uint8_t Sd2Card_cardCommand(uint8_t cmd, uint32_t arg) {
  // select card
  SPI_CHIP_SELECT_LOW();
  // wait up to 300 ms if busy
  SerialSend('W');
  Sd2Card_waitNotBusy(300);
  // send command
  spiSend(cmd | 0x40);
  // send argument
  for (int8_t s = 24; s >= 0; s -= 8) spiSend(arg >> s);
  // send CRC
  uint8_t crc = 0XFF;
  if (cmd == CMD0) crc = 0X95;  // correct crc for CMD0 with arg 0
  if (cmd == CMD8) crc = 0X87;  // correct crc for CMD8 with arg 0X1AA
  spiSend(crc);
  // wait for response
  for (uint8_t i = 0; ((Sd2Card_status_ = spiRec()) & 0X80) && i != 0XFF; i++);
  return Sd2Card_status_;
}

uint8_t Sd2Card_cardinit() {
  Sd2Card_type_ = 0;
  // 16-bit init start time allows over a minute
  uint32_t arg;

  // set pin modes
  SPI_CHIP_SELECT_INIT();
  SPI_CHIP_SELECT_HIGH();
  SPI_MISO_INIT();
  SPI_MOSI_INIT();
  SPI_SCK_INIT();


  // must supply min of 74 clock cycles with CS high.
  for (uint8_t i = 0; i < 10; i++) spiSend(0XFF);

  SPI_CHIP_SELECT_LOW();

  // command to go idle in SPI mode
  uint8_t ok = 0;
  for(uint32_t i=0;i<100;i++){//SD_INIT_TIMEOUT*1000
    if((Sd2Card_status_ = Sd2Card_cardCommand(CMD0, 0)) != R1_IDLE_STATE) {
    }
    else
    {
      ok=1;
      break;
    }
  }
  if (!ok) {
    errCode = 1;
    goto fail;
  }

  // check SD version
  if ((Sd2Card_cardCommand(CMD8, 0x1AA) & R1_ILLEGAL_COMMAND)) {
    Sd2Card_type_ = SD_CARD_TYPE_SD1;
  } else {
    // only need last byte of r7 response
    for (uint8_t i = 0; i < 4; i++) Sd2Card_status_ = spiRec();
    if (Sd2Card_status_ != 0XAA) {
      errCode = 2;
      goto fail;
    }
    Sd2Card_type_ = SD_CARD_TYPE_SD2;
  }
  // initialize card and send host supports SDHC if SD2
  arg = Sd2Card_type_ == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;

  ok=0;
  for(uint32_t i=0;i<100;i++){//SD_INIT_TIMEOUT*1000
    Sd2Card_cardCommand(CMD55, 0);
    if((Sd2Card_status_ = Sd2Card_cardCommand(ACMD41, arg)) != R1_READY_STATE) {
    }
    else
    {
      ok=1;
      break;
    }
  }
  if (!ok) {
      errCode = 3;
      goto fail;
  }
  
  // if SD2 read OCR register to check for SDHC card
  if (Sd2Card_type_ == SD_CARD_TYPE_SD2) {
    if (Sd2Card_cardCommand(CMD58, 0)) {
      errCode = 5;
      goto fail;
    }
    if ((spiRec() & 0XC0) == 0XC0) Sd2Card_type_ = SD_CARD_TYPE_SDHC;
    // discard rest of ocr - contains allowed voltage range
    for (uint8_t i = 0; i < 3; i++) spiRec();
  }
  SPI_CHIP_SELECT_HIGH();

  return true;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return false;
}

uint8_t MMCCard_cardinit() {
  Sd2Card_type_ = 0;
  // 16-bit init start time allows over a minute
  uint32_t arg;

  // set pin modes
  SPI_CHIP_SELECT_INIT();
  SPI_CHIP_SELECT_HIGH();
  SPI_MISO_INIT();
  SPI_MOSI_INIT();
  SPI_SCK_INIT();


  // must supply min of 74 clock cycles with CS high.
  for (uint8_t i = 0; i < 100; i++) spiSend(0XFF);

  SPI_CHIP_SELECT_LOW();

  // command to go idle in SPI mode
  uint8_t ok = 0;
  for(uint32_t i=0;i<1000;i++){//SD_INIT_TIMEOUT*1000
    if((Sd2Card_status_ = Sd2Card_cardCommand(CMD0, 0)) == R1_IDLE_STATE) 
    {
      ok=1;
      break;
    }
    SendInt(Sd2Card_status_);
  }
  if (!ok) {
    errCode = 1;
    goto fail;
  }
SerialSend('O');

  ok=0;
  for(uint32_t i=0;i<100;i++){//SD_INIT_TIMEOUT*1000
    if(Sd2Card_cardCommand(CMD1, 0) == R1_READY_STATE) {
      ok=1;
      break;
    }
  }
  if (!ok) {
      errCode = 3;
      goto fail;
  }
  
  SPI_CHIP_SELECT_HIGH();

  return true;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return false;
}

//------------------------------------------------------------------------------
uint8_t Sd2Card_waitStartBlock(void) {
  uint8_t ok=0;
  for(uint32_t i=0;i<SD_READ_TIMEOUT*1000;i++){
    if((Sd2Card_status_ = spiRec()) == 0XFF) {
    }
    else
    {
      ok=1;
      break;
    }
  }
  if (!ok) {
    errCode = 8;
    goto fail;
  }

  if (Sd2Card_status_ != DATA_START_BLOCK) {
    errCode = 9;
    goto fail;
  }
  return true;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return false;
}

uint8_t Sd2Card_readData(uint32_t block) {

  // use address if not SDHC card
  if (Sd2Card_type_ != SD_CARD_TYPE_SDHC) block <<= 9;
  if (Sd2Card_cardCommand(CMD17, block)) {
    errCode = 6;
    goto fail;
  }
  if (!Sd2Card_waitStartBlock()) {
    goto fail;
  }

  // transfer data
  for (uint16_t i = 0; i < 512; i++) {
    SDdata[i] = spiRec();
  }
  
  spiRec();//CRC
  spiRec();

  SPI_CHIP_SELECT_HIGH();
  return true;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return false;
}

//------------------------------------------------------------------------------
// send one block of data for write block or write multiple blocks
uint8_t Sd2Card_writeData(uint8_t token, uint8_t* src) {
  spiSend(token);
  for (uint16_t i = 0; i < 512; i++) {
    spiSend(src[i]);
  }
  spiSend(0xff);  // dummy crc
  spiSend(0xff);  // dummy crc

  Sd2Card_status_ = spiRec();
  if ((Sd2Card_status_ & DATA_RES_MASK) != DATA_RES_ACCEPTED) {
    errCode = 14;
    SPI_CHIP_SELECT_HIGH();
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
uint8_t Sd2Card_writeBlock(uint32_t blockNumber, uint8_t* src) {

  // use address if not SDHC card
  if (Sd2Card_type_ != SD_CARD_TYPE_SDHC) blockNumber <<= 9;
  if (Sd2Card_cardCommand(CMD24, blockNumber)) {
    errCode = 10;
    goto fail;
  }
  if (!Sd2Card_writeData(DATA_START_BLOCK, src)) goto fail;

  // wait for flash programming to complete
  if (!Sd2Card_waitNotBusy(SD_WRITE_TIMEOUT)) {
    errCode = 11;
    goto fail;
  }
  // response is r2 so get and check two bytes for nonzero
  if (Sd2Card_cardCommand(CMD13, 0) || spiRec()) {
    errCode = 12;
    goto fail;
  }
  SPI_CHIP_SELECT_HIGH();
  return true;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return false;
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
void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}

uint8_t ConvBCD(uint8_t val){
  if(val>='0'&&val<='9')
  {
    return val - '0';
  }
  else if(val>='a'&&val<='f')
  {
    return val - 'a' + 10;
  }
  else if(val>='A'&&val<='F')
  {
    return val - 'A' + 10;
  }
  return 0;
}

uint8_t GetByte(){
  uint8_t vh = ConvBCD(SerialRead());
  uint8_t vl = ConvBCD(SerialRead());
  uint8_t val = (( vh << 4 ) & 0xF0) | 
                (  vl        & 0x0F);
  return val;
}
uint8_t GetWord(){
  uint16_t v3 = ConvBCD(SerialRead());
  uint16_t v2 = ConvBCD(SerialRead());
  uint16_t v1 = ConvBCD(SerialRead());
  uint16_t v0 = ConvBCD(SerialRead());
  uint16_t val =(( v3 << 12) & 0xF000) | 
                (( v2 << 8 ) & 0x0F00) | 
                (( v1 << 4 ) & 0x00F0) | 
                (  v0        & 0x000F);
  return val;
}
uint8_t GetDWord(){
  uint32_t v7 = ConvBCD(SerialRead());
  uint32_t v6 = ConvBCD(SerialRead());
  uint32_t v5 = ConvBCD(SerialRead());
  uint32_t v4 = ConvBCD(SerialRead());
  uint32_t v3 = ConvBCD(SerialRead());
  uint32_t v2 = ConvBCD(SerialRead());
  uint32_t v1 = ConvBCD(SerialRead());
  uint32_t v0 = ConvBCD(SerialRead());
  uint32_t val =(( v7 << 28) & 0xF0000000) | 
                (( v6 << 24) & 0x0F000000) | 
                (( v5 << 20) & 0x00F00000) | 
                (( v4 << 16) & 0x000F0000) | 
                (( v3 << 12) & 0x0000F000) | 
                (( v2 << 8 ) & 0x00000F00) | 
                (( v1 << 4 ) & 0x000000F0) | 
                (  v0        & 0x0000000F);
  return val;
}

PROGMEM prog_uint8_t convt[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

//void printHex(uint16_t val){//"ll"+"hh"
//  Serial.print(convt[((val & 0xF0) >> 4)]);
//  Serial.print(convt[((val & 0x0F))]);
//  Serial.print(convt[((val & 0xF000) >> 12)]);
//  Serial.print(convt[((val & 0x0F00) >> 8)]);
//}
void printHex(uint8_t val){//"hl"
  SerialSend(pgm_read_byte_near(convt+((val & 0xF0) >> 4)));
  SerialSend(pgm_read_byte_near(convt+((val & 0x0F))));
}


int main(void) {
	SerialInit();
	TimerInit();
  //DDRD &= ~_BV(6);//pwrPIN;
  SerialSend('R');
  SerialSend('E');
  SerialSend('A');
  SerialSend('D');
  SerialSend('Y');
  SerialSend('\r');
  SerialSend('\n');

	for(;;)
	{
    uint8_t cmd = SerialRead();
    
    if(cmd=='m'){
      SerialSend('M');
      if(!MMCCard_cardinit()) {
        SendInt(errCode);
        SerialSend('N');
        SerialSend('G');
        SerialSend('\r');
        SerialSend('\n');
      }
      else
      {
        SerialSend('O');
        SerialSend('K');
        SerialSend('\r');
        SerialSend('\n');
      }
    }
    if(cmd=='i'){
      if(!Sd2Card_cardinit()) {
        SendInt(errCode);
        SerialSend('N');
        SerialSend('G');
        SerialSend('\r');
        SerialSend('\n');
      }
      else
      {
        SerialSend('O');
        SerialSend('K');
        SerialSend('\r');
        SerialSend('\n');
      }
    }
    if(cmd=='r'){
      uint32_t block = GetDWord();
      SerialSend('[');
      if (Sd2Card_readData(block)) {
        for(uint16_t i=0;i<512;i++){
          if((i&15)==0 && i!=0){
            SerialSend('\r');
            SerialSend('\n');
          }
          uint8_t data = SDdata[i];
          printHex(data);
        }
        SerialSend(']');
      }
      else {
        SendInt(errCode);
        SerialSend('E');
      }
      
    }
    //if(cmd=='w'){
    //  uint8_t n = SerialRead();
    //  filename[0] = n;
    //  filename[1] = 'R';
    //  filename[2] = 'I';
    //  filename[3] = 'T';
    //  filename[4] = 'E';
    //  filename[5] = 'F';
    //  filename[6] = 'I';
    //  filename[7] = 'L';
    //  filename[8] = 'T';
    //  filename[9] = 'X';
    //  filename[10] = 'T';
    //  filename[11] = '\0';
    //  SerialSend('W');
    //  if (File_open(filename, 0)) {
    //    for(uint16_t i=0;i<700;i++){
    //      char cc = 'a'+(i&31);
    //      SerialSend(cc);
    //      File_write(cc);
    //    }
    //    File_close();
    //    SerialSend('D');
    //  }
    //  else {
    //    SerialSend('E');
    //  }
    //}
	}
}
