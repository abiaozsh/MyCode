//函数指针
//变量指针
//中断向量
//浮点数

//sd硬件层用 E:\MyCode.github\arduino\softSD\simple Nofat  （带mmc）（现有的）
//文件系统用 E:\MyCode.github\fpga\mynios\inc\SD

//#define DEBUG

#include "inc/io.h"
#include "inc/system.h"

int uart_read(int timeout){
  if(timeout!=-1){
    IOWR(MYTIMER, 0 ,0);
  }
  while(1){
    if(timeout!=-1){
      int timer = IORD(MYTIMER, 0);
      if(timer>timeout){
        return -1;
      }
    }
    int tmp = IORD(MYUART, 0);
    if(tmp&0x100){
      return tmp & 0xFF;
    }
  }
}

int uart_read(){
  uart_read(-1);
}

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

int print(const char* str, int len){
  int idx = 0;
  int i;
  for(i=0;i<len;i++){
    char tmp = str[i];
    uart_write(tmp);
  }
}

int equal(const char* a,const  char* b, int maxlen){
  int i = 0;
  while(1){
    if(a[i]!=b[i]){
      return 0;
    }
    if(a[i]=='\0' && b[i]=='\0'){
      break;
    }
    if(a[i]=='\0' || b[i]=='\0'){
      return 0;
    }
    
    i++;
    if(maxlen>0){
      if(i>=maxlen){
        break;
      }
    }
  }
  return 1;
}

int printByte(int val){
  char* chardata = "0123456789ABCDEF";
  uart_write(chardata[(val>>4)&0x0F]);
  uart_write(chardata[(val)&0x0F]);
}



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
void SPI_CHIP_SELECT_LOW(int chip){IOWR(SOFTISP, SOFTISP_CS, ~(1<<chip));}

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

// End Of Chain values for FAT entries
#define FAT16EOC      0XFFFF
#define FAT16EOC_MIN  0XFFF8
#define FAT32EOC      0X0FFFFFFF
#define FAT32EOC_MIN  0X0FFFFFF8
#define FAT32MASK     0X0FFFFFFF

#define DIR_NAME_DELETED        '\xe5'
#define DIR_NAME_FREE           '\0'
#define DIR_ATT_READ_ONLY       0X01
#define DIR_ATT_HIDDEN          0X02
#define DIR_ATT_SYSTEM          0X04
#define DIR_ATT_VOLUME_ID       0X08
#define DIR_ATT_DIRECTORY       0X10
#define DIR_ATT_ARCHIVE         0X20
#define DIR_ATT_LONG_NAME_MASK  0X3F
#define DIR_ATT_DEFINED_BITS    0X3F
#define DIR_ATT_FILE_TYPE_MASK  (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY)

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
struct dir_t {//directoryEntry
  char name[11];
  char attributes;
  char reservedNT;
  char creationTimeTenths;
  char creationTime_0;
  char creationTime_1;
  char creationDate_0;
  char creationDate_1;
  char lastAccessDate_0;
  char lastAccessDate_1;
  char firstClusterHigh_0;
  char firstClusterHigh_1;
  char lastWriteTime_0;
  char lastWriteTime_1;
  char lastWriteDate_0;
  char lastWriteDate_1;
  char firstClusterLow_0;
  char firstClusterLow_1;
  char fileSize_0;
  char fileSize_1;
  char fileSize_2;
  char fileSize_3;
};
union cache_t {
  char  data[512];
  short fat16[256];
  int fat32[128];
  dir_t    dir[16];
  mbr_t    mbr;
  fbs_t    fbs;
};


struct SDcard{//partitionTable
  cache_t buff;
  int chip_select;//0,1,2
  int block;
  int inBlock;
  int offset;
  int status;
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
  int type;
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

short combineShort(char v0, char v1){
  return (v0&0x0FF) | ((v1<<8)&0x0FF00);
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
  SPI_CHIP_SELECT_LOW(sdcard->chip_select);
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
  //if (cmd == CMD8) crc = 0x87;  // correct crc for CMD8 with arg 0X1AA
  spiSend(crc);
  // wait for response
  int i;
  int Sd2Card_status;
  for (i = 0; ((Sd2Card_status = spiRec()) & 0x80) && i != 0xFF; i++);
  return Sd2Card_status;
}

int MMCCard_cardinit(SDcard* sdcard) {
  sdcard->type = 0;
  // 16-bit init start time allows over a minute
  int arg;

  // set pin modes
  SPI_CHIP_SELECT_HIGH();


  // must supply min of 74 clock cycles with CS high.
  for (int i = 0; i < 20; i++) spiSend(0XFF);

  SPI_CHIP_SELECT_LOW(sdcard->chip_select);

  // command to go idle in SPI mode
  int ok = 0;
  for(int i=0;i<10;i++){//SD_INIT_TIMEOUT*1000
    if((sdcard->status = Sd2Card_cardCommand(sdcard, CMD0, 0)) == R1_IDLE_STATE)
    {
      ok=1;
      break;
    }
  }
  if (!ok) {
    //print("err1\r\n");
    goto fail;
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
      goto fail;
  }

  SPI_CHIP_SELECT_HIGH();

  return 1;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return 0;
}


int Sd2Card_waitStartBlock(SDcard* sdcard) {
  int ok=0;
  int i;
  for(i=0;i<SD_READ_TIMEOUT*1000;i++){
    if((sdcard->status = spiRec()) == 0XFF) {
    }
    else
    {
      ok=1;
      break;
    }
  }
  if (!ok) {
    //print("err3\r\n");
    goto fail;
  }

  if (sdcard->status != DATA_START_BLOCK) {
    //print("err4\r\n");
    goto fail;
  }
  return 1;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return 0;
}

void Sd2Card_readEnd(SDcard* sdcard) {
  if (sdcard->inBlock) {
      // skip data and crc
    while (sdcard->offset++ < 514) spiRec();
    SPI_CHIP_SELECT_HIGH();
    sdcard->inBlock = 0;
  }
}
int Sd2Card_readData(SDcard* sdcard, int block) {
 // use address if not SDHC card
  if (sdcard->type != SD_CARD_TYPE_SDHC) block <<= 9;
  int result = Sd2Card_cardCommand(sdcard, CMD17, block);
   if (result) {
    //print("err5\r\n");
    goto fail;
  }

  if (!Sd2Card_waitStartBlock(sdcard)) {
    goto fail;
  }
  // transfer data
  int i;
  for (i = 0; i < 512; i++) {
    sdcard->buff.data[i] = spiRec();
  }
  
  spiRec();//CRC
  spiRec();

  SPI_CHIP_SELECT_HIGH();


  return 1;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return 0;
}

int Sd2Card_readData(SDcard* sdcard, int block, int offset, int count, char* dst) {
  if (count == 0) return 1;
  if ((count + offset) > 512) {
    goto fail;
  }
  if (!sdcard->inBlock || block != sdcard->block || offset < sdcard->offset) {
    sdcard->block = block;
    // use address if not SDHC card
    if (sdcard->type != SD_CARD_TYPE_SDHC) block <<= 9;
    if (Sd2Card_cardCommand(sdcard, CMD17, block)) {
      //print("err6\r\n");
      goto fail;
    }
    if (!Sd2Card_waitStartBlock(sdcard)) {
      goto fail;
    }
    sdcard->offset = 0;
    sdcard->inBlock = 1;
  }


  // skip data before offset
  for (;sdcard->offset < offset; sdcard->offset++) {
    spiRec();
  }
  // transfer data
  for (int i = 0; i < count; i++) {
    dst[i] = spiRec();
  }

  sdcard->offset += count;
  Sd2Card_readEnd(sdcard);
  return 1;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return 0;
}


int Sd2Card_readBlock(SDcard* sdcard, int block, char* dst) {
  return Sd2Card_readData(sdcard, block, 0, 512, dst);
}

int Sd2Card_writeData(SDcard* sdcard, char token) {
  spiSend(token);
  for (int i = 0; i < 512; i++) {
    spiSend(sdcard->buff.data[i]);
  }
  spiSend(0xff);  // dummy crc
  spiSend(0xff);  // dummy crc

  sdcard->status = spiRec();
  if ((sdcard->status & DATA_RES_MASK) != DATA_RES_ACCEPTED) {
    //print("err7\r\n");
    SPI_CHIP_SELECT_HIGH();
    return 0;
  }
  return 1;
}









//------------------------------------------------------------------------------------------------------------------------------------------------------------

//ok
int SdVolume_cacheRawBlock(SdVolume* sdvolume, int blockNumber) {
  if (sdvolume->cacheBlockNumber != blockNumber) {
    if (!Sd2Card_readData(sdvolume->sdcard, blockNumber)) return 0;
    sdvolume->cacheBlockNumber = blockNumber;
  }
  return 1;
}

// Fetch a FAT entry
int SdVolume_fatGet(SdVolume* sdvolume, int cluster, int* value) {
  if (cluster > (sdvolume->clusterCount + 1)) return 0;
  int lba = sdvolume->fatStartBlock;
  lba += cluster >> 8;
  if (lba != sdvolume->cacheBlockNumber) {
    if (!SdVolume_cacheRawBlock(sdvolume, lba)) return 0;
  }
  *value = sdvolume->sdcard->buff.fat16[cluster & 0XFF];
  return 1;
}


// if part == 0 assume super floppy with FAT boot sector in block zero
// if part > 0 assume mbr volume with partition table
int SdVolume_volumeinit(SdVolume* sdvolume, SDcard* sdcard) {
  sdvolume->sdcard = sdcard;
  sdvolume->cacheBlockNumber = 0XFFFFFFFF;
  sdvolume->cacheMirrorBlock = 0;
  sdvolume->allocSearchStart = 2;
  //sdvolume->fatType = 0;

  int volumeStartBlock = 0;
  if (!SdVolume_cacheRawBlock(sdvolume, volumeStartBlock)) {
    //print("err17\r\n");
    return 0;
    }
  part_t* p = &(sdvolume->sdcard->buff.mbr.part[0]);
  int totalSectors = combineInt(p->totalSectors_0, p->totalSectors_1, p->totalSectors_2, p->totalSectors_3);
  int firstSector = combineInt(p->firstSector_0, p->firstSector_1, p->firstSector_2, p->firstSector_3);
  //print("boot=");printInt(p->boot);print("\r\n");
  //print("totalSectors=");printInt(totalSectors);print("\r\n");
  //print("firstSector=");printInt(firstSector);print("\r\n");
  if ((p->boot & 0X7F) !=0 || totalSectors < 100 || firstSector == 0) {
    // not a valid partition
    {
      //print("err18\r\n");
      return 0;
    }
  }
  volumeStartBlock = firstSector;
  
  
  if (!SdVolume_cacheRawBlock(sdvolume, volumeStartBlock)) {
    //print("err19\r\n");
    return 0;
  }

  bpb_t* bpb = &(sdvolume->sdcard->buff.fbs.bpb);
  
  int bytesPerSector = combineShort(bpb->bytesPerSector_0, bpb->bytesPerSector_1);
  int reservedSectorCount = combineShort(bpb->reservedSectorCount_0, bpb->reservedSectorCount_1);
  if (bytesPerSector != 512 || bpb->fatCount == 0 || reservedSectorCount == 0 || bpb->sectorsPerCluster == 0) {
       // not valid FAT volume
      //print("bytesPerSector=");printInt(bytesPerSector);print("\r\n");
      //print("fatCount=");printInt(bpb->fatCount);print("\r\n");
      //print("reservedSectorCount=");printInt(reservedSectorCount);print("\r\n");
      //print("sectorsPerCluster=");printInt(bpb->sectorsPerCluster);print("\r\n");
      {
        //print("err20\r\n");
        return 0;
      }
  }
  sdvolume->fatCount = bpb->fatCount;
  sdvolume->blocksPerCluster = bpb->sectorsPerCluster;

  // determine shift that is same as multiply by SdVolume_blocksPerCluster_
  sdvolume->clusterSizeShift = 0;
  while (sdvolume->blocksPerCluster != (1 << sdvolume->clusterSizeShift)) {
    // error if not power of 2
    if (sdvolume->clusterSizeShift++ > 7) {
      //print("err21\r\n");
      return 0;
    }
  }
  
  int sectorsPerFat16 = combineShort(bpb->sectorsPerFat16_0, bpb->sectorsPerFat16_1);
  int sectorsPerFat32 = combineInt(bpb->sectorsPerFat32_0, bpb->sectorsPerFat32_1, bpb->sectorsPerFat32_2, bpb->sectorsPerFat32_3);
  sdvolume->blocksPerFat = sectorsPerFat16 ? sectorsPerFat16 : sectorsPerFat32;

  sdvolume->fatStartBlock = volumeStartBlock + reservedSectorCount;

  // count for FAT16 zero for FAT32
  int rootDirEntryCount = combineShort(bpb->rootDirEntryCount_0, bpb->rootDirEntryCount_1);
  sdvolume->rootDirEntryCount = rootDirEntryCount;

  // directory start for FAT16 dataStart for FAT32
  sdvolume->rootDirStart = sdvolume->fatStartBlock + bpb->fatCount * sdvolume->blocksPerFat;

  // data start for FAT16 and FAT32
  //sdvolume->dataStartBlock = sdvolume->rootDirStart + ((32 * bpb->rootDirEntryCount + 511)/512);
  sdvolume->dataStartBlock = sdvolume->rootDirStart + (((rootDirEntryCount<<5) + 511)>>9);

  // total blocks for FAT16 or FAT32
  int totalSectors16 = combineShort(bpb->totalSectors16_0, bpb->totalSectors16_1);
  int totalSectors32 = combineInt(bpb->totalSectors32_0, bpb->totalSectors32_1, bpb->totalSectors32_2, bpb->totalSectors32_3);
  int totalBlocks = totalSectors16 ? totalSectors16 : totalSectors32;
  // total data blocks
  sdvolume->clusterCount = totalBlocks - (sdvolume->dataStartBlock - volumeStartBlock);

  // divide by cluster size to get cluster count
  sdvolume->clusterCount >>= sdvolume->clusterSizeShift;

  // FAT type is determined by cluster count
  if (sdvolume->clusterCount < 4085) {
    return 0;
  } else if (sdvolume->clusterCount < 65525) {
    //sdvolume->fatType = 16;
  } else {
    return 0;
  }
  return 1;
}

int SdVolume_clusterStartBlock(SdVolume* sdvolume, int cluster) {
  return sdvolume->dataStartBlock + ((cluster - 2) << sdvolume->clusterSizeShift);
}
int  SdVolume_blockOfCluster(SdVolume* sdvolume, int position)  {
  return (position >> 9) & (sdvolume->blocksPerCluster - 1);
}

//------------------------------------------------------------------------------
// open a cached directory entry. Assumes vol_ is initializes
int File_openCachedEntry(File* file, int dirIndex) {
  // location of entry in cache
  dir_t* p = file->root->sdvolume->sdcard->buff.dir + dirIndex;

  // remember location of directory entry on SD
  file->dirIndex = dirIndex;
  file->dirBlock = file->root->sdvolume->cacheBlockNumber;

  // copy first cluster number for directory fields
  //file->firstCluster = (int)(p->firstClusterHigh) << 16;
  //file->firstCluster |= p->firstClusterLow;
  file->firstCluster = combineInt(p->firstClusterLow_0, p->firstClusterLow_1, p->firstClusterHigh_0, p->firstClusterHigh_1);

  // make sure it is a normal file or subdirectory
  //file->fileSize = p->fileSize;
  file->fileSize = combineInt(p->fileSize_0, p->fileSize_1, p->fileSize_2, p->fileSize_3);
  file->type = FAT_FILE_TYPE_NORMAL;

  // set to start of file
  file->curCluster = 0;
  file->curPosition = 0;
  return 1;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------





void Root_rewind(Root* root) {
  root->curPosition = root->curCluster = 0;
}


int Root_Load(Root* root) {

  //Root_fileSize_:16384,Root_curPosition_:0
  // max bytes left in file
  if (0 == (root->fileSize - root->curPosition)) return 0;

  // amount left to read
  int block;  // raw device block number
  int offset = root->curPosition & 0X1FF;  // offset in block
  if (root->type == FAT_FILE_TYPE_ROOT16) {
    block = root->sdvolume->rootDirStart + (root->curPosition >> 9);
  } else {
    int blockOfCluster = SdVolume_blockOfCluster(root->sdvolume, root->curPosition);
    if (offset == 0 && blockOfCluster == 0) {
      // start of new cluster
      if (root->curPosition == 0) {
        // use first cluster in file
        root->curCluster = root->firstCluster;
      } else {
        // get next cluster from FAT
        if (!SdVolume_fatGet(root->sdvolume, root->curCluster, &(root->curCluster))) return -1;
      }
    }
    block = SdVolume_clusterStartBlock(root->sdvolume, root->curCluster) + blockOfCluster;
  }

  // read block to cache and copy data to caller
  if (!SdVolume_cacheRawBlock(root->sdvolume, block)) return -1;

  return 1;
}



//------------------------------------------------------------------------------
// Read next directory entry into the cache
// Assumes file is correctly positioned
dir_t* Root_readDirCache(Root* root) {
  // error if not directory
  //if (!isDir()) return NULL;

  // index of entry in cache
  int i = (root->curPosition >> 5) & 0XF;

  // use read to locate and cache block
  if (Root_Load(root) < 0){
    //print("error12\r\n");
    return 0;
  } 

  // advance to next entry
  root->curPosition += 32;

  // return pointer to entry
  return &(root->sdvolume->sdcard->buff.dir[i]);
}



int Root_openRoot(Root* root, SdVolume* sdvolume) {
  root->sdvolume = sdvolume;
  // error if file is already open
  //if (isOpen()) return false;

  root->type = FAT_FILE_TYPE_ROOT16;
  root->firstCluster = 0;
  //root->fileSize = 32 * SdVolume_rootDirEntryCount_;
  root->fileSize = root->sdvolume->rootDirEntryCount<<5;
  
  
  // set to start of file
  root->curCluster = 0;
  root->curPosition = 0;

  // root has no directory entry
  root->dirBlock = 0;
  root->dirIndex = 0;
  
  
  dir_t* p;
  Root_rewind(root);
  // bool for empty entry found
  int emptyFound = 0;
  // search for file
  while (root->curPosition < root->fileSize) {
    int index = 0XF & (root->curPosition >> 5);
    p = Root_readDirCache(root);
    if (p == 0){
      //print("err22\r\n");
      return 0;
    }

    if(p->attributes == DIR_ATT_VOLUME_ID){
      for(int i=0;i<11;i++){
        root->volumeLabel[i] = p->name[i];
      }
      break;
    }
  }
  
  return 1;
}



int File_open(Root* root, File* file, const char* dname) {
  file->root = root;
  file->type = FAT_FILE_TYPE_CLOSED;          // type of file see above for values

  dir_t* p;

  Root_rewind(root);


  // search for file
  while (root->curPosition < root->fileSize) {
    int index = 0XF & (root->curPosition >> 5);
    p = Root_readDirCache(root);
    if (p == 0){
      //print("err23\r\n");
      return 0;
    }
    
    //print(dname, 11);
    //print(p->name, 11);
    //print("\r\n");
    if (p->name[0] == DIR_NAME_FREE) break;
    int eq = equal(dname, p->name, 11);
    if(eq) {
      // open found file
      //print("found \r\n");
      return File_openCachedEntry(file, 0XF & index);
    }
  }
  //print("notfound \r\n");
  return 0;
}


int File_read(File* file, char* buf, int nbyte) {
  char* dst = buf;
  
  // max bytes left in file
  if (nbyte > (file->fileSize - file->curPosition)){
    nbyte = file->fileSize - file->curPosition;
  }
  
  // amount left to read
  int toRead = nbyte;
  while (toRead > 0) {
    int block;  // raw device block number
    int offset = file->curPosition & 0X1FF;  // offset in block
    if (file->type == FAT_FILE_TYPE_ROOT16) {
      block = file->root->sdvolume->rootDirStart + (file->curPosition >> 9);
    } else {
      int blockOfCluster = SdVolume_blockOfCluster(file->root->sdvolume, file->curPosition);
      if (offset == 0 && blockOfCluster == 0) {
        // start of new cluster
        if (file->curPosition == 0) {
          // use first cluster in file
          file->curCluster = file->firstCluster;
        } else {
          // get next cluster from FAT
          if (!SdVolume_fatGet(file->root->sdvolume, file->curCluster, &(file->curCluster))) return -1;
        }
      }
      block = SdVolume_clusterStartBlock(file->root->sdvolume, file->curCluster) + blockOfCluster;
    }
    int n = toRead;

    // amount to be read from current block
    if (n > (512 - offset)) n = 512 - offset;

    // no buffering needed if n == 512 or user requests no buffering
    if ((n == 512) && block != file->root->sdvolume->cacheBlockNumber) {
      if (!Sd2Card_readData(file->root->sdvolume->sdcard, block, offset, n, dst)) return -1;
      dst += n;
    } else {
      // read block to cache and copy data to caller
      if (!SdVolume_cacheRawBlock(file->root->sdvolume, block)) return -1;
      char* src = file->root->sdvolume->sdcard->buff.data + offset;
      char* end = src + n;
      while (src != end) *dst++ = *src++;
    }
    file->curPosition += n;
    toRead -= n;
  }
  return nbyte;
}

int File_read(File* file) {
  char b;
  return File_read(file, &b, 1) == 1 ? b : -1;
}



int main()
{
  
  SDcard* sdcard;
  SdVolume* sdvolume;
  Root* root;
  File* file;
  
  sdcard = (SDcard*)(0x1000);//at sdram [512]
  sdvolume = (SdVolume*)(0x2000);
  root = (Root*)(0x3000);
  file = (File*)(0x4000);
  
	print("INIT\r\n");
  sdcard->chip_select = 2;
  int result = MMCCard_cardinit(sdcard);
  if(result){
    print("SD ok\r\n");
    result = SdVolume_volumeinit(sdvolume, sdcard);
    if(result){
      print("vol ok!\r\n");
      if(result){
        Root_openRoot(root, sdvolume);
        print("root ok!\r\n");
        print(root->volumeLabel,11);
        int result = File_open(root, file, "BOOT    BIN");
        if(result){
          print("[");
          int i;
          while(1){
            int data = File_read(file);
            if(data==-1){
              break;
            }
            printByte(data);
          }
          print("]");
        }
      }
    }
  }



  
	while(1){
	}
  return 0;
}
