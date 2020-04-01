//函数指针
//变量指针
//中断向量
//浮点数
#include "inc/io.h"
#include "inc/system.h"
#include "inc/struct.h"
#include "inc/uart.c"


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
  uart_write(chardata[(val>>4)&0x0F]);
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
// send one block of data for write block or write multiple blocks
int Sd2Card_writeData(uint8_t token, const char* src) {
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
uint8_t Sd2Card_writeBlock(uint32_t blockNumber, const uint8_t* src) {

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











cache_t  SdVolume_cacheBuffer_;        // 512 byte cache for device blocks
int SdVolume_cacheBlockNumber_ = 0XFFFFFFFF;  // Logical number of block in the cache
int SdVolume_cacheDirty_ = 0;         // cacheFlush() will write block if true
int SdVolume_cacheMirrorBlock_ = 0;  // block number for mirror FAT
//
int SdVolume_allocSearchStart_ = 2;   // start cluster for alloc search
int SdVolume_blocksPerCluster_;    // cluster size in blocks
int SdVolume_blocksPerFat_;       // FAT size in blocks
int SdVolume_clusterCount_;       // clusters in one FAT
int SdVolume_clusterSizeShift_;    // shift to convert cluster count to block count
int SdVolume_dataStartBlock_;     // first data block number
int SdVolume_fatCount_;            // number of FATs on volume
int SdVolume_fatStartBlock_;      // start block for first FAT
int SdVolume_fatType_ = 0;             // volume type (12, 16, OR 32)
int SdVolume_rootDirEntryCount_;  // number of entries in FAT16 root dir
int SdVolume_rootDirStart_;       // root start block for FAT16, cluster for FAT32

void SdVolume_cacheSetDirty(void) {
  SdVolume_cacheDirty_ |= CACHE_FOR_WRITE;
}
uint8_t SdVolume_cacheZeroBlock(uint32_t blockNumber) {
  if (!SdVolume_cacheFlush()) return false;

  // loop take less flash than memset(SdVolume_cacheBuffer_.data, 0, 512);
  for (uint16_t i = 0; i < 512; i++) {
    SdVolume_cacheBuffer_.data[i] = 0;
  }
  SdVolume_cacheBlockNumber_ = blockNumber;
  SdVolume_cacheSetDirty();
  return true;
}
uint8_t SdVolume_cacheFlush(void) {
  if (SdVolume_cacheDirty_) {
    if (!Sd2Card_writeBlock(SdVolume_cacheBlockNumber_, SdVolume_cacheBuffer_.data)) {
      return false;
    }
    // mirror FAT tables
    if (SdVolume_cacheMirrorBlock_) {
      if (!Sd2Card_writeBlock(SdVolume_cacheMirrorBlock_, SdVolume_cacheBuffer_.data)) {
        return false;
      }
      SdVolume_cacheMirrorBlock_ = 0;
    }
    SdVolume_cacheDirty_ = 0;
  }
  return true;
}

//------------------------------------------------------------------------------
uint8_t SdVolume_cacheRawBlock(uint32_t blockNumber, uint8_t action) {
  if (SdVolume_cacheBlockNumber_ != blockNumber) {
    if (!SdVolume_cacheFlush()) return false;
    if (!Sd2Card_readBlock(blockNumber, SdVolume_cacheBuffer_.data)) return false;
    SdVolume_cacheBlockNumber_ = blockNumber;
  }
  SdVolume_cacheDirty_ |= action;
  return true;
}

uint8_t SdVolume_volumeinit(uint8_t part) {
  uint32_t volumeStartBlock = 0;
  // if part == 0 assume super floppy with FAT boot sector in block zero
  // if part > 0 assume mbr volume with partition table
  if (part) {
    if (!SdVolume_cacheRawBlock(volumeStartBlock, CACHE_FOR_READ)) return false;
    part_t* p = &SdVolume_cacheBuffer_.mbr.part[part-1];
    if ((p->boot & 0X7F) !=0  ||
      p->totalSectors < 100 ||
      p->firstSector == 0) {
      // not a valid partition
      return false;
    }
    volumeStartBlock = p->firstSector;
  }
  if (!SdVolume_cacheRawBlock(volumeStartBlock, CACHE_FOR_READ)) return false;
  bpb_t* bpb = &SdVolume_cacheBuffer_.fbs.bpb;
  if (bpb->bytesPerSector != 512 ||
    bpb->fatCount == 0 ||
    bpb->reservedSectorCount == 0 ||
    bpb->sectorsPerCluster == 0) {
       // not valid FAT volume
      return false;
  }
  SdVolume_fatCount_ = bpb->fatCount;
  SdVolume_blocksPerCluster_ = bpb->sectorsPerCluster;

  // determine shift that is same as multiply by SdVolume_blocksPerCluster_
  SdVolume_clusterSizeShift_ = 0;
  while (SdVolume_blocksPerCluster_ != (1 << SdVolume_clusterSizeShift_)) {
    // error if not power of 2
    if (SdVolume_clusterSizeShift_++ > 7) return false;
  }
  SdVolume_blocksPerFat_ = bpb->sectorsPerFat16 ? bpb->sectorsPerFat16 : bpb->sectorsPerFat32;

  SdVolume_fatStartBlock_ = volumeStartBlock + bpb->reservedSectorCount;

  // count for FAT16 zero for FAT32
  SdVolume_rootDirEntryCount_ = bpb->rootDirEntryCount;

  // directory start for FAT16 dataStart for FAT32
  SdVolume_rootDirStart_ = SdVolume_fatStartBlock_ + bpb->fatCount * SdVolume_blocksPerFat_;

  // data start for FAT16 and FAT32
  SdVolume_dataStartBlock_ = SdVolume_rootDirStart_ + ((32 * bpb->rootDirEntryCount + 511)/512);

  // total blocks for FAT16 or FAT32
  uint32_t totalBlocks = bpb->totalSectors16 ? bpb->totalSectors16 : bpb->totalSectors32;
  // total data blocks
  SdVolume_clusterCount_ = totalBlocks - (SdVolume_dataStartBlock_ - volumeStartBlock);

  // divide by cluster size to get cluster count
  SdVolume_clusterCount_ >>= SdVolume_clusterSizeShift_;

  // FAT type is determined by cluster count
  if (SdVolume_clusterCount_ < 4085) {
    SdVolume_fatType_ = 12;
  } else if (SdVolume_clusterCount_ < 65525) {
    SdVolume_fatType_ = 16;
  } else {
    SdVolume_rootDirStart_ = bpb->fat32RootCluster;
    SdVolume_fatType_ = 32;
  }
  return true;
}
//------------------------------------------------------------------------------
// Fetch a FAT entry
uint8_t SdVolume_fatGet(uint32_t cluster, uint32_t* value) {
  if (cluster > (SdVolume_clusterCount_ + 1)) return false;
  uint32_t lba = SdVolume_fatStartBlock_;
  lba += SdVolume_fatType_ == 16 ? cluster >> 8 : cluster >> 7;
  if (lba != SdVolume_cacheBlockNumber_) {
    if (!SdVolume_cacheRawBlock(lba, CACHE_FOR_READ)) return false;
  }
  if (SdVolume_fatType_ == 16) {
    *value = SdVolume_cacheBuffer_.fat16[cluster & 0XFF];
  } else {
    *value = SdVolume_cacheBuffer_.fat32[cluster & 0X7F] & FAT32MASK;
  }
  return true;
}

//------------------------------------------------------------------------------
// Store a FAT entry
uint8_t SdVolume_fatPut(uint32_t cluster, uint32_t value) {
  // error if reserved cluster
  if (cluster < 2) return false;

  // error if not in FAT
  if (cluster > (SdVolume_clusterCount_ + 1)) return false;

  // calculate block address for entry
  uint32_t lba = SdVolume_fatStartBlock_;
  lba += SdVolume_fatType_ == 16 ? cluster >> 8 : cluster >> 7;

  if (lba != SdVolume_cacheBlockNumber_) {
    if (!SdVolume_cacheRawBlock(lba, CACHE_FOR_READ)) return false;
  }
  // store entry
  if (SdVolume_fatType_ == 16) {
    SdVolume_cacheBuffer_.fat16[cluster & 0XFF] = value;
  } else {
    SdVolume_cacheBuffer_.fat32[cluster & 0X7F] = value;
  }
  SdVolume_cacheSetDirty();

  // mirror second FAT
  if (SdVolume_fatCount_ > 1) SdVolume_cacheMirrorBlock_ = lba + SdVolume_blocksPerFat_;
  return true;
}

uint8_t SdVolume_fatPutEOC(uint32_t cluster) {
  return SdVolume_fatPut(cluster, 0x0FFFFFFF);
}

// find a contiguous group of clusters
uint8_t SdVolume_allocContiguous(uint32_t* curCluster) {
  // start of group
  uint32_t bgnCluster;

  // flag to save place to start next search
  uint8_t setStart;

  // set search start cluster
  if (*curCluster) {
    // try to make file contiguous
    bgnCluster = *curCluster + 1;

    // don't save new start location
    setStart = false;
  } else {
    // start at likely place for free cluster
    bgnCluster = SdVolume_allocSearchStart_;

    // save next search start if one cluster
    setStart = true;
  }
  // end of group
  uint32_t endCluster = bgnCluster;

  // last cluster of FAT
  uint32_t fatEnd = SdVolume_clusterCount_ + 1;

  // search the FAT for free clusters
  for (uint32_t n = 0;; n++, endCluster++) {
    // can't find space checked all clusters
    if (n >= SdVolume_clusterCount_) return false;

    // past end - start from beginning of FAT
    if (endCluster > fatEnd) {
      bgnCluster = endCluster = 2;
    }
    uint32_t f;
    if (!SdVolume_fatGet(endCluster, &f)) return false;

    if (f != 0) {
      // cluster in use try next cluster as bgnCluster
      bgnCluster = endCluster + 1;
    } else if (endCluster == bgnCluster) {
      // done - found space
      break;
    }
  }
  // mark end of chain
  if (!SdVolume_fatPutEOC(endCluster)) return false;

  // link clusters
  while (endCluster > bgnCluster) {
    if (!SdVolume_fatPut(endCluster - 1, endCluster)) return false;
    endCluster--;
  }
  if (*curCluster != 0) {
    // connect chains
    if (!SdVolume_fatPut(*curCluster, bgnCluster)) return false;
  }
  // return first cluster number to caller
  *curCluster = bgnCluster;

  // remember possible next free cluster
  if (setStart) SdVolume_allocSearchStart_ = bgnCluster + 1;

  return true;
}

uint8_t  SdVolume_isEOC(uint32_t cluster)  {return  cluster >= (SdVolume_fatType_ == 16 ? FAT16EOC_MIN : FAT32EOC_MIN);}

//------------------------------------------------------------------------------
// return the size in bytes of a cluster chain
uint8_t SdVolume_chainSize(uint32_t cluster, uint32_t* size) {
  uint32_t s = 0;
  do {
    if (!SdVolume_fatGet(cluster, &cluster)) return false;
    s += 512UL << SdVolume_clusterSizeShift_;
  } while (!SdVolume_isEOC(cluster));
  *size = s;
  return true;
}

uint8_t  SdVolume_blockOfCluster(uint32_t position)  {return (position >> 9) & (SdVolume_blocksPerCluster_ - 1);}
uint32_t SdVolume_clusterStartBlock(uint32_t cluster)  {return SdVolume_dataStartBlock_ + ((cluster - 2) << SdVolume_clusterSizeShift_);}













uint8_t   Root_type_;
uint32_t  Root_firstCluster_;
uint32_t  Root_fileSize_;
uint32_t  Root_curCluster_;
uint32_t  Root_curPosition_;
uint32_t  Root_dirBlock_;
uint8_t   Root_dirIndex_;

uint8_t Root_openRoot() {
  // error if file is already open
  //if (isOpen()) return false;

  if (SdVolume_fatType_ == 16) {
    Root_type_ = FAT_FILE_TYPE_ROOT16;
    Root_firstCluster_ = 0;
    Root_fileSize_ = 32 * SdVolume_rootDirEntryCount_;
  } else if (SdVolume_fatType_ == 32) {
    Root_type_ = FAT_FILE_TYPE_ROOT32;
    Root_firstCluster_ = SdVolume_rootDirStart_;
    if (!SdVolume_chainSize(Root_firstCluster_, &Root_fileSize_)) return false;
  } else {
    // volume is not initialized or FAT12
    return false;
  }

  // set to start of file
  Root_curCluster_ = 0;
  Root_curPosition_ = 0;

  // root has no directory entry
  Root_dirBlock_ = 0;
  Root_dirIndex_ = 0;
  return true;
}

void Root_rewind() {
  Root_curPosition_ = Root_curCluster_ = 0;
}

int16_t Root_Load() {

  //Root_fileSize_:16384,Root_curPosition_:0
  // max bytes left in file
  if (0 == (Root_fileSize_ - Root_curPosition_)) return 0;

  // amount left to read
  uint32_t block;  // raw device block number
  uint16_t offset = Root_curPosition_ & 0X1FF;  // offset in block
  if (Root_type_ == FAT_FILE_TYPE_ROOT16) {
    block = SdVolume_rootDirStart_ + (Root_curPosition_ >> 9);
  } else {
    uint8_t blockOfCluster = SdVolume_blockOfCluster(Root_curPosition_);
    if (offset == 0 && blockOfCluster == 0) {
      // start of new cluster
      if (Root_curPosition_ == 0) {
        // use first cluster in file
        Root_curCluster_ = Root_firstCluster_;
      } else {
        // get next cluster from FAT
        if (!SdVolume_fatGet(Root_curCluster_, &Root_curCluster_)) return -1;
      }
    }
    block = SdVolume_clusterStartBlock(Root_curCluster_) + blockOfCluster;
  }

  // read block to cache and copy data to caller
  if (!SdVolume_cacheRawBlock(block, CACHE_FOR_READ)) return -1;

  return 1;
}

//------------------------------------------------------------------------------
// Read next directory entry into the cache
// Assumes file is correctly positioned
dir_t* Root_readDirCache(void) {
  // error if not directory
  //if (!isDir()) return NULL;

  // index of entry in cache
  uint8_t i = (Root_curPosition_ >> 5) & 0XF;

  // use read to locate and cache block
  if (Root_Load() < 0) return NULL;

  // advance to next entry
  Root_curPosition_ += 32;

  // return pointer to entry
  return (SdVolume_cacheBuffer_.dir + i);
}


// private data
uint8_t   File_flags_;         // is F_FILE_DIR_DIRTY
uint8_t   File_type_ = FAT_FILE_TYPE_CLOSED;          // type of file see above for values
uint32_t  File_curCluster_;    // cluster for current file position
uint32_t  File_curPosition_;   // current file position in bytes from beginning
uint32_t  File_dirBlock_;      // SD block that contains directory entry for file
uint8_t   File_dirIndex_;      // index of entry in dirBlock 0 <= dirIndex_ <= 0XF
uint32_t  File_fileSize_;      // file size in bytes
uint32_t  File_firstCluster_;  // first cluster of file

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// open a cached directory entry. Assumes vol_ is initializes
uint8_t File_openCachedEntry(uint8_t dirIndex) {
  // location of entry in cache
  dir_t* p = SdVolume_cacheBuffer_.dir + dirIndex;

  // remember location of directory entry on SD
  File_dirIndex_ = dirIndex;
  File_dirBlock_ = SdVolume_cacheBlockNumber_;

  // copy first cluster number for directory fields
  File_firstCluster_ = (uint32_t)p->firstClusterHigh << 16;
  File_firstCluster_ |= p->firstClusterLow;

  // make sure it is a normal file or subdirectory
  File_fileSize_ = p->fileSize;
  File_type_ = FAT_FILE_TYPE_NORMAL;

  // set to start of file
  File_curCluster_ = 0;
  File_curPosition_ = 0;
  return true;
}

dir_t* File_cacheDirEntry(uint8_t action) {
  if (!SdVolume_cacheRawBlock(File_dirBlock_, action)) return NULL;
  return SdVolume_cacheBuffer_.dir + File_dirIndex_;
}
//------------------------------------------------------------------------------
/**
 * Sets a file's position.
 *
 * \param[in] pos The new position in bytes from the beginning of the file.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t File_seekSet(uint32_t pos) {
  // error if file not open or seek past end of file
  if (pos > File_fileSize_){errCode = 80;  return false;}//!isOpen() || 

  if (File_type_ == FAT_FILE_TYPE_ROOT16) {
    File_curPosition_ = pos;
    return true;
  }
  if (pos == 0) {
    // set position to start of file
    File_curCluster_ = 0;
    File_curPosition_ = 0;
    return true;
  }
  // calculate cluster index for cur and new position
  uint32_t nCur = (File_curPosition_ - 1) >> (SdVolume_clusterSizeShift_ + 9);
  uint32_t nNew = (pos - 1) >> (SdVolume_clusterSizeShift_ + 9);

  if (nNew < nCur || File_curPosition_ == 0) {
    // must follow chain from first cluster
    File_curCluster_ = File_firstCluster_;
  } else {
    // advance from curPosition
    nNew -= nCur;
  }
  while (nNew--) {
    if (!SdVolume_fatGet(File_curCluster_, &File_curCluster_)){errCode = 81;  return false;}
  }
  File_curPosition_ = pos;
  return true;
}

// 1 for append to end , 0 for read/write from start
uint8_t File_open(const char *filepath, uint8_t toend) {
  if (!File_open(filepath)) {
    return 0;
  }
  if (toend) 
    File_seekSet(File_fileSize_);
  return 1;
}

uint8_t File_open(const char* dname) {
  dir_t* p;

  Root_rewind();

  // bool for empty entry found
  uint8_t emptyFound = false;

  // search for file
  while (Root_curPosition_ < Root_fileSize_) {
    uint8_t index = 0XF & (Root_curPosition_ >> 5);
    p = Root_readDirCache();
    if (p == NULL){errCode = 103;return false;}

    if (p->name[0] == DIR_NAME_FREE || p->name[0] == DIR_NAME_DELETED) {
      // remember first empty slot
      if (!emptyFound) {
        emptyFound = true;
        File_dirIndex_ = index;
        File_dirBlock_ = SdVolume_cacheBlockNumber_;
      }
      // done if no entries follow
      if (p->name[0] == DIR_NAME_FREE) break;
    } else if (!memcmp(dname, p->name, 11)) {

      // open found file
      return File_openCachedEntry(0XF & index);
    }
  }

  // cache found slot or add cluster if end of file
  if (emptyFound) {
    p = File_cacheDirEntry(CACHE_FOR_WRITE);
    if (!p){errCode = 106; return false;}
  } else {
    //512/32=16 max 16file
    errCode = 107;
    return false;
  }
  // initialize as empty file
  memset(p, 0, sizeof(dir_t));
  memcpy(p->name, dname, 11);

  // force write of entry to SD
  if (!SdVolume_cacheFlush()){errCode = 109;  return false;}

  // open entry in cache
  return File_openCachedEntry(File_dirIndex_);
}

uint8_t File_sync(void) {
  // only allow open files and directories
  //if (!isOpen()) return false;

  if (File_flags_) {
    dir_t* d = File_cacheDirEntry(CACHE_FOR_WRITE);
    if (!d) return false;

    // do not set filesize for dir files
    d->fileSize = File_fileSize_;//if (!isDir()) 

    // update first cluster fields
    d->firstClusterLow = File_firstCluster_ & 0XFFFF;
    d->firstClusterHigh = File_firstCluster_ >> 16;

    // clear directory dirty
    File_flags_ = 0;
  }
  return SdVolume_cacheFlush();
}

uint8_t File_close(void) {
  if (!File_sync())return false;
  File_type_ = FAT_FILE_TYPE_CLOSED;
  return true;
}

//------------------------------------------------------------------------------
// add a cluster to a file
uint8_t File_addCluster() {
  if (!SdVolume_allocContiguous(&File_curCluster_)) return false;

  // if first cluster of file link to directory entry
  if (File_firstCluster_ == 0) {
    File_firstCluster_ = File_curCluster_;
    File_flags_ = 1;
  }
  return true;
}


int16_t File_read(uint8_t* buf, uint16_t nbyte) {
  uint8_t* dst = buf;

  // max bytes left in file
  if (nbyte > (File_fileSize_ - File_curPosition_)) nbyte = File_fileSize_ - File_curPosition_;

  // amount left to read
  uint16_t toRead = nbyte;
  while (toRead > 0) {
    uint32_t block;  // raw device block number
    uint16_t offset = File_curPosition_ & 0X1FF;  // offset in block
    if (File_type_ == FAT_FILE_TYPE_ROOT16) {
      block = SdVolume_rootDirStart_ + (File_curPosition_ >> 9);
    } else {
      uint8_t blockOfCluster = SdVolume_blockOfCluster(File_curPosition_);
      if (offset == 0 && blockOfCluster == 0) {
        // start of new cluster
        if (File_curPosition_ == 0) {
          // use first cluster in file
          File_curCluster_ = File_firstCluster_;
        } else {
          // get next cluster from FAT
          if (!SdVolume_fatGet(File_curCluster_, &File_curCluster_)) return -1;
        }
      }
      block = SdVolume_clusterStartBlock(File_curCluster_) + blockOfCluster;
    }
    uint16_t n = toRead;

    // amount to be read from current block
    if (n > (512 - offset)) n = 512 - offset;

    // no buffering needed if n == 512 or user requests no buffering
    if ((n == 512) && block != SdVolume_cacheBlockNumber_) {
      if (!Sd2Card_readData(block, offset, n, dst)) return -1;
      dst += n;
    } else {
      // read block to cache and copy data to caller
      if (!SdVolume_cacheRawBlock(block, CACHE_FOR_READ)) return -1;
      uint8_t* src = SdVolume_cacheBuffer_.data + offset;
      uint8_t* end = src + n;
      while (src != end) *dst++ = *src++;
    }
    File_curPosition_ += n;
    toRead -= n;
  }
  return nbyte;
}

int16_t File_read() {
  uint8_t b;
  return File_read(&b, 1) == 1 ? b : -1;
}

size_t File_write(uint8_t* buf, uint16_t nbyte) {
  // convert void* to uint8_t*  -  must be before goto statements
  uint8_t* src = buf;

  // number of bytes left to write  -  must be before goto statements
  uint16_t nToWrite = nbyte;

  while (nToWrite > 0) {
    uint8_t blockOfCluster = SdVolume_blockOfCluster(File_curPosition_);
    uint16_t blockOffset = File_curPosition_ & 0X1FF;
    if (blockOfCluster == 0 && blockOffset == 0) {
      // start of new cluster
      if (File_curCluster_ == 0) {
        if (File_firstCluster_ == 0) {
          // allocate first cluster of file
          if (!File_addCluster())return 0;
        } else {
          File_curCluster_ = File_firstCluster_;
        }
      } else {
        uint32_t next;
        if (!SdVolume_fatGet(File_curCluster_, &next)) return false;
        if (SdVolume_isEOC(next)) {
          // add cluster if at end of chain
          if (!File_addCluster())return 0;
        } else {
          File_curCluster_ = next;
        }
      }
    }
    // max space in block
    uint16_t n = 512 - blockOffset;

    // lesser of space and amount to write
    if (n > nToWrite) n = nToWrite;

    // block for data write
    uint32_t block = SdVolume_clusterStartBlock(File_curCluster_) + blockOfCluster;
    if (n == 512) {
      // full block - don't need to use cache
      // invalidate cache if block is in cache
      if (SdVolume_cacheBlockNumber_ == block) {
        SdVolume_cacheBlockNumber_ = 0XFFFFFFFF;
      }
      if (!Sd2Card_writeBlock(block, src)) return 0;
      src += 512;
    } else {
      if (blockOffset == 0 && File_curPosition_ >= File_fileSize_) {
        // start of new block don't need to read into cache
        if (!SdVolume_cacheFlush()) return 0;
        SdVolume_cacheBlockNumber_ = block;
        SdVolume_cacheSetDirty();
      } else {
        // rewrite part of block
        if (!SdVolume_cacheRawBlock(block, CACHE_FOR_WRITE)) {
          return 0;
        }
      }
      uint8_t* dst = SdVolume_cacheBuffer_.data + blockOffset;
      uint8_t* end = dst + n;
      while (dst != end) *dst++ = *src++;
    }
    nToWrite -= n;
    File_curPosition_ += n;
  }
  if (File_curPosition_ > File_fileSize_) {
    File_fileSize_ = File_curPosition_;
    File_flags_ = 1;
  }

  return nbyte;

}

size_t File_write(uint8_t b) {
  return File_write(&b, 1);
}


uint8_t Sd2Card_begin() {
  if(!Sd2Card_cardinit())return false;
  if(!SdVolume_volumeinit(1))
  {
      if(!SdVolume_volumeinit(0))
      {
        return false;
      }
  }
  return Root_openRoot();
}



//PROGMEM prog_uint8_t datas[] = "\r\n#include <Arduino.h>\r\n#include <avr/pgmspace.h>\r\n#include <stdint.h>\r\n\r\n\r\n#include <avr/io.h>\r\n\r\nuint8_t errCode = 0;\r\n\r\ninline void _dly()\r\n{\r\n  for(uint8_t i=0;i<5;i++)//5 is good\r\n  {\r\n    volatile uint8_t t;\r\n    t=0;\r\n  }\r\n}\r\n\r\n//B4\r\nvoid SPI_MOSI_INIT(){PORTB &= ~_BV(4);_dly();}\r\nvoid SPI_MOSI_HIGH(){DDRB &= ~_BV(4);_dly();}\r\nvoid SPI_MOSI_LOW(){DDRB |= _BV(4);_dly();}\r\n\r\n//B2\r\nvoid SPI_MISO_INIT(){PORTB &= ~_BV(2);DDRB &= ~_BV(2);_dly();}\r\nuint8_t SPI_MISO_GET(){return (~PINB) & _BV(2);}\r\n\r\n//B3\r\nvoid SPI_SCK_INIT(){PORTB &= ~_BV(3);_dly();}\r\nvoid SPI_SCK_HIGH(){DDRB &= ~_BV(3);_dly();}\r\nvoid SPI_SCK_LOW(){DDRB |= _BV(3);_dly();}\r\n\r\n//C0\r\nvoid SPI_CHIP_SELECT_INIT(){PORTC &= ~_BV(0);_dly();}\r\nvoid SPI_CHIP_SELECT_HIGH()";


char filename[12];


void setup(){
  Serial.begin(115200);
  DDRD &= ~_BV(6);//pwrPIN;
  
  while (!Sd2Card_begin()) {
    Serial.println("SD not ready.");
    delay(1000);
  }
  Serial.println("SD ok.");
}


void loop()
{
    uint8_t inByte = Serial.read();
    if(inByte=='r'){
      filename[0] = 'R';
      filename[1] = 'E';
      filename[2] = 'A';
      filename[3] = 'D';
      filename[4] = 'F';
      filename[5] = 'I';
      filename[6] = 'L';
      filename[7] = 'E';
      filename[8] = 'T';
      filename[9] = 'X';
      filename[10] = 'T';
      filename[11] = '\0';
      Serial.println("read.");
      if (File_open(filename, 0)) {
        for(uint16_t i=0;i<10000;i++){
          int cc = File_read();
          if(cc!=0 && cc!=-1){
            Serial.write(cc);
          }else
          {
            break;
          }
          
        }
        File_close();
        Serial.println("done.");
      }
      else {
        Serial.println("error.");
      }
      
    }
    if(inByte=='w'){
      int16_t n;
      while((n=Serial.read())==-1){
      }
      filename[0] = (uint8_t)n;
      filename[1] = 'R';
      filename[2] = 'I';
      filename[3] = 'T';
      filename[4] = 'E';
      filename[5] = 'F';
      filename[6] = 'I';
      filename[7] = 'L';
      filename[8] = 'T';
      filename[9] = 'X';
      filename[10] = 'T';
      filename[11] = '\0';
      Serial.println("write.");
      if (File_open(filename, 0)) {
        for(uint16_t i=0;i<700;i++){
          char cc = pgm_read_byte_near(datas+i);
          Serial.write(cc);
          File_write(cc);
        }
        File_close();
        Serial.println("done.");
      }
      else {
        Serial.println("error.");
      }
    }

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
