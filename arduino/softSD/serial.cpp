#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


uint8_t errCode = 0;

inline void _dly()
{
  for(uint8_t i=0;i<10;i++)//5 is good
  {
    asm volatile("nop");
  }
}

//D4
void SPI_MOSI_INIT(){PORTD &= ~_BV(4);_dly();}
void SPI_MOSI_HIGH(){DDRD &= ~_BV(4);_dly();}
void SPI_MOSI_LOW(){DDRD |= _BV(4);_dly();}

//D2
void SPI_MISO_INIT(){PORTD &= ~_BV(2);DDRD &= ~_BV(2);_dly();}
uint8_t SPI_MISO_GET(){return (~PIND) & _BV(2);}

//D3
void SPI_SCK_INIT(){PORTD &= ~_BV(3);_dly();}
void SPI_SCK_HIGH(){DDRD &= ~_BV(3);_dly();}
void SPI_SCK_LOW(){DDRD |= _BV(3);_dly();}

//D5
void SPI_CHIP_SELECT_INIT(){PORTD &= ~_BV(5);_dly();}
void SPI_CHIP_SELECT_HIGH(){DDRD &= ~_BV(5);_dly();}
void SPI_CHIP_SELECT_LOW(){DDRD |= _BV(5);_dly();}


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
uint8_t const ACMD23 = 0X17;
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





//------------------------------------------------------------------------------
// End Of Chain values for FAT entries
uint16_t const FAT16EOC = 0XFFFF;
uint16_t const FAT16EOC_MIN = 0XFFF8;
uint32_t const FAT32EOC = 0X0FFFFFFF;
uint32_t const FAT32EOC_MIN = 0X0FFFFFF8;
uint32_t const FAT32MASK = 0X0FFFFFFF;

uint8_t const DIR_NAME_DELETED = 0XE5;
uint8_t const DIR_NAME_FREE = 0X00;
uint8_t const DIR_ATT_READ_ONLY = 0X01;
uint8_t const DIR_ATT_HIDDEN = 0X02;
uint8_t const DIR_ATT_SYSTEM = 0X04;
uint8_t const DIR_ATT_VOLUME_ID = 0X08;
uint8_t const DIR_ATT_DIRECTORY = 0X10;
uint8_t const DIR_ATT_ARCHIVE = 0X20;
uint8_t const DIR_ATT_LONG_NAME_MASK = 0X3F;
uint8_t const DIR_ATT_DEFINED_BITS = 0X3F;
uint8_t const DIR_ATT_FILE_TYPE_MASK = (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY);

uint8_t const FAT_FILE_TYPE_CLOSED = 0;
uint8_t const FAT_FILE_TYPE_NORMAL = 1;
uint8_t const FAT_FILE_TYPE_ROOT16 = 2;
uint8_t const FAT_FILE_TYPE_ROOT32 = 3;
uint8_t const FAT_FILE_TYPE_MIN_DIR = FAT_FILE_TYPE_ROOT16;

uint8_t const CACHE_FOR_READ = 0;
uint8_t const CACHE_FOR_WRITE = 1;

struct part_t {//partitionTable
  uint8_t  boot;
  uint8_t  beginHead;
  unsigned beginSector : 6;
  unsigned beginCylinderHigh : 2;
  uint8_t  beginCylinderLow;
  uint8_t  type;
  uint8_t  endHead;
  unsigned endSector : 6;
  unsigned endCylinderHigh : 2;
  uint8_t  endCylinderLow;
  uint32_t firstSector;
  uint32_t totalSectors;
};
struct mbr_t {//masterBootRecord
  uint8_t  codeArea[440];
  uint32_t diskSignature;
  uint16_t usuallyZero;
  part_t   part[4];
  uint8_t  mbrSig0;
  uint8_t  mbrSig1;
};
struct bpb_t {//biosParmBlock
  uint16_t bytesPerSector;
  uint8_t  sectorsPerCluster;
  uint16_t reservedSectorCount;
  uint8_t  fatCount;
  uint16_t rootDirEntryCount;
  uint16_t totalSectors16;
  uint8_t  mediaType;
  uint16_t sectorsPerFat16;
  uint16_t sectorsPerTrtack;
  uint16_t headCount;
  uint32_t hidddenSectors;
  uint32_t totalSectors32;
  uint32_t sectorsPerFat32;
  uint16_t fat32Flags;
  uint16_t fat32Version;
  uint32_t fat32RootCluster;
  uint16_t fat32FSInfo;
  uint16_t fat32BackBootBlock;
  uint8_t  fat32Reserved[12];
};
struct fbs_t {//fat32BootSector
  uint8_t  jmpToBootCode[3];
  char     oemName[8];
  bpb_t    bpb;
  uint8_t  driveNumber;
  uint8_t  reserved1;
  uint8_t  bootSignature;
  uint32_t volumeSerialNumber;
  char     volumeLabel[11];
  char     fileSystemType[8];
  uint8_t  bootCode[420];
  uint8_t  bootSectorSig0;
  uint8_t  bootSectorSig1;
};
struct dir_t {//directoryEntry
  uint8_t  name[11];
  uint8_t  attributes;
  uint8_t  reservedNT;
  uint8_t  creationTimeTenths;
  uint16_t creationTime;
  uint16_t creationDate;
  uint16_t lastAccessDate;
  uint16_t firstClusterHigh;
  uint16_t lastWriteTime;
  uint16_t lastWriteDate;
  uint16_t firstClusterLow;
  uint32_t fileSize;
};
union cache_t {
  uint8_t  data[512];
  uint16_t fat16[256];
  uint32_t fat32[128];
  dir_t    dir[16];
  mbr_t    mbr;
  fbs_t    fbs;
};


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



uint32_t Sd2Card_block_;
uint8_t  Sd2Card_inBlock_ = 0;
uint16_t Sd2Card_offset_;
uint8_t  Sd2Card_status_;
uint8_t  Sd2Card_type_;

//------------------------------------------------------------------------------
// wait for card to go not busy
uint8_t Sd2Card_waitNotBusy(uint16_t timeoutMillis) {
  for(uint32_t i=0;i<timeoutMillis*1000;i++)
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
  Sd2Card_inBlock_ = Sd2Card_type_ = 0;
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
  for(uint32_t i=0;i<SD_INIT_TIMEOUT*1000;i++){
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
  for(uint32_t i=0;i<SD_INIT_TIMEOUT*1000;i++){
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

uint8_t Sd2Card_readData(uint32_t block, uint8_t* dst) {

  Sd2Card_block_ = block;
  // use address if not SDHC card
  if (Sd2Card_type_ != SD_CARD_TYPE_SDHC) block <<= 9;
  if (Sd2Card_cardCommand(CMD17, block)) {
    errCode = 6;
    goto fail;
  }
  if (!Sd2Card_waitStartBlock()) {
    goto fail;
  }
  Sd2Card_offset_ = 0;

  // transfer data
  for (uint16_t i = 0; i < 512; i++) {
    dst[i] = spiRec();
  }

  spiRec();//CRC
  spiRec();
  
  SPI_CHIP_SELECT_HIGH();
  Sd2Card_inBlock_ = 0;
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

cache_t  SdVolume_cacheBuffer_;        // 512 byte cache for device blocks
uint32_t SdVolume_cacheBlockNumber_ = 0XFFFFFFFF;  // Logical number of block in the cache
uint8_t  SdVolume_cacheDirty_ = 0;         // cacheFlush() will write block if true
uint32_t SdVolume_cacheMirrorBlock_ = 0;  // block number for mirror FAT
//
uint32_t SdVolume_allocSearchStart_ = 2;   // start cluster for alloc search
uint8_t  SdVolume_blocksPerCluster_;    // cluster size in blocks
uint32_t SdVolume_blocksPerFat_;       // FAT size in blocks
uint32_t SdVolume_clusterCount_;       // clusters in one FAT
uint8_t  SdVolume_clusterSizeShift_;    // shift to convert cluster count to block count
uint32_t SdVolume_dataStartBlock_;     // first data block number
uint8_t  SdVolume_fatCount_;            // number of FATs on volume
uint32_t SdVolume_fatStartBlock_;      // start block for first FAT
uint8_t  SdVolume_fatType_ = 0;             // volume type (12, 16, OR 32)
uint16_t SdVolume_rootDirEntryCount_;  // number of entries in FAT16 root dir
uint32_t SdVolume_rootDirStart_;       // root start block for FAT16, cluster for FAT32

//------------------------------------------------------------------------------
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

void SdVolume_cacheSetDirty(void) {
  SdVolume_cacheDirty_ |= CACHE_FOR_WRITE;
}

//------------------------------------------------------------------------------
uint8_t SdVolume_cacheRawBlock(uint32_t blockNumber, uint8_t action) {
  if (SdVolume_cacheBlockNumber_ != blockNumber) {
    if (!SdVolume_cacheFlush()) return false;
    if (!Sd2Card_readData(blockNumber, SdVolume_cacheBuffer_.data)) return false;
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
  if (cluster >= SdVolume_clusterCount_) return false;
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
  if (!SdVolume_fatPut(endCluster, 0x0FFFFFFF)) return false;

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

uint8_t  SdVolume_isEOC(uint32_t cluster)  {
  return  cluster >= (SdVolume_fatType_ == 16 ? FAT16EOC_MIN : FAT32EOC_MIN);
}

uint8_t SdVolume_blockOfCluster(uint32_t position) {
  return (position >> 9) & (SdVolume_blocksPerCluster_ - 1);
}
uint32_t SdVolume_clusterStartBlock(uint32_t cluster)  {
  return SdVolume_dataStartBlock_ + ((cluster - 2) << SdVolume_clusterSizeShift_);
}




int16_t Root_Load() {
  uint32_t block;  // raw device block number
  if (SdVolume_fatType_ == 16) {
    block = SdVolume_rootDirStart_;// + (Root_curPosition_ >> 9);
  } else if (SdVolume_fatType_ == 32) {
    block = SdVolume_clusterStartBlock(SdVolume_rootDirStart_);// + blockOfCluster;//Root_curCluster_
  } else {
    return false;
  }

  // read block to cache and copy data to caller
  if (!SdVolume_cacheRawBlock(block, CACHE_FOR_READ)) return -1;

  return 1;
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
  if (!SdVolume_cacheRawBlock(File_dirBlock_, action)) return 0;
  return SdVolume_cacheBuffer_.dir + File_dirIndex_;
}

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

//max 16 file
dir_t* File_list() {
	if (Root_Load() < 0) return 0;

  // return pointer to entry
  return SdVolume_cacheBuffer_.dir;
}

uint8_t File_open(uint8_t idx) {
	if (Root_Load() < 0) return 0;

  dir_t* p = &SdVolume_cacheBuffer_.dir[idx];
  
  if (p->name[0] == DIR_NAME_FREE || p->name[0] == DIR_NAME_DELETED) {
    return 0;
  }

  return File_openCachedEntry(idx);
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

uint8_t File_addCluster() {
  if (!SdVolume_allocContiguous(&File_curCluster_)) return false;

  // if first cluster of file link to directory entry
  if (File_firstCluster_ == 0) {
    File_firstCluster_ = File_curCluster_;
    File_flags_ = 1;
  }
  return true;
}

uint8_t File_read() {

  uint32_t block;  // raw device block number
  uint16_t offset = File_curPosition_ & 0X1FF;  // offset in block

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

  // read block to cache and copy data to caller
  if (!SdVolume_cacheRawBlock(block, CACHE_FOR_READ)) return -1;
  uint8_t val = SdVolume_cacheBuffer_.data[offset];
  File_curPosition_++;
  return val;
}

void File_write(uint8_t b) {
  uint8_t blockOfCluster = SdVolume_blockOfCluster(File_curPosition_);
  uint16_t blockOffset = File_curPosition_ & 0X1FF;
  if (blockOfCluster == 0 && blockOffset == 0) {
    // start of new cluster
    if (File_curCluster_ == 0) {
      if (File_firstCluster_ == 0) {
        // allocate first cluster of file
        if (!File_addCluster())return;
      } else {
        File_curCluster_ = File_firstCluster_;
      }
    } else {
      uint32_t next;
      if (!SdVolume_fatGet(File_curCluster_, &next)) return;
      if (SdVolume_isEOC(next)) {
        // add cluster if at end of chain
        if (!File_addCluster())return;
      } else {
        File_curCluster_ = next;
      }
    }
  }
  // block for data write
  uint32_t block = SdVolume_clusterStartBlock(File_curCluster_) + blockOfCluster;
  if (blockOffset == 0 && File_curPosition_ >= File_fileSize_) {
    // start of new block don't need to read into cache
    if (!SdVolume_cacheFlush()) return;
    SdVolume_cacheBlockNumber_ = block;
    SdVolume_cacheSetDirty();
  } else {
    // rewrite part of block
    if (!SdVolume_cacheRawBlock(block, CACHE_FOR_WRITE)) {
      return;
    }
  }
  
  SdVolume_cacheBuffer_.data[blockOffset] = b;

  File_curPosition_++;

  if (File_curPosition_ > File_fileSize_) {
    File_fileSize_ = File_curPosition_;
    File_flags_ = 1;
  }
}

//1:SD 2:MMC
uint8_t Sd2Card_begin(uint8_t type) {
  if(type==1){
    if(!Sd2Card_cardinit())return 0;
  }
  else if(type==2){
    if(!MMCCard_cardinit())return 0;
  }
  if(!SdVolume_volumeinit(1))
  {
      if(!SdVolume_volumeinit(0))
      {
        return 0;
      }
  }
  return 1;
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CUR_TIMING TIMING_16M_TCCR1B_1_115200
#define TCCR1B_Value 1
PROGMEM prog_uint16_t TIMING_16M_TCCR1B_1_115200[] = {  138,  277,  416,  555,  694,  833,  972, 1111, 1250, 1388};

#define DDR_Send DDRD
#define PORT_Send PORTD
#define BIT_Send _BV(1)
#define DDR_Recv DDRD
#define PIN_Recv PIND
#define BIT_Recv _BV(0)

void TimerInit() {
	TCCR1A = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
}
void SerialInit(){
	UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}

void SerialSend(uint8_t val)
{
	cli();
	TCCR1B = TCCR1B_Value;
	TCNT1 = 0;
	uint16_t timing;
	PORT_Send &= ~BIT_Send;timing = pgm_read_word_near(CUR_TIMING);while(TCNT1<timing);//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 1;i<=8;i++)
	{
		if(val&chkbit){PORT_Send |= BIT_Send;}else{PORT_Send &= ~BIT_Send;}chkbit<<=1;timing = pgm_read_word_near(CUR_TIMING + i);while(TCNT1<timing);
	}
	PORT_Send |= BIT_Send;timing = pgm_read_word_near(CUR_TIMING + 9);while(TCNT1<timing);
	sei();
}

//void SerialPrint(char* str){
//  for(uint16_t i=0;i<strlen(str);i++){
//    SerialSend(*str));
//    str++;
//  }
//}

uint8_t SerialRead()
{
	cli();
	TCCR1B = TCCR1B_Value;
	uint8_t val = 0;
	while(PIN_Recv&BIT_Recv){}
	TCNT1 = 0;
	uint16_t timing;
	timing = pgm_read_word_near(CUR_TIMING);while(TCNT1<timing);//startbit

	for(uint8_t i = 1;i<=8;i++)
	{
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);timing = pgm_read_word_near(CUR_TIMING + i);while(TCNT1<timing);
	}
	sei();
	return val;
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
uint16_t GetWord(){
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
uint32_t GetDWord(){
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

void printHex(uint8_t val){//"hl"
  SerialSend(pgm_read_byte_near(convt+((val & 0xF0) >> 4)));
  SerialSend(pgm_read_byte_near(convt+((val & 0x0F))));
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

uint8_t ReadChar(){
  while(1){
    uint8_t val = SerialRead();
    if(val==0 || val=='\r' || val=='\n'){
    } else {
      return val;
    }
  }
}

int main(void) {
	SerialInit();
	TimerInit();
  DDRD &= ~_BV(6);//pwrPIN;
  uint8_t writebuff[16];
	for(;;)
	{
    uint8_t cmd1 = ReadChar();
    uint8_t cmd2 = ReadChar();

    //is  init sd card
    if(cmd1=='i' && cmd2=='s'){
      if(Sd2Card_begin(1)) {
        SerialSend('O');SerialSend('\r');SerialSend('\n');
      }
      else
      {
        SendInt(errCode);
        SerialSend('N');SerialSend('\r');SerialSend('\n');
      }
    }
    
    //im  init mmc card
    if(cmd1=='i' && cmd2=='m'){
      if(Sd2Card_begin(2)) {
        SerialSend('O');SerialSend('\r');SerialSend('\n');
      }
      else
      {
        SendInt(errCode);
        SerialSend('N');SerialSend('\r');SerialSend('\n');
      }
    }
    
    //ls  list
    if(cmd1=='l' && cmd2=='s'){
      dir_t* dir = File_list();
      for(uint8_t i = 0; i<16; i++){
        printHex(i);
        SerialSend(' ');
        if (dir[i].name[0] == DIR_NAME_FREE || dir[i].name[0] == DIR_NAME_DELETED) {
          SerialSend('E');
        } else {
          for(uint8_t j = 0;j<11;j++){
            SerialSend(dir[i].name[j]);
          }
          SerialSend(' ');
          SendInt(dir[i].fileSize);
          SerialSend(' ');
          printHex(dir[i].attributes);
        }
        SerialSend('\r');
        SerialSend('\n');
      }
    }
    
    //op  open file
    if(cmd1=='o' && cmd2=='p'){
      uint8_t idx = GetByte();
      if (File_open(idx)) {
        SerialSend('[');
        SendInt(File_fileSize_);
        SerialSend(']');
      }
    }
    
    //sk  seek XXXXXXXX
    if(cmd1=='s' && cmd2=='k'){
      uint32_t pos = GetDWord();
      File_seekSet(pos);
      SerialSend(cmd1);SerialSend(cmd2);SerialSend('\r');SerialSend('\n');
    }
    
    //se  seek to end
    if(cmd1=='s' && cmd2=='e'){
      File_seekSet(File_fileSize_);
      SerialSend(cmd1);SerialSend(cmd2);SerialSend('\r');SerialSend('\n');
    }
    
    //cl  close
    if(cmd1=='c' && cmd2=='l'){
      File_close();
      SerialSend(cmd1);SerialSend(cmd2);SerialSend('\r');SerialSend('\n');
    }
    
    //rc  read 1 char
    if(cmd1=='r' && cmd2=='c'){
      uint8_t v = File_read();
      SerialSend(v);
    }
    //rb  read 1 byte
    if(cmd1=='r' && cmd2=='b'){
      uint8_t v = File_read();
      printHex(v);
    }

    //rd xxxx  read chars
    if(cmd1=='r' && cmd2=='d'){
      uint32_t len = GetWord();
      for(uint32_t i=0;i<len;i++){
        uint8_t val = File_read();
        SerialSend(val);
      }
    }
    //rx xxxx  read bytes
    if(cmd1=='r' && cmd2=='x'){
      uint32_t len = GetWord();
      for(uint32_t i=0;i<len;i++){
        uint8_t val = File_read();
        printHex(val);
      }
    }

    //w1 xx write bytes
    if(cmd1=='w' && cmd2=='1'){
      uint8_t data = GetByte();
      File_write(data);
      printHex(data);
    }
    
    //wx xx write bytes max16byte
    if(cmd1=='w' && cmd2=='x'){
      uint8_t len = GetByte();
      if(len>16)len=16;
      for(uint8_t i=0;i<len;i++){
        writebuff[i] = GetByte();
      }
      for(uint8_t i=0;i<len;i++){
        uint8_t data = writebuff[i];
        File_write(data);
        printHex(data);
      }
    }
    //wl write line end with \n
    if(cmd1=='w' && cmd2=='l'){
      for(uint8_t i=0;i<16;i++){
        uint8_t c = SerialRead();
        writebuff[i] = c;
        if(c == '\n'){
          break;
        }
      }
      for(uint8_t i=0;i<16;i++){
        uint8_t c = writebuff[i];
        File_write(c);
        SerialSend(c);
        if(c == '\n'){
          break;
        }
      }
    }
	}
}
