//函数指针
//变量指针
//中断向量
//浮点数

//sd硬件层用 E:\MyCode.github\arduino\softSD\simple Nofat  （带mmc）（现有的）
//文件系统用 E:\MyCode.github\fpga\mynios\inc\SD

//#define DEBUG

#include "inc/io.h"
#include "inc/system.h"

int uart_write(int val){
  while((IORD(MYUART, 1)) & 0x100);
  IOWR(MYUART, 1 ,val);
}


int print(const char* str){
  int idx = 0;
  while(1){
    char tmp = str[idx];
    if(tmp=='\0')break;
    uart_write(tmp);
    idx++;
  }
}
//
//int print(const char* str, int len){
//  int idx = 0;
//  int i;
//  for(i=0;i<len;i++){
//    char tmp = str[i];
//    uart_write(tmp);
//  }
//}
//
//int printByte(int val){
//  char* chardata = "0123456789ABCDEF";
//  uart_write(chardata[(val>>4)&0x0F]);
//  uart_write(chardata[(val)&0x0F]);
//}
//


inline void _dly()
{
  IOWR(MYTIMER, 0, 0);
  while(1)
  {
	  int time = IORD(MYTIMER, 0);
	  if(time > 20){
		  break;
	  }
  }
}


void SPI_CHIP_SELECT_HIGH(){IOWR(SOFTISP, SOFTISP_CS, 0x07);}

//------------------------------------------------------------------------------
int spiRec(void) {
  int data = 0;
  // no interrupts during byte receive - about 8 us
  //cli();
  // output pin high - like sending 0XFF
  IOWR(SOFTISP, SOFTISP_MOSI, 1);
  int i;
  for (i = 0; i < 8; i++) {
    IOWR(SOFTISP, SOFTISP_SCK, 1);
    _dly();
    data <<= 1;
    
    if (IORD(SOFTISP, SOFTISP_MISO)) data |= 1;

    IOWR(SOFTISP, SOFTISP_SCK, 0);
    _dly();
  }
  // enable interrupts
  //sei();
  #ifdef DEBUG
  uart_write('[');
  uart_write('r');
  printByte(data);
  uart_write(']');
  #endif
  return data;
}
//------------------------------------------------------------------------------
void spiSend(int data) {
  #ifdef DEBUG
  uart_write('[');
  uart_write('s');
  printByte(data);
  uart_write(']');
  #endif
  // no interrupts during byte send - about 8 us
  //cli();
  int i;
  for (i = 0; i < 8; i++) {
    IOWR(SOFTISP, SOFTISP_SCK, 0);
    _dly();
    if(data & 0x80)
    {
      IOWR(SOFTISP, SOFTISP_MOSI, 1);
    }
    else
    {
      IOWR(SOFTISP, SOFTISP_MOSI, 0);
    }
    data <<= 1;
    IOWR(SOFTISP, SOFTISP_SCK, 1);
    _dly();
  }
  IOWR(SOFTISP, SOFTISP_SCK, 0);
  // enable interrupts
  //sei();
}



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


#define FAT_FILE_TYPE_CLOSED  0
#define FAT_FILE_TYPE_NORMAL  1
#define FAT_FILE_TYPE_ROOT16  2
#define FAT_FILE_TYPE_ROOT32  3
#define FAT_FILE_TYPE_MIN_DIR  FAT_FILE_TYPE_ROOT16

struct part_t {//partitionTable
  char boot;
  char beginHead;
  char beginSector_beginCylinderHigh;//6:2
  char beginCylinderLow;
  char type;
  char endHead;
  char endSector_endCylinderHigh;//6:2
  char endCylinderLow;
  char firstSector_0;
  char firstSector_1;
  char firstSector_2;
  char firstSector_3;
  char totalSectors_0;
  char totalSectors_1;
  char totalSectors_2;
  char totalSectors_3;
};
struct mbr_t {//masterBootRecord
  char codeArea[440];
  char diskSignature_0;
  char diskSignature_1;
  char diskSignature_2;
  char diskSignature_3;
  char usuallyZero_0;
  char usuallyZero_1;
  part_t   part[4];
  char  mbrSig0;
  char  mbrSig1;
};
struct bpb_t {//biosParmBlock
  char bytesPerSector_0;
  char bytesPerSector_1;
  char sectorsPerCluster;
  char reservedSectorCount_0;
  char reservedSectorCount_1;
  char fatCount;
  char rootDirEntryCount_0;
  char rootDirEntryCount_1;
  char totalSectors16_0;
  char totalSectors16_1;
  char mediaType;
  char sectorsPerFat16_0;
  char sectorsPerFat16_1;
  char sectorsPerTrtack_0;
  char sectorsPerTrtack_1;
  char headCount_0;
  char headCount_1;
  char hidddenSectors_0;
  char hidddenSectors_1;
  char hidddenSectors_2;
  char hidddenSectors_3;
  char totalSectors32_0;
  char totalSectors32_1;
  char totalSectors32_2;
  char totalSectors32_3;
  char sectorsPerFat32_0;
  char sectorsPerFat32_1;
  char sectorsPerFat32_2;
  char sectorsPerFat32_3;
  char fat32Flags_0;
  char fat32Flags_1;
  char fat32Version_0;
  char fat32Version_1;
  char fat32RootCluster_0;
  char fat32RootCluster_1;
  char fat32RootCluster_2;
  char fat32RootCluster_3;
  char fat32FSInfo_0;
  char fat32FSInfo_1;
  char fat32BackBootBlock_0;
  char fat32BackBootBlock_1;
  char fat32Reserved[12];
};
struct fbs_t {//fat32BootSector
  char jmpToBootCode[3];
  char oemName[8];
  bpb_t bpb;
  char driveNumber;
  char reserved1;
  char bootSignature;
  char volumeSerialNumber_0;
  char volumeSerialNumber_1;
  char volumeSerialNumber_2;
  char volumeSerialNumber_3;
  char volumeLabel[11];
  char fileSystemType[8];
  char bootCode[420];
  char bootSectorSig0;
  char bootSectorSig1;
};

union cache_t {
  char  data[512];
  short fat16[256];
  int fat32[128];
  //dir_t    dir[16];
  mbr_t    mbr;
  fbs_t    fbs;
};


struct SDcard{//partitionTable
  cache_t buff;
  int chip_select;//0,1,2
  int block;
  int inBlock;
  int offset;
  int type;
};


struct SdVolume{
  SDcard* sdcard;
  int cacheDirty;// cacheFlush() will write block if true
  int cacheBlockNumber;  // Logical number of block in the cache
  int cacheMirrorBlock;  // block number for mirror FAT
  int fatCount;            // number of FATs on volume
  int blocksPerCluster;    // cluster size in blocks
  int allocSearchStart;   // start cluster for alloc search
  int blocksPerFat;       // FAT size in blocks
  int clusterCount;       // clusters in one FAT
  int clusterSizeShift;    // shift to convert cluster count to block count
  int dataStartBlock;     // first data block number
  int fatStartBlock;      // start block for first FAT
  int fatType;             // volume type (12, 16, OR 32)
  int rootDirEntryCount;  // number of entries in FAT16 root dir
  int rootDirStart;       // root start block for FAT16, cluster for FAT32

};


struct Root{
  SdVolume* sdvolume;
  char volumeLabel[11];
  int firstCluster;
  int fileSize;
  int curCluster;
  int curPosition;
  int dirBlock;
  int dirIndex;
};


struct File{
  Root* root;
  int flags;         // is F_FILE_DIR_DIRTY
  int type;          // type of file see above for values
  int curCluster;    // cluster for current file position
  int curPosition;   // current file position in bytes from beginning
  int dirBlock;      // SD block that contains directory entry for file
  int dirIndex;      // index of entry in dirBlock 0 <= dirIndex_ <= 0XF
  int fileSize;      // file size in bytes
  int firstCluster;  // first cluster of file
};


int combineInt(char v0, char v1, char v2, char v3){
  return (v0&0x0FF) | ((v1<<8)&0x0FF00) | ((v2<<16)&0x0FF0000) | ((v3<<24)&0x0FF000000);
}

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

int Sd2Card_cardCommand(SDcard* sdcard, int cmd, int arg) {
  // select card
  //SPI_CHIP_SELECT_LOW(sdcard->chip_select);
  IOWR(SOFTISP, SOFTISP_CS, ~(1<<sdcard->chip_select));
  // wait up to 300 ms if busy
  Sd2Card_waitNotBusy(3000000);
  // send command
  spiSend(cmd | 0x40);
  // send argument
  for (int s = 24; s >= 0; s -= 8) spiSend(arg >> s);
  // send CRC
  int crc = 0xFF;
  if (cmd == CMD0) crc = 0x95;  // correct crc for CMD0 with arg 0
  //if (cmd == CMD8) crc = 0x87;  // correct crc for CMD8 with arg 0X1AA
  spiSend(crc);
  // wait for response
  int i;
  int Sd2Card_status;
  for (i = 0; ((Sd2Card_status = spiRec()) & 0x80) && i != 0xFF; i++);
  return Sd2Card_status;
}

int MMCCard_cardinit(SDcard* sdcard) {
  //sdcard->type = 0;
  // 16-bit init start time allows over a minute
  //int arg;

  // set pin modes
  //SPI_CHIP_SELECT_HIGH();


  // must supply min of 74 clock cycles with CS high.
  for (int i = 0; i < 20; i++) spiSend(0XFF);

  // command to go idle in SPI mode
  int ok = 0;
  for(int i=0;i<10;i++){//SD_INIT_TIMEOUT*1000
    if(Sd2Card_cardCommand(sdcard, CMD0, 0) == R1_IDLE_STATE)
    {
      ok=1;
      break;
    }
  }
  if (!ok) {
    //print("err1\r\n");
    return 0;
  }

  ok=0;
  for(int i=0;i<100;i++){//SD_INIT_TIMEOUT*1000
    if(Sd2Card_cardCommand(sdcard, CMD1, 0) == R1_READY_STATE) {
      ok=1;
      break;
    }
  }
  if (!ok) {
      //print("err2\r\n");
    return 0;
  }
  return 1;
}


int Sd2Card_waitStartBlock(SDcard* sdcard) {
  int ok=0;
  int i;
  int status;
  for(i=0;i<SD_READ_TIMEOUT*1000;i++){
    if((status = spiRec()) == 0XFF) {
    }
    else
    {
      ok=1;
      break;
    }
  }
  if (!ok) {
    //print("err3\r\n");
    return 0;
  }

  if (status != DATA_START_BLOCK) {
    //print("err4\r\n");
    return 0;
  }
  return 1;
}

int Sd2Card_readData(SDcard* sdcard, int block) {
 // use address if not SDHC card
  //if (sdcard->type != SD_CARD_TYPE_SDHC) 
  block <<= 9;
  int result = Sd2Card_cardCommand(sdcard, CMD17, block);
   if (result) {
    //print("err5\r\n");
    return 0;
  }

  if (!Sd2Card_waitStartBlock(sdcard)) {
    return 0;
  }
  // transfer data
  int i;
  for (i = 0; i < 512; i++) {
    sdcard->buff.data[i] = spiRec();
  }
  
  spiRec();//CRC
  spiRec();

  return 1;
}










int main()
{
  SDcard* sdcard;
  
  sdcard = (SDcard*)(0x100000);//at 1M
  char* zero = (char*)(0);
  
	print("INIT\r\n");
  sdcard->chip_select = 2;
  int result = MMCCard_cardinit(sdcard);
  if(result){
    print("SD ok\r\n");
    Sd2Card_readData(sdcard, 0);
    part_t* p = &(sdcard->buff.mbr.part[0]);
    int firstSector = combineInt(p->firstSector_0, p->firstSector_1, p->firstSector_2, p->firstSector_3);
    
    Sd2Card_readData(sdcard, firstSector);
    int sign = combineInt(sdcard->buff.fbs.bootCode[0], sdcard->buff.fbs.bootCode[1], sdcard->buff.fbs.bootCode[2], sdcard->buff.fbs.bootCode[3]);
    if(sign==0x78563412){
      print("sign ok\r\n");
      int firstBlock = combineInt(sdcard->buff.fbs.bootCode[4], sdcard->buff.fbs.bootCode[5], sdcard->buff.fbs.bootCode[6], sdcard->buff.fbs.bootCode[7]);
      int blockCount = combineInt(sdcard->buff.fbs.bootCode[8], sdcard->buff.fbs.bootCode[9], sdcard->buff.fbs.bootCode[10], sdcard->buff.fbs.bootCode[11]);
      firstBlock = firstBlock + firstSector;
      for(int b = 0;b<blockCount;b++){
        Sd2Card_readData(sdcard, firstBlock + b);
        for(int i=0;i<512;i++){
          char data = sdcard->buff.data[i];
          //printByte(data);
          zero[(b<<9)+i] = data;
        }
      }
      SPI_CHIP_SELECT_HIGH();
      asm volatile("jmpi 0");
    }
    SPI_CHIP_SELECT_HIGH();
    while(1);
  }



  SPI_CHIP_SELECT_HIGH();
	while(1);
  return 0;
}
