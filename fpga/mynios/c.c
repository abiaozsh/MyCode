//函数指针
//变量指针
//中断向量
//浮点数
#include "inc/io.h"
#include "inc/system.h"


int uart_read(int timeout){
  if(timeout!=-1){
    IOWR(MYTIMER, 0 ,0);
  }
  while(1){
    if(timeout!=-1){
      int timer = IORD(MYTIMER, 0);
      if(timer>1000000){
        return 0;
      }
    }
    int tmp = IORD(MYUART, 0);
    if(tmp&0x100){
      return tmp;
    }
  }
}

int uart_write(int val){
  while((IORD(MYUART, 1)) & 0x100);
  IOWR(MYUART, 1 ,val);
}

int myprintf(char* str){
  int idx = 0;
  while(1){
    char tmp = str[idx];
    if(tmp=='\0')break;
    uart_write(tmp);
    idx++;
  }
}

int myprintf2(char* str, int len){
  int idx = 0;
  int i;
  for(i=0;i<len;i++){
    char tmp = str[i];
    if(tmp=='\0')break;
    uart_write(tmp);
  }
}

char* chardata = "0123456789ABCDEF";
int printChar(int val, char s){
  uart_write('[');
  uart_write(s);
  int i = 4;
  uart_write(chardata[(val>>i)&0x0F]);
  uart_write(chardata[(val)&0x0F]);
  uart_write(']');
}

int num10s[] = {
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

void printInt(int val)
{
  int idx;
  int i;
  int num;
  if(val<0){
    num = val;
    uart_write('+');
  }else{
    num = -val;
    uart_write('-');
  }
	
	int outNum;
	for(idx = 0; idx < 10 ; idx++)
	{
		outNum = 0;
		int CmpNum = num10s[idx];
		for(i = 0; i < 10 ; i++)
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
    uart_write('0' + outNum);
	}
}

int errCode2 = 0;

int errCode = 1;

inline void _dly()
{
  IOWR(MYTIMER, 0, 0);
  while(1)
  {
	  int time = IORD(MYTIMER, 0);
	  if(time > 10){
		  break;
	  }
  }
}


void SPI_MISO_INIT(){;_dly();}//PORTB &= ~_BV(2);DDRB &= ~_BV(2)
int SPI_MISO_GET(){return IORD(SOFTISP, 0);}

void SPI_MOSI_INIT(){;_dly();}//PORTB &= ~_BV(0)
void SPI_MOSI_HIGH(){IOWR(SOFTISP, 1, 1);_dly();}
void SPI_MOSI_LOW(){IOWR(SOFTISP, 1, 0);_dly();}

void SPI_SCK_INIT(){;_dly();}//PORTD &= ~_BV(7)
void SPI_SCK_HIGH(){IOWR(SOFTISP, 2, 1);_dly();}
void SPI_SCK_LOW(){IOWR(SOFTISP, 2, 0);_dly();}

void SPI_CHIP_SELECT_INIT(){;_dly();}//PORTD &= ~_BV(6)
void SPI_CHIP_SELECT_HIGH(){IOWR(SOFTISP, 3, 0x07);_dly();}
void SPI_CHIP_SELECT_LOW(int chip){IOWR(SOFTISP, 3, ~(1<<chip));_dly();}


#define CMD0   0X00
#define CMD1   0X01
#define CMD8   0X08
#define CMD9   0X09
#define CMD10  0X0A
#define CMD13  0X0D
#define CMD17  0X11
#define CMD24  0X18
#define CMD25  0X19
#define CMD32  0X20
#define CMD33  0X21
#define CMD38  0X26
#define CMD55  0X37
#define CMD58  0X3A
#define ACMD41 0X29
//------------------------------------------------------------------------------
#define R1_READY_STATE        0X00
#define R1_IDLE_STATE         0X01
#define R1_ILLEGAL_COMMAND    0X04
#define DATA_START_BLOCK      0XFE
#define STOP_TRAN_TOKEN       0XFD
#define WRITE_MULTIPLE_TOKEN  0XFC
#define DATA_RES_MASK         0X1F
#define DATA_RES_ACCEPTED     0X05
//------------------------------------------------------------------------------
#define SD_INIT_TIMEOUT  1000
#define SD_READ_TIMEOUT  300
#define SD_WRITE_TIMEOUT 600
//------------------------------------------------------------------------------
// card types
#define SD_CARD_TYPE_SD1  1
#define SD_CARD_TYPE_SD2  2
#define SD_CARD_TYPE_SDHC 3
#define CACHE_FOR_READ    0
#define CACHE_FOR_WRITE   1



//------------------------------------------------------------------------------
int spiRec(void) {
  int data = 0;
  // no interrupts during byte receive - about 8 us
  //cli();
  // output pin high - like sending 0XFF
  SPI_MOSI_HIGH();
  int i;
  for (i = 0; i < 8; i++) {
    SPI_SCK_HIGH();

    data <<= 1;

    if (SPI_MISO_GET()) data |= 1;

    SPI_SCK_LOW();
  }
  // enable interrupts
  //sei();
  printChar(data,'r');
  return data;
}
//------------------------------------------------------------------------------
void spiSend(int data) {
  printChar(data,'s');
  // no interrupts during byte send - about 8 us
  //cli();
  int i;
  for (i = 0; i < 8; i++) {
    SPI_SCK_LOW();
    if(data & 0x80)
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
  //sei();
}

char* SDdata;


int  Sd2Card_status_;
int  Sd2Card_type_;

//------------------------------------------------------------------------------
// wait for card to go not busy
int Sd2Card_waitNotBusy(int timeoutMillis) {
	IOWR(MYTIMER, 0, 0);

  while(1)
  {
	  if (spiRec() == 0xFF) return 1;

	  int time = IORD(MYTIMER, 0);
	  if(time > timeoutMillis){
		  break;
	  }
  }
  return 0;
}

//------------------------------------------------------------------------------
// send command and return error code.  Return zero for OK
int Sd2Card_cardCommand(int cmd, int arg) {
  // select card
  SPI_CHIP_SELECT_LOW(0);
  // wait up to 300 ms if busy
  myprintf("W");
  Sd2Card_waitNotBusy(3000000);
  // send command
  spiSend(cmd | 0x40);
  // send argument
  int s;
  for (s = 24; s >= 0; s -= 8) spiSend(arg >> s);
  // send CRC
  int crc = 0xFF;
  if (cmd == CMD0) crc = 0x95;  // correct crc for CMD0 with arg 0
  if (cmd == CMD8) crc = 0x87;  // correct crc for CMD8 with arg 0X1AA
  spiSend(crc);
  // wait for response
  int i;
  for (i = 0; ((Sd2Card_status_ = spiRec()) & 0x80) && i != 0xFF; i++);
  return Sd2Card_status_;
}

int MMCCard_cardinit() {
  Sd2Card_type_ = 0;
  // 16-bit init start time allows over a minute
  int arg;

  // set pin modes
  SPI_CHIP_SELECT_INIT();
  SPI_CHIP_SELECT_HIGH();
  SPI_MISO_INIT();
  SPI_MOSI_INIT();
  SPI_SCK_INIT();


  // must supply min of 74 clock cycles with CS high.
  int i;
  for (i = 0; i < 20; i++) spiSend(0XFF);

  SPI_CHIP_SELECT_LOW(0);

  // command to go idle in SPI mode
  int ok = 0;
  for(i=0;i<10;i++){//SD_INIT_TIMEOUT*1000
    if((Sd2Card_status_ = Sd2Card_cardCommand(CMD0, 0)) == R1_IDLE_STATE)
    {
      myprintf("ok");
      ok=1;
      break;
    }
    myprintf("s:");
    printInt(Sd2Card_status_);
    myprintf("\r\n");
  }
  if (!ok) {
    errCode = 1;
    goto fail;
  }

  ok=0;
  for(i=0;i<100;i++){//SD_INIT_TIMEOUT*1000
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

  return 1;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return 0;
}






//------------------------------------------------------------------------------
int Sd2Card_waitStartBlock(void) {
  int ok=0;
  int i;
  for(i=0;i<SD_READ_TIMEOUT*1000;i++){
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
  return 1;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return 0;
}

int Sd2Card_readData(int block) {
  //myprintf("s1\r\n");
 // use address if not SDHC card
  if (Sd2Card_type_ != SD_CARD_TYPE_SDHC) block <<= 9;
  int result = Sd2Card_cardCommand(CMD17, block);
  printInt(result);
  //asm("hlt 1");
  //asm("hlt 0");
   if (result) {
    //myprintf("s2\r\n");
    //asm("hlt 1");
    //asm("hlt 0");
    errCode = 6;
    goto fail;
  }

  //myprintf("s3\r\n");
  //asm("hlt 1");
  //asm("hlt 0");

  if (!Sd2Card_waitStartBlock()) {
    goto fail;
  }
  // transfer data
  int i;
  for (i = 0; i < 512; i++) {
    SDdata[i] = spiRec();
  }
  
  spiRec();//CRC
  spiRec();

  SPI_CHIP_SELECT_HIGH();


  return 1;

 fail:
  //myprintf("s4\r\n");
  //asm("hlt 1");
  //asm("hlt 0");
  SPI_CHIP_SELECT_HIGH();
  //myprintf("s5\r\n");
  return 0;
}









int main()
{
  SDdata = (char*)(0);//at sdram [512]
  
	char buff[4];

	myprintf("Hello from Nios II!\r\n");

	while(1){
    char buff[2];
    buff[0] = (char)uart_read(-1);
    int s;

    
		if(buff[0]=='i'){
			myprintf("init start!\r\n");
			MMCCard_cardinit();
			myprintf("init end!\r\n");
		}

    if(buff[0]=='r'){
      int block;
      
      buff[0] = (char)uart_read(-1);
      if(buff[0]=='0'){
        block = 0;
      }else{
        block = 1;
      }
      
      
      myprintf("[");
      if (Sd2Card_readData(block)) {
      int i;
        for(i=0;i<512;i++){
          if((i&15)==0 && i!=0){
            myprintf("\n");
          }
          char data = SDdata[i];
          uart_write(data);
        }
        myprintf("]");
      }
      else {
        myprintf("err:");
        printInt(errCode);
      }

    }


	}
  return 0;
}
