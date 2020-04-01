
int errCode2 = 0;

int errCode = 1;



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
  SPI_CHIP_SELECT_HIGH();


  // must supply min of 74 clock cycles with CS high.
  int i;
  for (i = 0; i < 20; i++) spiSend(0XFF);

  SPI_CHIP_SELECT_LOW(0);

  // command to go idle in SPI mode
  int ok = 0;
  for(i=0;i<10;i++){//SD_INIT_TIMEOUT*1000
    if((Sd2Card_status_ = Sd2Card_cardCommand(CMD0, 0)) == R1_IDLE_STATE)
    {
      ok=1;
      break;
    }
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

int Sd2Card_readData(int block, char* buff) {
 // use address if not SDHC card
  if (Sd2Card_type_ != SD_CARD_TYPE_SDHC) block <<= 9;
  int result = Sd2Card_cardCommand(CMD17, block);
   if (result) {
    errCode = 6;
    goto fail;
  }

  if (!Sd2Card_waitStartBlock()) {
    goto fail;
  }
  // transfer data
  int i;
  for (i = 0; i < 512; i++) {
    buff[i] = spiRec();
  }
  
  spiRec();//CRC
  spiRec();

  SPI_CHIP_SELECT_HIGH();


  return 1;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return 0;
}

//------------------------------------------------------------------------------
// send one block of data for write block or write multiple blocks
int Sd2Card_writeData(char token, const char* src) {
  spiSend(token);
  for (int i = 0; i < 512; i++) {
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

int Sd2Card_writeData(const char* src) {
  // wait for previous write to finish
  if (!Sd2Card_waitNotBusy(SD_WRITE_TIMEOUT)) {
    errCode = 13;
    SPI_CHIP_SELECT_HIGH();
    return false;
  }
  return Sd2Card_writeData(WRITE_MULTIPLE_TOKEN, src);
}
//------------------------------------------------------------------------------
int Sd2Card_writeBlock(int blockNumber, const char* src) {

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

