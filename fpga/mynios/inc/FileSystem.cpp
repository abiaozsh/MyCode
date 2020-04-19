//------------------------------------------------------------------------------
// End Of Chain values for FAT entries
#define FAT16EOC     0XFFFF
#define FAT16EOC_MIN 0XFFF8
#define FAT32EOC     0X0FFFFFFF
#define FAT32EOC_MIN 0X0FFFFFF8
#define FAT32MASK    0X0FFFFFFF

#define CACHE_FOR_READ   0
#define CACHE_FOR_WRITE  1

#define NULL 0

struct part_t {
  char boot;
  char beginHead;
  char beginCylinderHigh_beginSector;//2:6
  char beginCylinderLow;
  char type;
  char endHead;
  char endCylinderHigh_endSector;//2:6
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

struct mbr_t {
  char  codeArea[440];
  char diskSignature_0;
  char diskSignature_1;
  char diskSignature_2;
  char diskSignature_3;
  char usuallyZero_0;
  char usuallyZero_1;
  part_t part[4];
  char mbrSig0;
  char mbrSig1;
};

struct dir_t {
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

struct bpb_t {
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

struct fbs_t {
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
  dir_t    dir[16];
  mbr_t    mbr;
  fbs_t    fbs;
};

int combineInt(char v0, char v1, char v2, char v3){
  return (v0&0x0FF) | ((v1<<8)&0x0FF00) | ((v2<<16)&0x0FF0000) | ((v3<<24)&0x0FF000000);
}

short combineShort(char v0, char v1){
  return (v0&0x0FF) | ((v1<<8)&0x0FF00);
}

class SdFile;

class SdVolume {
 public:
  
  cache_t cacheBuffer_;        // 512 byte cache for device blocks
  uint32_t cacheBlockNumber_;  // Logical number of block in the cache
  uint8_t cacheDirty_;         // cacheFlush() will write block if true
  uint32_t cacheMirrorBlock_;  // block number for mirror FAT
  uint32_t allocSearchStart_;   // start cluster for alloc search
  uint8_t blocksPerCluster_;    // cluster size in blocks
  uint32_t blocksPerFat_;       // FAT size in blocks
  uint32_t clusterCount_;       // clusters in one FAT
  uint8_t clusterSizeShift_;    // shift to convert cluster count to block count
  uint32_t dataStartBlock_;     // first data block number
  uint8_t fatCount_;            // number of FATs on volume
  uint32_t fatStartBlock_;      // start block for first FAT
  uint8_t fatType_;             // volume type (12, 16, OR 32)
  uint16_t rootDirEntryCount_;  // number of entries in FAT16 root dir
  uint32_t rootDirStart_;       // root start block for FAT16, cluster for FAT32

  SdFile* root;
  
  int error;
  
  Sd2Card* sdCard_;            // Sd2Card object for cache
  Sd2Card* sdCard(void) {return sdCard_;}

  /** Create an instance of SdVolume */
  
  /** Clear the cache and returns a pointer to the cache.  Used by the WaveRP
   *  recorder to do raw write to the SD card.  Not for normal apps.
   */
  char* cacheClear(void) {
    cacheFlush();
    cacheBlockNumber_ = 0XFFFFFFFF;
    return cacheBuffer_.data;
  }
  /**
   * Initialize a FAT volume.  Try partition one first then try super
   * floppy format.
   *
   * \param[in] dev The Sd2Card where the volume is located.
   *
   * \return The value one, true, is returned for success and
   * the value zero, false, is returned for failure.  Reasons for
   * failure include not finding a valid partition, not finding a valid
   * FAT file system or an I/O error.
   */
  uint8_t init(Sd2Card* dev, uint8_t part) {
    error = 0;
    cacheBlockNumber_ = 0XFFFFFFFF;
    cacheDirty_ = 0;  // cacheFlush() will write block if true
    cacheMirrorBlock_ = 0;  // mirror  block for second FAT
    
    
    allocSearchStart_ = 2;
    fatType_ = 0;
    uint32_t volumeStartBlock = 0;
    sdCard_ = dev;
    // if part == 0 assume super floppy with FAT boot sector in block zero
    // if part > 0 assume mbr volume with partition table
    if (part) {
      if (part > 4)return false;
      if (!cacheRawBlock(volumeStartBlock, CACHE_FOR_READ)){
        error += 1;
        return false;
      }
      part_t* p = &cacheBuffer_.mbr.part[part-1];
    
      //print("[");
      //for(int i=0;i<512;i++){
      //  printByte(cacheBuffer_.data[i]);
      //}
      //print("]");

      int totalSectors = combineInt(p->totalSectors_0, p->totalSectors_1, p->totalSectors_2, p->totalSectors_3);
      int firstSector = combineInt(p->firstSector_0, p->firstSector_1, p->firstSector_2, p->firstSector_3);
      //printInt(p->boot);print("\r\n");
      //printInt(totalSectors);print("\r\n");
      //printInt(firstSector);print("\r\n");

      if ((p->boot & 0X7F) !=0  ||
        totalSectors < 100 ||
        firstSector == 0) {
        // not a valid partition
        
        
        error += 2;
        return false;
      }
      volumeStartBlock = firstSector;
    }
    if (!cacheRawBlock(volumeStartBlock, CACHE_FOR_READ)){
      error += 3;
      return false;
    }
    bpb_t* bpb = &cacheBuffer_.fbs.bpb;
    
    int bytesPerSector = combineShort(bpb->bytesPerSector_0, bpb->bytesPerSector_1);
    int reservedSectorCount = combineShort(bpb->reservedSectorCount_0, bpb->reservedSectorCount_1);
    if (bytesPerSector != 512 ||
      bpb->fatCount == 0 ||
      reservedSectorCount == 0 ||
      bpb->sectorsPerCluster == 0) {
         // not valid FAT volume
        error += 4;
        return false;
    }
    fatCount_ = bpb->fatCount;
    blocksPerCluster_ = bpb->sectorsPerCluster;

    // determine shift that is same as multiply by blocksPerCluster_
    clusterSizeShift_ = 0;
    while (blocksPerCluster_ != (1 << clusterSizeShift_)) {
      // error if not power of 2
      if (clusterSizeShift_++ > 7) {
        error += 5;
        return false;
      }
    }
    
    int sectorsPerFat16 = combineShort(bpb->sectorsPerFat16_0, bpb->sectorsPerFat16_1);
    int sectorsPerFat32 = combineInt(bpb->sectorsPerFat32_0, bpb->sectorsPerFat32_1, bpb->sectorsPerFat32_2, bpb->sectorsPerFat32_3);

    blocksPerFat_ = sectorsPerFat16 ?
                      sectorsPerFat16 : sectorsPerFat32;

    fatStartBlock_ = volumeStartBlock + reservedSectorCount;

    // count for FAT16 zero for FAT32
    int rootDirEntryCount = combineShort(bpb->rootDirEntryCount_0, bpb->rootDirEntryCount_1);
    rootDirEntryCount_ = rootDirEntryCount;

    // directory start for FAT16 dataStart for FAT32
    rootDirStart_ = fatStartBlock_ + bpb->fatCount * blocksPerFat_;

    // data start for FAT16 and FAT32
    dataStartBlock_ = rootDirStart_ + (((rootDirEntryCount<<5) + 511)>>9);

    // total blocks for FAT16 or FAT32
    int totalSectors16 = combineShort(bpb->totalSectors16_0, bpb->totalSectors16_1);
    int totalSectors32 = combineInt(bpb->totalSectors32_0, bpb->totalSectors32_1, bpb->totalSectors32_2, bpb->totalSectors32_3);
    uint32_t totalBlocks = totalSectors16 ?
                             totalSectors16 : totalSectors32;
    // total data blocks
    clusterCount_ = totalBlocks - (dataStartBlock_ - volumeStartBlock);

    // divide by cluster size to get cluster count
    clusterCount_ >>= clusterSizeShift_;

    // FAT type is determined by cluster count
    if (clusterCount_ < 4085) {
      fatType_ = 12;
    } else if (clusterCount_ < 65525) {
      fatType_ = 16;
    } else {
      int fat32RootCluster = combineInt(bpb->fat32RootCluster_0, bpb->fat32RootCluster_1, bpb->fat32RootCluster_2, bpb->fat32RootCluster_3);
      rootDirStart_ = fat32RootCluster;
      fatType_ = 32;
    }
    return true;
  }

  // inline functions that return volume info
  /** \return The volume's cluster size in blocks. */
  uint8_t blocksPerCluster(void) const {return blocksPerCluster_;}
  /** \return The number of blocks in one FAT. */
  uint32_t blocksPerFat(void)  const {return blocksPerFat_;}
  /** \return The total number of clusters in the volume. */
  uint32_t clusterCount(void) const {return clusterCount_;}
  /** \return The shift count required to multiply by blocksPerCluster. */
  uint8_t clusterSizeShift(void) const {return clusterSizeShift_;}
  /** \return The logical block number for the start of file data. */
  uint32_t dataStartBlock(void) const {return dataStartBlock_;}
  /** \return The number of FAT structures on the volume. */
  uint8_t fatCount(void) const {return fatCount_;}
  /** \return The logical block number for the start of the first FAT. */
  uint32_t fatStartBlock(void) const {return fatStartBlock_;}
  /** \return The FAT type of the volume. Values are 12, 16 or 32. */
  uint8_t fatType(void) const {return fatType_;}
  /** \return The number of entries in the root directory for FAT16 volumes. */
  uint32_t rootDirEntryCount(void) const {return rootDirEntryCount_;}
  /** \return The logical block number for the start of the root directory
       on FAT16 volumes or the first cluster number on FAT32 volumes. */
  uint32_t rootDirStart(void) const {return rootDirStart_;}
  /** return a pointer to the Sd2Card object for this volume */
  //----------------------------------------------------------------------------
// find a contiguous group of clusters
  uint8_t allocContiguous(uint32_t count, uint32_t* curCluster) {
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
      bgnCluster = allocSearchStart_;

      // save next search start if one cluster
      setStart = 1 == count;
    }
    // end of group
    uint32_t endCluster = bgnCluster;

    // last cluster of FAT
    uint32_t fatEnd = clusterCount_ + 1;

    // search the FAT for free clusters
    for (uint32_t n = 0;; n++, endCluster++) {
      // can't find space checked all clusters
      if (n >= clusterCount_) return false;

      // past end - start from beginning of FAT
      if (endCluster > fatEnd) {
        bgnCluster = endCluster = 2;
      }
      uint32_t f;
      if (!fatGet(endCluster, &f)) return false;

      if (f != 0) {
        // cluster in use try next cluster as bgnCluster
        bgnCluster = endCluster + 1;
      } else if ((endCluster - bgnCluster + 1) == count) {
        // done - found space
        break;
      }
    }
    // mark end of chain
    if (!fatPutEOC(endCluster)) return false;

    // link clusters
    while (endCluster > bgnCluster) {
      if (!fatPut(endCluster - 1, endCluster)) return false;
      endCluster--;
    }
    if (*curCluster != 0) {
      // connect chains
      if (!fatPut(*curCluster, bgnCluster)) return false;
    }
    // return first cluster number to caller
    *curCluster = bgnCluster;

    // remember possible next free cluster
    if (setStart) allocSearchStart_ = bgnCluster + 1;

    return true;
  }  
  
  
  uint8_t blockOfCluster(uint32_t position) const {
          return (position >> 9) & (blocksPerCluster_ - 1);}
  uint32_t clusterStartBlock(uint32_t cluster) const {
           return dataStartBlock_ + ((cluster - 2) << clusterSizeShift_);}
  uint32_t blockNumber(uint32_t cluster, uint32_t position) const {
           return clusterStartBlock(cluster) + blockOfCluster(position);}
  uint8_t cacheFlush(void) {
    if (cacheDirty_) {
      if (!sdCard_->writeBlock(cacheBlockNumber_, cacheBuffer_.data)) {
        error += 100;
        return false;
      }
      // mirror FAT tables
      if (cacheMirrorBlock_) {
        if (!sdCard_->writeBlock(cacheMirrorBlock_, cacheBuffer_.data)) {
        error += 200;
          return false;
        }
        cacheMirrorBlock_ = 0;
      }
      cacheDirty_ = 0;
    }
    return true;
  }
  uint8_t cacheRawBlock(uint32_t blockNumber, uint8_t action) {
    if (cacheBlockNumber_ != blockNumber) {
      if (!cacheFlush()){
        error += 10;
        return false;
      }
      if (!sdCard_->readBlock(blockNumber, cacheBuffer_.data)){
        error += 20;
        return false;
      }
      cacheBlockNumber_ = blockNumber;
    }
    cacheDirty_ |= action;
    return true;
  }
  
  void cacheSetDirty(void) {cacheDirty_ |= CACHE_FOR_WRITE;}
  // cache a zero block for blockNumber
  uint8_t cacheZeroBlock(uint32_t blockNumber) {
    if (!cacheFlush()) return false;

    // loop take less flash than memset(cacheBuffer_.data, 0, 512);
    for (uint16_t i = 0; i < 512; i++) {
      cacheBuffer_.data[i] = 0;
    }
    cacheBlockNumber_ = blockNumber;
    cacheSetDirty();
    return true;
  }  
  
  
  // return the size in bytes of a cluster chain
  uint8_t chainSize(uint32_t cluster, uint32_t* size) {
    uint32_t s = 0;
    do {
      if (!fatGet(cluster, &cluster)) return false;
      s += 512UL << clusterSizeShift_;
    } while (!isEOC(cluster));
    *size = s;
    return true;
  }

  // Fetch a FAT entry
  uint8_t fatGet(uint32_t cluster, uint32_t* value) {
    if (cluster > (clusterCount_ + 1)) return false;
    uint32_t lba = fatStartBlock_;
    lba += fatType_ == 16 ? cluster >> 8 : cluster >> 7;
    if (lba != cacheBlockNumber_) {
      if (!cacheRawBlock(lba, CACHE_FOR_READ)) return false;
    }
    if (fatType_ == 16) {
      *value = cacheBuffer_.fat16[cluster & 0XFF];
    } else {
      *value = cacheBuffer_.fat32[cluster & 0X7F] & FAT32MASK;
    }
    return true;
  }

  // Store a FAT entry
  uint8_t fatPut(uint32_t cluster, uint32_t value) {
    // error if reserved cluster
    if (cluster < 2) return false;

    // error if not in FAT
    if (cluster > (clusterCount_ + 1)) return false;

    // calculate block address for entry
    uint32_t lba = fatStartBlock_;
    lba += fatType_ == 16 ? cluster >> 8 : cluster >> 7;

    if (lba != cacheBlockNumber_) {
      if (!cacheRawBlock(lba, CACHE_FOR_READ)) return false;
    }
    // store entry
    if (fatType_ == 16) {
      cacheBuffer_.fat16[cluster & 0XFF] = value;
    } else {
      cacheBuffer_.fat32[cluster & 0X7F] = value;
    }
    cacheSetDirty();

    // mirror second FAT
    if (fatCount_ > 1) cacheMirrorBlock_ = lba + blocksPerFat_;
    return true;
  }
  
  uint8_t fatPutEOC(uint32_t cluster) {
    return fatPut(cluster, 0x0FFFFFFF);
  }
  // free a cluster chain
  uint8_t freeChain(uint32_t cluster) {
    // clear free cluster location
    allocSearchStart_ = 2;

    do {
      uint32_t next;
      if (!fatGet(cluster, &next)) return false;

      // free cluster
      if (!fatPut(cluster, 0)) return false;

      cluster = next;
    } while (!isEOC(cluster));

    return true;
  }
  
  
  uint8_t isEOC(uint32_t cluster) const {
    return  cluster >= (fatType_ == 16 ? FAT16EOC_MIN : FAT32EOC_MIN);
  }
  uint8_t readBlock(uint32_t block, char* dst) {
    return sdCard_->readBlock(block, dst);
  }
  uint8_t readData(uint32_t block, uint16_t offset,
    uint16_t count, char* dst) {
      return sdCard_->readData(block, offset, count, dst);
  }
  uint8_t writeBlock(uint32_t block, const char* dst) {
    return sdCard_->writeBlock(block, dst);
  }
};


// flags for ls()

#define LS_DATE  1 /** ls() flag to print modify date */
#define LS_SIZE  2 /** ls() flag to print file size */
#define LS_R     4 /** ls() flag for recursive list of subdirectories */

// use the gnu style oflag in open()

#define O_READ      0X01               /** open() oflag for reading */
#define O_RDONLY    O_READ             /** open() oflag - same as O_READ */
#define O_WRITE     0X02               /** open() oflag for write */
#define O_WRONLY    O_WRITE            /** open() oflag - same as O_WRITE */
#define O_RDWR      (O_READ | O_WRITE) /** open() oflag for reading and writing */
#define O_ACCMODE   (O_READ | O_WRITE) /** open() oflag mask for access modes */
#define O_APPEND    0X04               /** The file offset shall be set to the end of the file prior to each write. */
#define O_SYNC      0X08               /** synchronous writes - call sync() after each write */
#define O_CREAT     0X10               /** create the file if nonexistent */
#define O_EXCL      0X20               /** If O_CREAT and O_EXCL are set, open() shall fail if the file exists */
#define O_TRUNC     0X40               /** truncate the file to zero length */

// flags for timestamp

#define T_ACCESS              1                     /** set the file's last access date */
#define T_CREATE              2                     /** set the file's creation date and time */
#define T_WRITE               4                     /** Set the file's write date and time */
#define FAT_FILE_TYPE_CLOSED  0                     /** This SdFile has not been opened. */
#define FAT_FILE_TYPE_NORMAL  1                     /** SdFile for a file */
#define FAT_FILE_TYPE_ROOT16  2                     /** SdFile for a FAT16 root directory */
#define FAT_FILE_TYPE_ROOT32  3                     /** SdFile for a FAT32 root directory */
#define FAT_FILE_TYPE_SUBDIR  4                     /** SdFile for a subdirectory */
#define FAT_FILE_TYPE_MIN_DIR FAT_FILE_TYPE_ROOT16  /** Test value for directory type */

// bits defined in flags_
// should be 0XF
#define F_OFLAG   (O_ACCMODE | O_APPEND | O_SYNC)
// available bits
#define F_UNUSED  0X30
// use unbuffered SD read
#define F_FILE_UNBUFFERED_READ  0X40
// sync of directory entry required
#define F_FILE_DIR_DIRTY        0X80

#define DIR_NAME_DELETED        '\xE5'
#define DIR_NAME_FREE           '\0'
#define DIR_ATT_READ_ONLY       0X01
#define DIR_ATT_HIDDEN          0X02
#define DIR_ATT_SYSTEM          0X04
#define DIR_ATT_VOLUME_ID       0X08
#define DIR_ATT_DIRECTORY       0X10
#define DIR_ATT_ARCHIVE         0X20
#define DIR_ATT_LONG_NAME       0X0F
#define DIR_ATT_LONG_NAME_MASK  0X3F
#define DIR_ATT_DEFINED_BITS    0X3F
#define DIR_ATT_FILE_TYPE_MASK  (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY)

/** Directory entry is part of a long name */
uint8_t DIR_IS_LONG_NAME(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_LONG_NAME_MASK) == DIR_ATT_LONG_NAME;
}
/** Mask for file/subdirectory tests */
/** Directory entry is for a file */
uint8_t DIR_IS_FILE(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == 0;
}
/** Directory entry is for a subdirectory */
uint8_t DIR_IS_SUBDIR(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == DIR_ATT_DIRECTORY;
}
uint8_t DIR_IS_FILE_OR_SUBDIR(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_VOLUME_ID) == 0;
}

class SdFile {
 public:
  // private data
  uint8_t   flags_;         // See above for definition of flags_ bits
  uint8_t   type_;          // type of file see above for values
  uint32_t  curCluster_;    // cluster for current file position
  uint32_t  curPosition_;   // current file position in bytes from beginning
  uint32_t  dirBlock_;      // SD block that contains directory entry for file
  uint8_t   dirIndex_;      // index of entry in dirBlock 0 <= dirIndex_ <= 0XF
  uint32_t  fileSize_;      // file size in bytes
  uint32_t  firstCluster_;  // first cluster of file
  SdVolume* vol_;           // volume where file is located
  /** Create an instance of SdFile. */
  int fileError;
  /**
   * writeError is set to true if an error occurs during a write().
   * Set writeError to false before calling print() and/or write() and check
   * for true after calls to print() and/or write().
   */
  //bool writeError;
  /**
   * Cancel unbuffered reads for this file.
   * See setUnbufferedRead()
   */
  void clearUnbufferedRead(void) {
    flags_ &= ~F_FILE_UNBUFFERED_READ;
  }
  
  uint8_t close(void) {
    if (!sync())return false;
    type_ = FAT_FILE_TYPE_CLOSED;
    return true;
  }  
  
  uint8_t contiguousRange(uint32_t* bgnBlock, uint32_t* endBlock);
  uint8_t createContiguous(SdFile* dirFile,
          const char* fileName, uint32_t size);
  /** \return The current cluster number for a file or directory. */
  uint32_t curCluster(void) const {return curCluster_;}
  /** \return The current position for a file or directory. */
  uint32_t curPosition(void) const {return curPosition_;}
  /**
   * Set the date/time callback function
   *
   * \param[in] dateTime The user's call back function.  The callback
   * function is of the form:
   *
   * \code
   * void dateTime(uint16_t* date, uint16_t* time) {
   *   uint16_t year;
   *   uint8_t month, day, hour, minute, second;
   *
   *   // User gets date and time from GPS or real-time clock here
   *
   *   // return date using FAT_DATE macro to format fields
   *   *date = FAT_DATE(year, month, day);
   *
   *   // return time using FAT_TIME macro to format fields
   *   *time = FAT_TIME(hour, minute, second);
   * }
   * \endcode
   *
   * Sets the function that is called when a file is created or when
   * a file's directory entry is modified by sync(). All timestamps,
   * access, creation, and modify, are set when a file is created.
   * sync() maintains the last access date and last modify date/time.
   *
   * See the timestamp() function.
   */
//  static void dateTimeCallback(
//    void (*dateTime)(uint16_t* date, uint16_t* time)) {
//    dateTime_ = dateTime;
//  }

  /**
   * Cancel the date/time callback function.
   */
//  static void dateTimeCallbackCancel(void) {
//    // use explicit zero since NULL is not defined for Sanguino
//    dateTime_ = 0;
//  }

  /** \return Address of the block that contains this file's directory. */
  uint32_t dirBlock(void) const {return dirBlock_;}
  uint8_t dirEntry(dir_t* dir);
  /** \return Index of this file's directory in the block dirBlock. */
  uint8_t dirIndex(void) const {return dirIndex_;}
  static void dirName(const dir_t& dir, char* name);
  /** \return The total number of bytes in a file or directory. */
  uint32_t fileSize(void) const {return fileSize_;}
  /** \return The first cluster number for a file or directory. */
  uint32_t firstCluster(void) const {return firstCluster_;}
  /** \return True if this is a SdFile for a directory else false. */
  uint8_t isDir(void) const {return type_ >= FAT_FILE_TYPE_MIN_DIR;}
  /** \return True if this is a SdFile for a file else false. */
  uint8_t isFile(void) const {return type_ == FAT_FILE_TYPE_NORMAL;}
  /** \return True if this is a SdFile for an open file/directory else false. */
  uint8_t isOpen(void) const {return type_ != FAT_FILE_TYPE_CLOSED;}
  /** \return True if this is a SdFile for a subdirectory else false. */
  uint8_t isSubDir(void) const {return type_ == FAT_FILE_TYPE_SUBDIR;}
  /** \return True if this is a SdFile for the root directory. */
  uint8_t isRoot(void) const {
    return type_ == FAT_FILE_TYPE_ROOT16 || type_ == FAT_FILE_TYPE_ROOT32;
  }
  uint8_t makeDir(SdFile* parentDir, const char* dirName) {
    dir_t d;
    SdFile* dir = parentDir;
    
    // create a normal file
    if (!open(dir, dirName, O_CREAT | O_EXCL | O_RDWR)) return false;

    // convert SdFile to directory
    flags_ = O_READ;
    type_ = FAT_FILE_TYPE_SUBDIR;

    // allocate and zero first cluster
    if (!addDirCluster())return false;

    // force entry to SD
    if (!sync()) return false;

    // cache entry - should already be in cache due to sync() call
    dir_t* p = cacheDirEntry(CACHE_FOR_WRITE);
    if (!p) return false;

    // change directory entry  attribute
    p->attributes = DIR_ATT_DIRECTORY;

    // make entry for '.'
    memcpy(&d, p, sizeof(d));
    
    for (uint8_t i = 1; i < 11; i++) d.name[i] = ' ';
    d.name[0] = '.';

    // cache block for '.'  and '..'
    uint32_t block = vol_->clusterStartBlock(firstCluster_);
    if (!vol_->cacheRawBlock(block, CACHE_FOR_WRITE)) return false;

    // copy '.' to block
    memcpy(&vol_->cacheBuffer_.dir[0], &d, sizeof(d));

    // make entry for '..'
    d.name[1] = '.';
    if (dir->isRoot()) {
      //d.firstClusterLow = 0;
      //d.firstClusterHigh = 0;
      d.firstClusterLow_0  = 0;
      d.firstClusterLow_1  = 0;
      d.firstClusterHigh_0 = 0;
      d.firstClusterHigh_1 = 0;
    } else {
      d.firstClusterLow_0  = (dir->firstCluster_) & 0xFF;
      d.firstClusterLow_1  = (dir->firstCluster_>>8) & 0xFF;
      d.firstClusterHigh_0 = (dir->firstCluster_>>16) & 0xFF;
      d.firstClusterHigh_1 = (dir->firstCluster_>>24) & 0xFF;
    }
    // copy '..' to block
    memcpy(&vol_->cacheBuffer_.dir[1], &d, sizeof(d));

    // set position after '..'
    curPosition_ = 2 * sizeof(d);

    // write first block
    return vol_->cacheFlush();
  }  
  
  uint8_t open(SdFile* dirFile, const char* fileName, uint8_t oflag) {
    fileError = 0;
    
    type_ = FAT_FILE_TYPE_CLOSED;
    char dname[11];
    dir_t* p;

    // error if already open
    if (isOpen()){
      fileError += 1;
      return false;
    }
    if (!make83Name(fileName, dname)){
      fileError += 2;
      return false;
    }
    vol_ = dirFile->vol_;
    dirFile->rewind();

    // bool for empty entry found
    uint8_t emptyFound = false;

    // search for file
    while (dirFile->curPosition_ < dirFile->fileSize_) {
      uint8_t index = 0XF & (dirFile->curPosition_ >> 5);
      p = dirFile->readDirCache();
      if (p == NULL) {
        fileError += 3;
        return false;
      }
      
      //print(dname,11);
      //print(p->name, 11);
      //print("\r\n");
      if (p->name[0] == DIR_NAME_FREE || p->name[0] == DIR_NAME_DELETED) {
        // remember first empty slot
        if (!emptyFound) {
          emptyFound = true;
          dirIndex_ = index;
          dirBlock_ = vol_->cacheBlockNumber_;
        }
        // done if no entries follow
        if (p->name[0] == DIR_NAME_FREE) break;
      } else if (equal(dname, p->name, 11)) {
        // don't open existing file if O_CREAT and O_EXCL
        if ((oflag & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL)) {
          fileError += 4;
          return false;
        }

        // open found file
        if(!openCachedEntry(0XF & index, oflag)){
          fileError += 5;
          return false;
        }else{
          return true;
        }
      }
    }
    // only create file if O_CREAT and O_WRITE
    if ((oflag & (O_CREAT | O_WRITE)) != (O_CREAT | O_WRITE)) {
      fileError += 6;
      return false;
    }

    // cache found slot or add cluster if end of file
    if (emptyFound) {
      p = cacheDirEntry(CACHE_FOR_WRITE);
      if (!p) {
        fileError += 7;
        return false;
      }
    } else {
      if (dirFile->type_ == FAT_FILE_TYPE_ROOT16) {
        fileError += 8;
        return false;
      }

      // add and zero cluster for dirFile - first cluster is in cache for write
      if (!dirFile->addDirCluster()) {
        fileError += 9;
        return false;
      }

      // use first entry in cluster
      dirIndex_ = 0;
      p = vol_->cacheBuffer_.dir;
    }
    // initialize as empty file
    //memset(p, 0, sizeof(dir_t));
    for(int i=0;i<sizeof(dir_t);i++){
      ((char*)p)[i]=0;
    }
    memcpy(p->name, dname, 11);
    
    
//    // set timestamps
//    if (dateTime_) {
//      // call user function
//      dateTime_(&p->creationDate, &p->creationTime);
//    } else {
//      // use default date/time
//      p->creationDate = FAT_DEFAULT_DATE;
//      p->creationTime = FAT_DEFAULT_TIME;
//    }
//    p->lastAccessDate = p->creationDate;
//    p->lastWriteDate = p->creationDate;
//    p->lastWriteTime = p->creationTime;

    // force write of entry to SD
    if (!vol_->cacheFlush()) {
      fileError += 9;
      return false;
    }

    // open entry in cache
    if(!openCachedEntry(dirIndex_, oflag)){
      fileError += 9;
      return false;
    }else{
      return true;
    }
    return true;
  }

  uint8_t open(SdFile* dirFile, uint16_t index, uint8_t oflag) {
    type_ = FAT_FILE_TYPE_CLOSED;
    // error if already open
    if (isOpen())return false;

    // don't open existing file if O_CREAT and O_EXCL - user call error
    if ((oflag & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL)) return false;

    vol_ = dirFile->vol_;

    // seek to location of entry
    if (!dirFile->seekSet(32 * index)) return false;

    // read entry into cache
    dir_t* p = dirFile->readDirCache();
    if (p == NULL) return false;

    // error if empty slot or '.' or '..'
    if (p->name[0] == DIR_NAME_FREE ||
        p->name[0] == DIR_NAME_DELETED || p->name[0] == '.') {
      return false;
    }
    // open cached entry
    return openCachedEntry(index & 0XF, oflag);
  }

  uint8_t openRoot(SdVolume* vol) {
    type_ = FAT_FILE_TYPE_CLOSED;
    // error if file is already open
    if (isOpen()) return false;

    if (vol->fatType() == 16) {
      type_ = FAT_FILE_TYPE_ROOT16;
      firstCluster_ = 0;
      fileSize_ = 32 * vol->rootDirEntryCount();
    } else if (vol->fatType() == 32) {
      type_ = FAT_FILE_TYPE_ROOT32;
      firstCluster_ = vol->rootDirStart();
      if (!vol->chainSize(firstCluster_, &fileSize_)) return false;
    } else {
      // volume is not initialized or FAT12
      return false;
    }
    vol_ = vol;
    // read only
    flags_ = O_READ;

    // set to start of file
    curCluster_ = 0;
    curPosition_ = 0;

    // root has no directory entry
    dirBlock_ = 0;
    dirIndex_ = 0;
    return true;
  }
  
  
//------------------------------------------------------------------------------
/** List directory contents to Serial.
 *
 * \param[in] flags The inclusive OR of
 *
 * LS_DATE - %Print file modification date
 *
 * LS_SIZE - %Print file size.
 *
 * LS_R - Recursive list of subdirectories.
 *
 * \param[in] indent Amount of space before file name. Used for recursive
 * list to indicate subdirectory level.
 */
  void dirList() {
    fileError = 0;
    dir_t* p;

    rewind();
    while (true) {
      p = readDirCache();
      if(!p){
        print("[err");printInt(fileError);print("]\r\n");
        break;
      }
      // done if past last used entry
      if (p->name[0] == DIR_NAME_FREE) {
        print("[end]\r\n");
        break;
      }

      // skip deleted entry and entries for . and  ..
      if (p->name[0] == DIR_NAME_DELETED || p->name[0] == '.') continue;

      // only list subdirectories and files
      if (!DIR_IS_FILE_OR_SUBDIR(p)) continue;

      // print any indent spaces
//      for (int8_t i = 0; i < indent; i++) uart_write(' ');

      // print file name with possible blank fill
      //printDirName(*p, flags & (LS_DATE | LS_SIZE) ? 14 : 0);
      printDirName(*p, 14);

      // print modify date/time if requested
//      if (flags & LS_DATE) {
//         printFatDate(p->lastWriteDate);
//         uart_write(' ');
//         printFatTime(p->lastWriteTime);
//      }
      // print size if requested
      if (!DIR_IS_SUBDIR(p)) {// && (flags & LS_SIZE)
        uart_write(' ');
        int fileSize = combineInt(p->fileSize_0, p->fileSize_1, p->fileSize_2, p->fileSize_3);
        printInt(fileSize);
      }
      print("\r\n");

      // list subdirectory content if requested
//      if ((flags & LS_R) && DIR_IS_SUBDIR(p)) {
//        uint16_t index = curPosition()/32 - 1;
//        SdFile s;
//        if (s.open(this, index, O_READ)) s.ls(flags, indent + 2);
//        seekSet(32 * (index + 1));
//      }
    }
  }  
    
  
  void printDirName(const dir_t& dir, uint8_t width) {
    uint8_t w = 0;
    for (uint8_t i = 0; i < 11; i++) {
      if (dir.name[i] == ' ')continue;
      if (i == 8) {
        uart_write('.');
        w++;
      }
      uart_write(dir.name[i]);
      w++;
    }
    if (DIR_IS_SUBDIR(&dir)) {
      uart_write('/');
      w++;
    }
    while (w < width) {
      uart_write(' ');
      w++;
    }
  }
//  static void printFatDate(uint16_t fatDate);
//  static void printFatTime(uint16_t fatTime);
  static void printTwoDigits(uint8_t v);
  /**
   * Read the next byte from a file.
   *
   * \return For success read returns the next byte in the file as an int.
   * If an error occurs or end of file is reached -1 is returned.
   */
  int16_t read() {
    char b;
    int result = read(&b, 1);
    if(result == 1){
      return ((int)b)&0xFF;
    }else{
      fileError += 5000;
      return -1;
    }
  }

  int16_t read(char* buf, uint16_t nbyte) {
    char* dst = buf;

    // error if not open or write only
    if (!isOpen() || !(flags_ & O_READ)){
      fileError += 1000;
      return -1;
    }

    // max bytes left in file
    if (nbyte > (fileSize_ - curPosition_)) nbyte = fileSize_ - curPosition_;

    // amount left to read
    uint16_t toRead = nbyte;
    while (toRead > 0) {
      uint32_t block;  // raw device block number
      uint16_t offset = curPosition_ & 0X1FF;  // offset in block
      if (type_ == FAT_FILE_TYPE_ROOT16) {
        block = vol_->rootDirStart() + (curPosition_ >> 9);
      } else {
        uint8_t blockOfCluster = vol_->blockOfCluster(curPosition_);
        if (offset == 0 && blockOfCluster == 0) {
          // start of new cluster
          if (curPosition_ == 0) {
            // use first cluster in file
            curCluster_ = firstCluster_;
          } else {
            // get next cluster from FAT
            if (!vol_->fatGet(curCluster_, &curCluster_)) {
              fileError += 2000;
              return -1;
            }
          }
        }
        block = vol_->clusterStartBlock(curCluster_) + blockOfCluster;
      }
      uint16_t n = toRead;

      // amount to be read from current block
      if (n > (512 - offset)) n = 512 - offset;

      // no buffering needed if n == 512 or user requests no buffering
      if ((unbufferedRead() || n == 512) &&
        block != vol_->cacheBlockNumber_) {
        if (!vol_->readData(block, offset, n, dst)) {
          fileError += 3000;
          return -1;
        }
        dst += n;
      } else {
        // read block to cache and copy data to caller
        if (!vol_->cacheRawBlock(block, CACHE_FOR_READ)) {
          fileError += 4000;
          return -1;
        }
        char* src = vol_->cacheBuffer_.data + offset;
        char* end = src + n;
        while (src != end) *dst++ = *src++;
      }
      curPosition_ += n;
      toRead -= n;
    }
    return nbyte;
  }





  int8_t readDir(dir_t* dir);
  static uint8_t remove(SdFile* dirFile, const char* fileName);
  uint8_t remove(void);
  /** Set the file's current position to zero. */
  void rewind(void) {
    curPosition_ = curCluster_ = 0;
  }
  uint8_t rmDir(void);
  uint8_t rmRfStar(void);
  /** Set the files position to current position + \a pos. See seekSet(). */
  uint8_t seekCur(uint32_t pos) {
    return seekSet(curPosition_ + pos);
  }
  /**
   *  Set the files current position to end of file.  Useful to position
   *  a file for append. See seekSet().
   */
  uint8_t seekEnd(void) {return seekSet(fileSize_);}
  /**
   * Sets a file's position.
   *
   * \param[in] pos The new position in bytes from the beginning of the file.
   *
   * \return The value one, true, is returned for success and
   * the value zero, false, is returned for failure.
   */
  uint8_t seekSet(uint32_t pos) {
    // error if file not open or seek past end of file
    if (!isOpen() || pos > fileSize_) return false;

    if (type_ == FAT_FILE_TYPE_ROOT16) {
      curPosition_ = pos;
      return true;
    }
    if (pos == 0) {
      // set position to start of file
      curCluster_ = 0;
      curPosition_ = 0;
      return true;
    }
    // calculate cluster index for cur and new position
    uint32_t nCur = (curPosition_ - 1) >> (vol_->clusterSizeShift_ + 9);
    uint32_t nNew = (pos - 1) >> (vol_->clusterSizeShift_ + 9);

    if (nNew < nCur || curPosition_ == 0) {
      // must follow chain from first cluster
      curCluster_ = firstCluster_;
    } else {
      // advance from curPosition
      nNew -= nCur;
    }
    while (nNew--) {
      if (!vol_->fatGet(curCluster_, &curCluster_)) return false;
    }
    curPosition_ = pos;
    return true;
  }
  
  
  /**
   * Use unbuffered reads to access this file.  Used with Wave
   * Shield ISR.  Used with Sd2Card::partialBlockRead() in WaveRP.
   *
   * Not recommended for normal applications.
   */
  void setUnbufferedRead(void) {
    if (isFile()) flags_ |= F_FILE_UNBUFFERED_READ;
  }
//  uint8_t timestamp(uint8_t flag, uint16_t year, uint8_t month, uint8_t day,
//          uint8_t hour, uint8_t minute, uint8_t second);
  /**
   * The sync() call causes all modified data and directory fields
   * to be written to the storage device.
   *
   * \return The value one, true, is returned for success and
   * the value zero, false, is returned for failure.
   * Reasons for failure include a call to sync() before a file has been
   * opened or an I/O error.
   */
  uint8_t sync(void) {
    // only allow open files and directories
    if (!isOpen()) return false;

    if (flags_ & F_FILE_DIR_DIRTY) {
      dir_t* d = cacheDirEntry(CACHE_FOR_WRITE);
      if (!d) return false;

      // do not set filesize for dir files
      if (!isDir()){
        //d->fileSize = fileSize_;
        d->fileSize_0 = (fileSize_) & 0xFF;
        d->fileSize_1 = (fileSize_>>8) & 0xFF;
        d->fileSize_2 = (fileSize_>>16) & 0xFF;
        d->fileSize_3 = (fileSize_>>24) & 0xFF;
      }
      // update first cluster fields
      //d->firstClusterLow = firstCluster_ & 0XFFFF;
      //d->firstClusterHigh = firstCluster_ >> 16;
      d->firstClusterLow_0  = (firstCluster_) & 0xFF;
      d->firstClusterLow_1  = (firstCluster_>>8) & 0xFF;
      d->firstClusterHigh_0 = (firstCluster_>>16) & 0xFF;
      d->firstClusterHigh_1 = (firstCluster_>>24) & 0xFF;

      // set modify time if user supplied a callback date/time function
//      if (dateTime_) {
//        dateTime_(&d->lastWriteDate, &d->lastWriteTime);
//        d->lastAccessDate = d->lastWriteDate;
//      }
      // clear directory dirty
      flags_ &= ~F_FILE_DIR_DIRTY;
    }
    return vol_->cacheFlush();
  }  
  
  /** Type of this SdFile.  You should use isFile() or isDir() instead of type()
   * if possible.
   *
   * \return The file or directory type.
   */
  uint8_t type(void) const {return type_;}
/**
 * Truncate a file to a specified length.  The current file position
 * will be maintained if it is less than or equal to \a length otherwise
 * it will be set to end of file.
 *
 * \param[in] length The desired length for the file.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include file is read only, file is a directory,
 * \a length is greater than the current file size or an I/O error occurs.
 */
  uint8_t truncate(uint32_t length) {
  // error if not a normal file or read-only
    if (!isFile() || !(flags_ & O_WRITE)) return false;

    // error if length is greater than current size
    if (length > fileSize_) return false;

    // fileSize and length are zero - nothing to do
    if (fileSize_ == 0) return true;

    // remember position for seek after truncation
    uint32_t newPos = curPosition_ > length ? length : curPosition_;

    // position to last cluster in truncated file
    if (!seekSet(length)) return false;

    if (length == 0) {
      // free all clusters
      if (!vol_->freeChain(firstCluster_)) return false;
      firstCluster_ = 0;
    } else {
      uint32_t toFree;
      if (!vol_->fatGet(curCluster_, &toFree)) return false;

      if (!vol_->isEOC(toFree)) {
        // free extra clusters
        if (!vol_->freeChain(toFree)) return false;

        // current cluster is end of chain
        if (!vol_->fatPutEOC(curCluster_)) return false;
      }
    }
    fileSize_ = length;

    // need to update directory entry
    flags_ |= F_FILE_DIR_DIRTY;

    if (!sync()) return false;

    // set file to correct position
    return seekSet(newPos);
  }  
  
  /** \return Unbuffered read flag. */
  uint8_t unbufferedRead(void) const {
    return flags_ & F_FILE_UNBUFFERED_READ;
  }
  /** \return SdVolume that contains this file. */
  SdVolume* volume(void) const {return vol_;}
  size_t write(uint8_t b);
  size_t write(const void* buf, uint16_t nbyte);
  size_t write(const char* str);
//------------------------------------------------------------------------------
 private:


  // private functions
  // add a cluster to a file
  uint8_t addCluster() {
    if (!vol_->allocContiguous(1, &curCluster_)) return false;

    // if first cluster of file link to directory entry
    if (firstCluster_ == 0) {
      firstCluster_ = curCluster_;
      flags_ |= F_FILE_DIR_DIRTY;
    }
    return true;
  }

  // Add a cluster to a directory file and zero the cluster.
  // return with first block of cluster in the cache
  uint8_t addDirCluster(void) {
    if (!addCluster()) return false;

    // zero data in cluster insure first cluster is in cache
    uint32_t block = vol_->clusterStartBlock(curCluster_);
    for (uint8_t i = vol_->blocksPerCluster_; i != 0; i--) {
      if (!vol_->cacheZeroBlock(block + i - 1)) return false;
    }
    // Increase directory file size by cluster size
    fileSize_ += 512UL << vol_->clusterSizeShift_;
    return true;
  }
  // cache a file's directory entry
  // return pointer to cached entry or null for failure
  dir_t* cacheDirEntry(uint8_t action) {
    if (!vol_->cacheRawBlock(dirBlock_, action)) return NULL;
    return vol_->cacheBuffer_.dir + dirIndex_;
  }

//  static void (*dateTime_)(uint16_t* date, uint16_t* time);

//------------------------------------------------------------------------------
// format directory name field from a 8.3 name string
  uint8_t make83Name(const char* str, char* name) {
    char c;
    uint8_t n = 7;  // max index for part before dot
    uint8_t i = 0;
    // blank fill name and extension
    while (i < 11) name[i++] = ' ';
    i = 0;
    while ((c = *str++) != '\0') {
      if (c == '.') {
        if (n == 10) return false;  // only one dot allowed
        n = 10;  // max index for full 8.3 name
        i = 8;   // place for extension
      } else {
        // illegal FAT characters
        uint8_t b;
        const char* valid = "|<>^+=?/[];,*\"\\";
        const char *p = valid;
        while ((b = *p++)) if (b == c) return false;
        // check size and only allow ASCII printable characters
        if (i > n || c < 0X21 || c > 0X7E)return false;
        // only upper case allowed in 8.3 names - convert lower to upper
        name[i++] = c < 'a' || c > 'z' ?  c : c + ('A' - 'a');
      }
    }
    // must have a file name, extension is optional
    return name[0] != ' ';
  }  
    
  // open a cached directory entry. Assumes vol_ is initializes
  uint8_t openCachedEntry(uint8_t dirIndex, uint8_t oflag) {
    // location of entry in cache
    dir_t* p = vol_->cacheBuffer_.dir + dirIndex;

    // write or truncate is an error for a directory or read-only file
    if (p->attributes & (DIR_ATT_READ_ONLY | DIR_ATT_DIRECTORY)) {
      if (oflag & (O_WRITE | O_TRUNC)) {
        fileError += 10;
        return false;
      }
    }
    // remember location of directory entry on SD
    dirIndex_ = dirIndex;
    dirBlock_ = vol_->cacheBlockNumber_;

    
    // copy first cluster number for directory fields
    //firstCluster_ = (uint32_t)p->firstClusterHigh << 16;
    //firstCluster_ |= p->firstClusterLow;
    firstCluster_ = combineInt(p->firstClusterLow_0, p->firstClusterLow_1, p->firstClusterHigh_0, p->firstClusterHigh_1);

    // make sure it is a normal file or subdirectory
    print("dir->attributes:");printInt(p->attributes);print("\r\n");
    print("dirIndex:");printInt(dirIndex);print("\r\n");
    if (DIR_IS_FILE(p)) {
      //fileSize_ = p->fileSize;
      fileSize_ = combineInt(p->fileSize_0, p->fileSize_1, p->fileSize_2, p->fileSize_3);
      type_ = FAT_FILE_TYPE_NORMAL;
    } else if (DIR_IS_SUBDIR(p)) {
      if (!vol_->chainSize(firstCluster_, &fileSize_)) {
        fileError += 20;
        return false;
      }
      type_ = FAT_FILE_TYPE_SUBDIR;
    } else {
      fileError += 30;
      return false;
    }
    // save open flags for read/write
    flags_ = oflag & (O_ACCMODE | O_SYNC | O_APPEND);

    // set to start of file
    curCluster_ = 0;
    curPosition_ = 0;

    // truncate file to zero length if requested
    if (oflag & O_TRUNC) {
      if(!truncate(0)){
        fileError += 40;
        return false;
      }else{
        return true;
      }
    }
    return true;
  }  
  
  // Read next directory entry into the cache
  // Assumes file is correctly positioned
  dir_t* readDirCache(void) {
    // error if not directory
    if (!isDir()){
      fileError += 100;
      return NULL;
    }

    // index of entry in cache
    uint8_t i = (curPosition_ >> 5) & 0XF;

    // use read to locate and cache block
    if (read() < 0) {
      
      fileError += 200;
      return NULL;
    }

    // advance to next entry
    curPosition_ += 31;

    // return pointer to entry
    return (vol_->cacheBuffer_.dir + i);
  }

};