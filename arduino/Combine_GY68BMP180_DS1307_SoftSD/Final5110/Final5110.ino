
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdint.h>


#include <avr/io.h>

uint8_t errCode = 0;

inline void _dly()
{
  for(uint8_t i=0;i<5;i++)//5 is good
  {
    volatile uint8_t t;
    t=0;
  }
}

//B4
void SPI_MOSI_INIT(){PORTB &= ~_BV(4);_dly();}
void SPI_MOSI_HIGH(){DDRB &= ~_BV(4);_dly();}
void SPI_MOSI_LOW(){DDRB |= _BV(4);_dly();}

//B2
void SPI_MISO_INIT(){PORTB &= ~_BV(2);DDRB &= ~_BV(2);_dly();}
uint8_t SPI_MISO_GET(){return (~PINB) & _BV(2);}

//B3
void SPI_SCK_INIT(){PORTB &= ~_BV(3);_dly();}
void SPI_SCK_HIGH(){DDRB &= ~_BV(3);_dly();}
void SPI_SCK_LOW(){DDRB |= _BV(3);_dly();}

//C0
void SPI_CHIP_SELECT_INIT(){PORTC &= ~_BV(0);_dly();}
void SPI_CHIP_SELECT_HIGH(){DDRC &= ~_BV(0);_dly();}
void SPI_CHIP_SELECT_LOW(){DDRC |= _BV(0);_dly();}

// www.sdcard.org/developers/tech/sdcard/pls/Simplified_Physical_Layer_Spec.pdf
//------------------------------------------------------------------------------
// SD card commands
/** GO_IDLE_STATE - init card in spi mode if CS low */
uint8_t const CMD0 = 0X00;
/** SEND_IF_COND - verify SD Memory Card interface operating condition.*/
uint8_t const CMD8 = 0X08;
/** SEND_CSD - read the Card Specific Data (CSD register) */
uint8_t const CMD9 = 0X09;
/** SEND_CID - read the card identification information (CID register) */
uint8_t const CMD10 = 0X0A;
/** SEND_STATUS - read the card status register */
uint8_t const CMD13 = 0X0D;
/** READ_BLOCK - read a single data block from the card */
uint8_t const CMD17 = 0X11;
/** WRITE_BLOCK - write a single data block to the card */
uint8_t const CMD24 = 0X18;
/** WRITE_MULTIPLE_BLOCK - write blocks of data until a STOP_TRANSMISSION */
uint8_t const CMD25 = 0X19;
/** ERASE_WR_BLK_START - sets the address of the first block to be erased */
uint8_t const CMD32 = 0X20;
/** ERASE_WR_BLK_END - sets the address of the last block of the continuous
    range to be erased*/
uint8_t const CMD33 = 0X21;
/** ERASE - erase all previously selected blocks */
uint8_t const CMD38 = 0X26;
/** APP_CMD - escape for application specific command */
uint8_t const CMD55 = 0X37;
/** READ_OCR - read the OCR register of a card */
uint8_t const CMD58 = 0X3A;
/** SET_WR_BLK_ERASE_COUNT - Set the number of write blocks to be
     pre-erased before writing */
uint8_t const ACMD23 = 0X17;
/** SD_SEND_OP_COMD - Sends host capacity support information and
    activates the card's initialization process */
uint8_t const ACMD41 = 0X29;
//------------------------------------------------------------------------------
/** status for card in the ready state */
uint8_t const R1_READY_STATE = 0X00;
/** status for card in the idle state */
uint8_t const R1_IDLE_STATE = 0X01;
/** status bit for illegal command */
uint8_t const R1_ILLEGAL_COMMAND = 0X04;
/** start data token for read or write single block*/
uint8_t const DATA_START_BLOCK = 0XFE;
/** stop token for write multiple blocks*/
uint8_t const STOP_TRAN_TOKEN = 0XFD;
/** start data token for write multiple blocks*/
uint8_t const WRITE_MULTIPLE_TOKEN = 0XFC;
/** mask for data response tokens after a write block operation */
uint8_t const DATA_RES_MASK = 0X1F;
/** write data accepted token */
uint8_t const DATA_RES_ACCEPTED = 0X05;




 // define software SPI pins so Mega can use unmodified GPS Shield
//GND

//------------------------------------------------------------------------------
/** Protect block zero from write if nonzero */
/** init timeout ms */
uint16_t const SD_INIT_TIMEOUT = 1000;
/** read timeout ms */
uint16_t const SD_READ_TIMEOUT = 300;
/** write time out ms */
uint16_t const SD_WRITE_TIMEOUT = 600;
//------------------------------------------------------------------------------
// card types
/** Standard capacity V1 SD card */
uint8_t const SD_CARD_TYPE_SD1 = 1;
/** Standard capacity V2 SD card */
uint8_t const SD_CARD_TYPE_SD2 = 2;
/** High Capacity SD card */
uint8_t const SD_CARD_TYPE_SDHC = 3;



//------------------------------------------------------------------------------
/**
 * \class Sd2Card
 * \brief Raw access to SD and SDHC flash memory cards.
 */
class Sd2Card {
 public:
  /** Construct an instance of Sd2Card. */
  Sd2Card(void) : inBlock_(0), type_(0) {}

  /**
   * Initialize an SD flash memory card with the selected SPI clock rate
   * and the default SD chip select pin.
   */
  uint8_t cardinit();
  uint8_t readBlock(uint32_t block, uint8_t* dst);
  uint8_t readData(uint32_t block, uint16_t offset, uint16_t count, uint8_t* dst);
  void readEnd(void);
  /** Return the card type: SD V1, SD V2 or SDHC */
  uint8_t type(void) const {return type_;}
  uint8_t writeBlock(uint32_t blockNumber, const uint8_t* src);
  uint8_t writeData(const uint8_t* src);
 private:
  uint32_t block_;
  uint8_t inBlock_;
  uint16_t offset_;
  uint8_t status_;
  uint8_t type_;
  // private functions
  uint8_t cardAcmd(uint8_t cmd, uint32_t arg) {
    cardCommand(CMD55, 0);
    return cardCommand(cmd, arg);
  }
  uint8_t cardCommand(uint8_t cmd, uint32_t arg);
  uint8_t sendWriteCommand(uint32_t blockNumber, uint32_t eraseCount);
  void type(uint8_t value) {type_ = value;}
  uint8_t waitNotBusy(uint16_t timeoutMillis);
  uint8_t writeData(uint8_t token, const uint8_t* src);
  uint8_t waitStartBlock(void);
};


/**
 * \struct partitionTable
 * \brief MBR partition table entry
 *
 * A partition table entry for a MBR formatted storage device.
 * The MBR partition table has four entries.
 */
struct partitionTable {
          /**
           * Boot Indicator . Indicates whether the volume is the active
           * partition.  Legal values include: 0X00. Do not use for booting.
           * 0X80 Active partition.
           */
  uint8_t  boot;
          /**
            * Head part of Cylinder-head-sector address of the first block in
            * the partition. Legal values are 0-255. Only used in old PC BIOS.
            */
  uint8_t  beginHead;
          /**
           * Sector part of Cylinder-head-sector address of the first block in
           * the partition. Legal values are 1-63. Only used in old PC BIOS.
           */
  unsigned beginSector : 6;
           /** High bits cylinder for first block in partition. */
  unsigned beginCylinderHigh : 2;
          /**
           * Combine beginCylinderLow with beginCylinderHigh. Legal values
           * are 0-1023.  Only used in old PC BIOS.
           */
  uint8_t  beginCylinderLow;
          /**
           * Partition type. See defines that begin with PART_TYPE_ for
           * some Microsoft partition types.
           */
  uint8_t  type;
          /**
           * head part of cylinder-head-sector address of the last sector in the
           * partition.  Legal values are 0-255. Only used in old PC BIOS.
           */
  uint8_t  endHead;
          /**
           * Sector part of cylinder-head-sector address of the last sector in
           * the partition.  Legal values are 1-63. Only used in old PC BIOS.
           */
  unsigned endSector : 6;
           /** High bits of end cylinder */
  unsigned endCylinderHigh : 2;
          /**
           * Combine endCylinderLow with endCylinderHigh. Legal values
           * are 0-1023.  Only used in old PC BIOS.
           */
  uint8_t  endCylinderLow;
           /** Logical block address of the first block in the partition. */
  uint32_t firstSector;
           /** Length of the partition, in blocks. */
  uint32_t totalSectors;
};
/** Type name for partitionTable */
typedef struct partitionTable part_t;
//------------------------------------------------------------------------------
/**
 * \struct masterBootRecord
 *
 * \brief Master Boot Record
 *
 * The first block of a storage device that is formatted with a MBR.
 */
struct masterBootRecord {
           /** Code Area for master boot program. */
  uint8_t  codeArea[440];
           /** Optional WindowsNT disk signature. May contain more boot code. */
  uint32_t diskSignature;
           /** Usually zero but may be more boot code. */
  uint16_t usuallyZero;
           /** Partition tables. */
  part_t   part[4];
           /** First MBR signature byte. Must be 0X55 */
  uint8_t  mbrSig0;
           /** Second MBR signature byte. Must be 0XAA */
  uint8_t  mbrSig1;
};
/** Type name for masterBootRecord */
typedef struct masterBootRecord mbr_t;
//------------------------------------------------------------------------------
/** 
 * \struct biosParmBlock
 *
 * \brief BIOS parameter block
 * 
 *  The BIOS parameter block describes the physical layout of a FAT volume.
 */
struct biosParmBlock {
          /**
           * Count of bytes per sector. This value may take on only the
           * following values: 512, 1024, 2048 or 4096
           */
  uint16_t bytesPerSector;
          /**
           * Number of sectors per allocation unit. This value must be a
           * power of 2 that is greater than 0. The legal values are
           * 1, 2, 4, 8, 16, 32, 64, and 128.
           */
  uint8_t  sectorsPerCluster;
          /**
           * Number of sectors before the first FAT.
           * This value must not be zero.
           */
  uint16_t reservedSectorCount;
          /** The count of FAT data structures on the volume. This field should
           *  always contain the value 2 for any FAT volume of any type.
           */
  uint8_t  fatCount;
          /**
          * For FAT12 and FAT16 volumes, this field contains the count of
          * 32-byte directory entries in the root directory. For FAT32 volumes,
          * this field must be set to 0. For FAT12 and FAT16 volumes, this
          * value should always specify a count that when multiplied by 32
          * results in a multiple of bytesPerSector.  FAT16 volumes should
          * use the value 512.
          */
  uint16_t rootDirEntryCount;
          /**
           * This field is the old 16-bit total count of sectors on the volume.
           * This count includes the count of all sectors in all four regions
           * of the volume. This field can be 0; if it is 0, then totalSectors32
           * must be non-zero.  For FAT32 volumes, this field must be 0. For
           * FAT12 and FAT16 volumes, this field contains the sector count, and
           * totalSectors32 is 0 if the total sector count fits
           * (is less than 0x10000).
           */
  uint16_t totalSectors16;
          /**
           * This dates back to the old MS-DOS 1.x media determination and is
           * no longer usually used for anything.  0xF8 is the standard value
           * for fixed (non-removable) media. For removable media, 0xF0 is
           * frequently used. Legal values are 0xF0 or 0xF8-0xFF.
           */
  uint8_t  mediaType;
          /**
           * Count of sectors occupied by one FAT on FAT12/FAT16 volumes.
           * On FAT32 volumes this field must be 0, and sectorsPerFat32
           * contains the FAT size count.
           */
  uint16_t sectorsPerFat16;
           /** Sectors per track for interrupt 0x13. Not used otherwise. */
  uint16_t sectorsPerTrtack;
           /** Number of heads for interrupt 0x13.  Not used otherwise. */
  uint16_t headCount;
          /**
           * Count of hidden sectors preceding the partition that contains this
           * FAT volume. This field is generally only relevant for media
           *  visible on interrupt 0x13.
           */
  uint32_t hidddenSectors;
          /**
           * This field is the new 32-bit total count of sectors on the volume.
           * This count includes the count of all sectors in all four regions
           * of the volume.  This field can be 0; if it is 0, then
           * totalSectors16 must be non-zero.
           */
  uint32_t totalSectors32;
          /**
           * Count of sectors occupied by one FAT on FAT32 volumes.
           */
  uint32_t sectorsPerFat32;
          /**
           * This field is only defined for FAT32 media and does not exist on
           * FAT12 and FAT16 media.
           * Bits 0-3 -- Zero-based number of active FAT.
           *             Only valid if mirroring is disabled.
           * Bits 4-6 -- Reserved.
           * Bit 7	-- 0 means the FAT is mirrored at runtime into all FATs.
	         *        -- 1 means only one FAT is active; it is the one referenced in bits 0-3.
           * Bits 8-15 	-- Reserved.
           */
  uint16_t fat32Flags;
          /**
           * FAT32 version. High byte is major revision number.
           * Low byte is minor revision number. Only 0.0 define.
           */
  uint16_t fat32Version;
          /**
           * Cluster number of the first cluster of the root directory for FAT32.
           * This usually 2 but not required to be 2.
           */
  uint32_t fat32RootCluster;
          /**
           * Sector number of FSINFO structure in the reserved area of the
           * FAT32 volume. Usually 1.
           */
  uint16_t fat32FSInfo;
          /**
           * If non-zero, indicates the sector number in the reserved area
           * of the volume of a copy of the boot record. Usually 6.
           * No value other than 6 is recommended.
           */
  uint16_t fat32BackBootBlock;
          /**
           * Reserved for future expansion. Code that formats FAT32 volumes
           * should always set all of the bytes of this field to 0.
           */
  uint8_t  fat32Reserved[12];
};
/** Type name for biosParmBlock */
typedef struct biosParmBlock bpb_t;
//------------------------------------------------------------------------------
/**
 * \struct fat32BootSector
 *
 * \brief Boot sector for a FAT16 or FAT32 volume.
 * 
 */  
struct fat32BootSector {
           /** X86 jmp to boot program */
  uint8_t  jmpToBootCode[3];
           /** informational only - don't depend on it */
  char     oemName[8];
           /** BIOS Parameter Block */
  bpb_t    bpb;
           /** for int0x13 use value 0X80 for hard drive */
  uint8_t  driveNumber;
           /** used by Windows NT - should be zero for FAT */
  uint8_t  reserved1;
           /** 0X29 if next three fields are valid */
  uint8_t  bootSignature;
           /** usually generated by combining date and time */
  uint32_t volumeSerialNumber;
           /** should match volume label in root dir */
  char     volumeLabel[11];
           /** informational only - don't depend on it */
  char     fileSystemType[8];
           /** X86 boot code */
  uint8_t  bootCode[420];
           /** must be 0X55 */
  uint8_t  bootSectorSig0;
           /** must be 0XAA */
  uint8_t  bootSectorSig1;
};
//------------------------------------------------------------------------------
// End Of Chain values for FAT entries
/** FAT16 end of chain value used by Microsoft. */
uint16_t const FAT16EOC = 0XFFFF;
/** Minimum value for FAT16 EOC.  Use to test for EOC. */
uint16_t const FAT16EOC_MIN = 0XFFF8;
/** FAT32 end of chain value used by Microsoft. */
uint32_t const FAT32EOC = 0X0FFFFFFF;
/** Minimum value for FAT32 EOC.  Use to test for EOC. */
uint32_t const FAT32EOC_MIN = 0X0FFFFFF8;
/** Mask a for FAT32 entry. Entries are 28 bits. */
uint32_t const FAT32MASK = 0X0FFFFFFF;

/** Type name for fat32BootSector */
typedef struct fat32BootSector fbs_t;
//------------------------------------------------------------------------------
/**
 * \struct directoryEntry
 * \brief FAT short directory entry
 *
 * Short means short 8.3 name, not the entry size.
 *  
 */
struct directoryEntry {
           /**
            * Short 8.3 name.
            * The first eight bytes contain the file name with blank fill.
            * The last three bytes contain the file extension with blank fill.
            */
  uint8_t  name[11];
  uint8_t  attributes;
          /**
           * Reserved for use by Windows NT. Set value to 0 when a file is
           * created and never modify or look at it after that.
           */
  uint8_t  reservedNT;
          /**
           * The granularity of the seconds part of creationTime is 2 seconds
           * so this field is a count of tenths of a second and its valid
           * value range is 0-199 inclusive. (WHG note - seems to be hundredths)
           */
  uint8_t  creationTimeTenths;
  uint16_t creationTime;
  uint16_t creationDate;
  uint16_t lastAccessDate;
          /**
           * High word of this entry's first cluster number (always 0 for a
           * FAT12 or FAT16 volume).
           */
  uint16_t firstClusterHigh;
  uint16_t lastWriteTime;
  uint16_t lastWriteDate;
           /** Low word of this entry's first cluster number. */
  uint16_t firstClusterLow;
           /** 32-bit unsigned holding this file's size in bytes. */
  uint32_t fileSize;
};
//------------------------------------------------------------------------------
// Definitions for directory entries
//
/** Type name for directoryEntry */
typedef struct directoryEntry dir_t;
/** name[0] value for entry that is free after being "deleted" */
uint8_t const DIR_NAME_DELETED = 0XE5;
/** name[0] value for entry that is free and no allocated entries follow */
uint8_t const DIR_NAME_FREE = 0X00;
/** file is read-only */
uint8_t const DIR_ATT_READ_ONLY = 0X01;
/** File should hidden in directory listings */
uint8_t const DIR_ATT_HIDDEN = 0X02;
/** Entry is for a system file */
uint8_t const DIR_ATT_SYSTEM = 0X04;
/** Directory entry contains the volume label */
uint8_t const DIR_ATT_VOLUME_ID = 0X08;
/** Entry is for a directory */
uint8_t const DIR_ATT_DIRECTORY = 0X10;
/** Old DOS archive bit for backup support */
uint8_t const DIR_ATT_ARCHIVE = 0X20;
/** Test mask for long name entry */
uint8_t const DIR_ATT_LONG_NAME_MASK = 0X3F;
/** defined attribute bits */
uint8_t const DIR_ATT_DEFINED_BITS = 0X3F;
/** Mask for file/subdirectory tests */
uint8_t const DIR_ATT_FILE_TYPE_MASK = (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY);

//------------------------------------------------------------------------------
// forward declaration since SdVolume is used in SdFile
class SdVolume;
//==============================================================================
// SdFile class
// values for type_
/** This SdFile has not been opened. */
uint8_t const FAT_FILE_TYPE_CLOSED = 0;
/** SdFile for a file */
uint8_t const FAT_FILE_TYPE_NORMAL = 1;
/** SdFile for a FAT16 root directory */
uint8_t const FAT_FILE_TYPE_ROOT16 = 2;
/** SdFile for a FAT32 root directory */
uint8_t const FAT_FILE_TYPE_ROOT32 = 3;
/** Test value for directory type */
uint8_t const FAT_FILE_TYPE_MIN_DIR = FAT_FILE_TYPE_ROOT16;

//------------------------------------------------------------------------------
/**
 * \class SdFile
 * \brief Access FAT16 and FAT32 files on SD and SDHC cards.
 */
class SdFile : public Print {
 public:
  /** Create an instance of SdFile. */
  SdFile(void) : type_(FAT_FILE_TYPE_CLOSED) {}
  /**
   * writeError is set to true if an error occurs during a write().
   * Set writeError to false before calling print() and/or write() and check
   * for true after calls to print() and/or write().
   */
  //bool writeError;
  uint8_t close(void);
  /** \return The current position for a file or directory. */
  uint32_t curPosition(void) const {return curPosition_;}
  /** \return The total number of bytes in a file or directory. */
  uint32_t fileSize(void) const {return fileSize_;}
  /** \return True if this is a SdFile for a directory else false. */
  uint8_t isDir(void) const {return type_ >= FAT_FILE_TYPE_MIN_DIR;}
  /** \return True if this is a SdFile for an open file/directory else false. */
  uint8_t isOpen(void) const {return type_ != FAT_FILE_TYPE_CLOSED;}

  uint8_t open02(SdFile* dirFile, const char* fileName);
  uint8_t openRoot(SdVolume* vol);
  /**
   * Read the next byte from a file.
   *
   * \return For success read returns the next byte in the file as an int.
   * If an error occurs or end of file is reached -1 is returned.
   */
  int16_t read(void) {
    uint8_t b;
    return read(&b, 1) == 1 ? b : -1;
  }
  
  int16_t read(void* buf, uint16_t nbyte);
  
  /** Set the file's current position to zero. */
  void rewind(void) {
    curPosition_ = curCluster_ = 0;
  }
  /** Set the files position to current position + \a pos. See seekSet(). */
  uint8_t seekCur(uint32_t pos) {
    return seekSet(curPosition_ + pos);
  }
  uint8_t seekSet(uint32_t pos);
  uint8_t sync(void);
  /** Type of this SdFile.  You should use isFile() or isDir() instead of type()
   * if possible.
   *
   * \return The file or directory type.
   */
  uint8_t type(void) const {return type_;}
  uint8_t truncate(uint32_t size);
  /** \return SdVolume that contains this file. */
  SdVolume* volume(void) const {return vol_;}
  size_t write(uint8_t b);
  size_t write(const void* buf, uint16_t nbyte);
  size_t write(const char* str);
  void write_P(PGM_P str);
  void writeln_P(PGM_P str);
//------------------------------------------------------------------------------
 private:

  // private data
  uint8_t   flags_;         // is F_FILE_DIR_DIRTY
  uint8_t   type_;          // type of file see above for values
  uint32_t  curCluster_;    // cluster for current file position
  uint32_t  curPosition_;   // current file position in bytes from beginning
  uint32_t  dirBlock_;      // SD block that contains directory entry for file
  uint8_t   dirIndex_;      // index of entry in dirBlock 0 <= dirIndex_ <= 0XF
  uint32_t  fileSize_;      // file size in bytes
  uint32_t  firstCluster_;  // first cluster of file
  SdVolume* vol_;           // volume where file is located

  // private functions
  uint8_t addCluster(void);
  uint8_t addDirCluster(void);
  dir_t* cacheDirEntry(uint8_t action);
  static uint8_t make83Name(const char* str, uint8_t* name);
  uint8_t openCachedEntry(uint8_t cacheIndex);
  dir_t* readDirCache(void);
};
//==============================================================================
// SdVolume class
/**
 * \brief Cache for an SD data block
 */
union cache_t {
           /** Used to access cached file data blocks. */
  uint8_t  data[512];
           /** Used to access cached FAT16 entries. */
  uint16_t fat16[256];
           /** Used to access cached FAT32 entries. */
  uint32_t fat32[128];
           /** Used to access cached directory entries. */
  dir_t    dir[16];
           /** Used to access a cached MasterBoot Record. */
  mbr_t    mbr;
           /** Used to access to a cached FAT boot sector. */
  fbs_t    fbs;
};
//------------------------------------------------------------------------------
/**
 * \class SdVolume
 * \brief Access FAT16 and FAT32 volumes on SD and SDHC cards.
 */
class SdVolume {
 public:
  /** Create an instance of SdVolume */
  SdVolume(void) :allocSearchStart_(2), fatType_(0) {}

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
  uint8_t volumeinit(Sd2Card* dev, uint8_t part);

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
  static Sd2Card* sdCard(void) {return sdCard_;}
//------------------------------------------------------------------------------
  private:
  // Allow SdFile access to SdVolume private data.
  friend class SdFile;

  // value for action argument in cacheRawBlock to indicate read from cache
  static uint8_t const CACHE_FOR_READ = 0;
  // value for action argument in cacheRawBlock to indicate cache dirty
  static uint8_t const CACHE_FOR_WRITE = 1;

  static cache_t cacheBuffer_;        // 512 byte cache for device blocks
  static uint32_t cacheBlockNumber_;  // Logical number of block in the cache
  static Sd2Card* sdCard_;            // Sd2Card object for cache
  static uint8_t cacheDirty_;         // cacheFlush() will write block if true
  static uint32_t cacheMirrorBlock_;  // block number for mirror FAT
//
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
  //----------------------------------------------------------------------------
  uint8_t allocContiguous(uint32_t* curCluster);
  uint8_t blockOfCluster(uint32_t position) const {return (position >> 9) & (blocksPerCluster_ - 1);}
  uint32_t clusterStartBlock(uint32_t cluster) const {return dataStartBlock_ + ((cluster - 2) << clusterSizeShift_);}
  uint32_t blockNumber(uint32_t cluster, uint32_t position) const {return clusterStartBlock(cluster) + blockOfCluster(position);}
  static uint8_t cacheFlush(void);
  static uint8_t cacheRawBlock(uint32_t blockNumber, uint8_t action);
  static void cacheSetDirty(void) {cacheDirty_ |= CACHE_FOR_WRITE;}
  static uint8_t cacheZeroBlock(uint32_t blockNumber);
  uint8_t chainSize(uint32_t beginCluster, uint32_t* size) const;
  uint8_t fatGet(uint32_t cluster, uint32_t* value) const;
  uint8_t fatPut(uint32_t cluster, uint32_t value);
  uint8_t fatPutEOC(uint32_t cluster) {return fatPut(cluster, 0x0FFFFFFF);}
  uint8_t isEOC(uint32_t cluster) const {return  cluster >= (fatType_ == 16 ? FAT16EOC_MIN : FAT32EOC_MIN);}
  uint8_t readBlock(uint32_t block, uint8_t* dst) {return sdCard_->readBlock(block, dst);}
  uint8_t readData(uint32_t block, uint16_t offset, uint16_t count, uint8_t* dst) {return sdCard_->readData(block, offset, count, dst);}
  uint8_t writeBlock(uint32_t block, const uint8_t* dst) {return sdCard_->writeBlock(block, dst);}
};


class File : public Stream {
 private:
  SdFile *_file;  // underlying file pointer

public:
  void setSDFile(SdFile* f);     // wraps an underlying SdFile
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  virtual int read();
  virtual int peek();
  virtual int available();
  virtual void flush();
  int read(void *buf, uint16_t nbyte);
  boolean seek(uint32_t pos);
  uint32_t position();
  uint32_t size();
  void close();
  
  using Print::write;
};




//------------------------------------------------------------------------------
/** Soft SPI receive */
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
/** Soft SPI send */
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

//------------------------------------------------------------------------------
// send command and return error code.  Return zero for OK
uint8_t Sd2Card::cardCommand(uint8_t cmd, uint32_t arg) {
  // end read if in partialBlockRead mode
  readEnd();
  // select card
  SPI_CHIP_SELECT_LOW();
  // wait up to 300 ms if busy
  waitNotBusy(300);
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
  for (uint8_t i = 0; ((status_ = spiRec()) & 0X80) && i != 0XFF; i++);
  return status_;
}
//------------------------------------------------------------------------------
uint8_t Sd2Card::cardinit() {
  inBlock_ = type_ = 0;
  // 16-bit init start time allows over a minute
  uint16_t t0 = (uint16_t)millis();
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
  while ((status_ = cardCommand(CMD0, 0)) != R1_IDLE_STATE) {
    if (((uint16_t)millis() - t0) > SD_INIT_TIMEOUT) {
      errCode = 1;
      goto fail;
    }
  }
  // check SD version
  if ((cardCommand(CMD8, 0x1AA) & R1_ILLEGAL_COMMAND)) {
    type(SD_CARD_TYPE_SD1);
  } else {
    // only need last byte of r7 response
    for (uint8_t i = 0; i < 4; i++) status_ = spiRec();
    if (status_ != 0XAA) {
      errCode = 2;
      goto fail;
    }
    type(SD_CARD_TYPE_SD2);
  }
  // initialize card and send host supports SDHC if SD2
  arg = type() == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;

  while ((status_ = cardAcmd(ACMD41, arg)) != R1_READY_STATE) {
    // check for timeout
    if (((uint16_t)millis() - t0) > SD_INIT_TIMEOUT) {
      errCode = 3;
      goto fail;
    }
  }
  // if SD2 read OCR register to check for SDHC card
  if (type() == SD_CARD_TYPE_SD2) {
    if (cardCommand(CMD58, 0)) {
      errCode = 5;
      goto fail;
    }
    if ((spiRec() & 0XC0) == 0XC0) type(SD_CARD_TYPE_SDHC);
    // discard rest of ocr - contains allowed voltage range
    for (uint8_t i = 0; i < 3; i++) spiRec();
  }
  SPI_CHIP_SELECT_HIGH();

  return true;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return false;
}
//------------------------------------------------------------------------------
/**
 * Read a 512 byte block from an SD card device.
 *
 * \param[in] block Logical block to be read.
 * \param[out] dst Pointer to the location that will receive the data.

 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t Sd2Card::readBlock(uint32_t block, uint8_t* dst) {
  return readData(block, 0, 512, dst);
}
//------------------------------------------------------------------------------
/**
 * Read part of a 512 byte block from an SD card.
 *
 * \param[in] block Logical block to be read.
 * \param[in] offset Number of bytes to skip at start of block
 * \param[out] dst Pointer to the location that will receive the data.
 * \param[in] count Number of bytes to read
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t Sd2Card::readData(uint32_t block, uint16_t offset, uint16_t count, uint8_t* dst) {
  if (count == 0) return true;
  if ((count + offset) > 512) {
    goto fail;
  }
  if (!inBlock_ || block != block_ || offset < offset_) {
    block_ = block;
    // use address if not SDHC card
    if (type()!= SD_CARD_TYPE_SDHC) block <<= 9;
    if (cardCommand(CMD17, block)) {
      errCode = 6;
      goto fail;
    }
    if (!waitStartBlock()) {
      goto fail;
    }
    offset_ = 0;
    inBlock_ = 1;
  }


  // skip data before offset
  for (;offset_ < offset; offset_++) {
    spiRec();
  }
  // transfer data
  for (uint16_t i = 0; i < count; i++) {
    dst[i] = spiRec();
  }

  offset_ += count;
  readEnd();
  return true;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return false;
}
//------------------------------------------------------------------------------
/** Skip remaining data in a block when in partial block read mode. */
void Sd2Card::readEnd(void) {
  if (inBlock_) {
      // skip data and crc
    while (offset_++ < 514) spiRec();
    SPI_CHIP_SELECT_HIGH();
    inBlock_ = 0;
  }
}
//------------------------------------------------------------------------------
// wait for card to go not busy
uint8_t Sd2Card::waitNotBusy(uint16_t timeoutMillis) {
  uint16_t t0 = millis();
  do {
    if (spiRec() == 0XFF) return true;
  }
  while (((uint16_t)millis() - t0) < timeoutMillis);
  return false;
}
//------------------------------------------------------------------------------
/** Wait for start block token */
uint8_t Sd2Card::waitStartBlock(void) {
  uint16_t t0 = millis();
  while ((status_ = spiRec()) == 0XFF) {
    if (((uint16_t)millis() - t0) > SD_READ_TIMEOUT) {
      errCode = 8;
      goto fail;
    }
  }
  if (status_ != DATA_START_BLOCK) {
    errCode = 9;
    goto fail;
  }
  return true;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return false;
}
//------------------------------------------------------------------------------
/**
 * Writes a 512 byte block to an SD card.
 *
 * \param[in] blockNumber Logical block to be written.
 * \param[in] src Pointer to the location of the data to be written.
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t Sd2Card::writeBlock(uint32_t blockNumber, const uint8_t* src) {

  // use address if not SDHC card
  if (type() != SD_CARD_TYPE_SDHC) blockNumber <<= 9;
  if (cardCommand(CMD24, blockNumber)) {
    errCode = 10;
    goto fail;
  }
  if (!writeData(DATA_START_BLOCK, src)) goto fail;

  // wait for flash programming to complete
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) {
    errCode = 11;
    goto fail;
  }
  // response is r2 so get and check two bytes for nonzero
  if (cardCommand(CMD13, 0) || spiRec()) {
    errCode = 12;
    goto fail;
  }
  SPI_CHIP_SELECT_HIGH();
  return true;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return false;
}
//------------------------------------------------------------------------------
/** Write one data block in a multiple block write sequence */
uint8_t Sd2Card::writeData(const uint8_t* src) {
  // wait for previous write to finish
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) {
    errCode = 13;
    SPI_CHIP_SELECT_HIGH();
    return false;
  }
  return writeData(WRITE_MULTIPLE_TOKEN, src);
}
//------------------------------------------------------------------------------
// send one block of data for write block or write multiple blocks
uint8_t Sd2Card::writeData(uint8_t token, const uint8_t* src) {
  spiSend(token);
  for (uint16_t i = 0; i < 512; i++) {
    spiSend(src[i]);
  }
  spiSend(0xff);  // dummy crc
  spiSend(0xff);  // dummy crc

  status_ = spiRec();
  if ((status_ & DATA_RES_MASK) != DATA_RES_ACCEPTED) {
    errCode = 14;
    SPI_CHIP_SELECT_HIGH();
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
// add a cluster to a file
uint8_t SdFile::addCluster() {
  if (!vol_->allocContiguous(&curCluster_)) return false;

  // if first cluster of file link to directory entry
  if (firstCluster_ == 0) {
    firstCluster_ = curCluster_;
    flags_ = 1;
  }
  return true;
}
//------------------------------------------------------------------------------
// Add a cluster to a directory file and zero the cluster.
// return with first block of cluster in the cache
uint8_t SdFile::addDirCluster(void) {
  if (!addCluster()) return false;

  // zero data in cluster insure first cluster is in cache
  uint32_t block = vol_->clusterStartBlock(curCluster_);
  for (uint8_t i = vol_->blocksPerCluster_; i != 0; i--) {
    if (!SdVolume::cacheZeroBlock(block + i - 1)) return false;
  }
  // Increase directory file size by cluster size
  fileSize_ += 512UL << vol_->clusterSizeShift_;
  return true;
}
//------------------------------------------------------------------------------
// cache a file's directory entry
// return pointer to cached entry or null for failure
dir_t* SdFile::cacheDirEntry(uint8_t action) {
  if (!SdVolume::cacheRawBlock(dirBlock_, action)) return NULL;
  return SdVolume::cacheBuffer_.dir + dirIndex_;
}
//------------------------------------------------------------------------------
/**
 *  Close a file and force cached data and directory information
 *  to be written to the storage device.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include no file is open or an I/O error.
 */
uint8_t SdFile::close(void) {
  if (!sync())return false;
  type_ = FAT_FILE_TYPE_CLOSED;
  return true;
}
//------------------------------------------------------------------------------
uint8_t SdFile::open02(SdFile* dirFile, const char* dname) {
  dir_t* p;

  vol_ = dirFile->vol_;
  dirFile->rewind();

  // bool for empty entry found
  uint8_t emptyFound = false;

  // search for file
  while (dirFile->curPosition_ < dirFile->fileSize_) {
    uint8_t index = 0XF & (dirFile->curPosition_ >> 5);
    p = dirFile->readDirCache();
    if (p == NULL){errCode = 103;return false;}

    if (p->name[0] == DIR_NAME_FREE || p->name[0] == DIR_NAME_DELETED) {
      // remember first empty slot
      if (!emptyFound) {
        emptyFound = true;
        dirIndex_ = index;
        dirBlock_ = SdVolume::cacheBlockNumber_;
      }
      // done if no entries follow
      if (p->name[0] == DIR_NAME_FREE) break;
    } else if (!memcmp(dname, p->name, 11)) {

      // open found file
      return openCachedEntry(0XF & index);
    }
  }

  // cache found slot or add cluster if end of file
  if (emptyFound) {
    p = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
    if (!p){errCode = 106; return false;}
  } else {
    if (dirFile->type_ == FAT_FILE_TYPE_ROOT16){errCode = 107;  return false;}

    // add and zero cluster for dirFile - first cluster is in cache for write
    if (!dirFile->addDirCluster()){errCode = 108;  return false;}

    // use first entry in cluster
    dirIndex_ = 0;
    p = SdVolume::cacheBuffer_.dir;
  }
  // initialize as empty file
  memset(p, 0, sizeof(dir_t));
  memcpy(p->name, dname, 11);

  // force write of entry to SD
  if (!SdVolume::cacheFlush()){errCode = 109;  return false;}

  // open entry in cache
  return openCachedEntry(dirIndex_);
}
//------------------------------------------------------------------------------
// open a cached directory entry. Assumes vol_ is initializes
uint8_t SdFile::openCachedEntry(uint8_t dirIndex) {
  // location of entry in cache
  dir_t* p = SdVolume::cacheBuffer_.dir + dirIndex;

  // remember location of directory entry on SD
  dirIndex_ = dirIndex;
  dirBlock_ = SdVolume::cacheBlockNumber_;

  // copy first cluster number for directory fields
  firstCluster_ = (uint32_t)p->firstClusterHigh << 16;
  firstCluster_ |= p->firstClusterLow;

  // make sure it is a normal file or subdirectory
  fileSize_ = p->fileSize;
  type_ = FAT_FILE_TYPE_NORMAL;

  // set to start of file
  curCluster_ = 0;
  curPosition_ = 0;
  return true;
}
//------------------------------------------------------------------------------
/**
 * Open a volume's root directory.
 *
 * \param[in] vol The FAT volume containing the root directory to be opened.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include the FAT volume has not been initialized
 * or it a FAT12 volume.
 */
uint8_t SdFile::openRoot(SdVolume* vol) {
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

  // set to start of file
  curCluster_ = 0;
  curPosition_ = 0;

  // root has no directory entry
  dirBlock_ = 0;
  dirIndex_ = 0;
  return true;
}
//------------------------------------------------------------------------------
/**
 * Read data from a file starting at the current position.
 *
 * \param[out] buf Pointer to the location that will receive the data.
 *
 * \param[in] nbyte Maximum number of bytes to read.
 *
 * \return For success read() returns the number of bytes read.
 * A value less than \a nbyte, including zero, will be returned
 * if end of file is reached.
 * If an error occurs, read() returns -1.  Possible errors include
 * read() called before a file has been opened, corrupt file system
 * or an I/O error occurred.
 */
int16_t SdFile::read(void* buf, uint16_t nbyte) {
  uint8_t* dst = reinterpret_cast<uint8_t*>(buf);

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
          if (!vol_->fatGet(curCluster_, &curCluster_)) return -1;
        }
      }
      block = vol_->clusterStartBlock(curCluster_) + blockOfCluster;
    }
    uint16_t n = toRead;

    // amount to be read from current block
    if (n > (512 - offset)) n = 512 - offset;

    // no buffering needed if n == 512 or user requests no buffering
    if ((n == 512) && block != SdVolume::cacheBlockNumber_) {
      if (!vol_->readData(block, offset, n, dst)) return -1;
      dst += n;
    } else {
      // read block to cache and copy data to caller
      if (!SdVolume::cacheRawBlock(block, SdVolume::CACHE_FOR_READ)) return -1;
      uint8_t* src = SdVolume::cacheBuffer_.data + offset;
      uint8_t* end = src + n;
      while (src != end) *dst++ = *src++;
    }
    curPosition_ += n;
    toRead -= n;
  }
  return nbyte;
}
//------------------------------------------------------------------------------
// Read next directory entry into the cache
// Assumes file is correctly positioned
dir_t* SdFile::readDirCache(void) {
  // error if not directory
  if (!isDir()) return NULL;

  // index of entry in cache
  uint8_t i = (curPosition_ >> 5) & 0XF;

  // use read to locate and cache block
  if (read() < 0) return NULL;

  // advance to next entry
  curPosition_ += 31;

  // return pointer to entry
  return (SdVolume::cacheBuffer_.dir + i);
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
uint8_t SdFile::seekSet(uint32_t pos) {
  // error if file not open or seek past end of file
  if (!isOpen() || pos > fileSize_){errCode = 80;  return false;}

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
    if (!vol_->fatGet(curCluster_, &curCluster_)){errCode = 81;  return false;}
  }
  curPosition_ = pos;
  return true;
}
//------------------------------------------------------------------------------
/**
 * The sync() call causes all modified data and directory fields
 * to be written to the storage device.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include a call to sync() before a file has been
 * opened or an I/O error.
 */
uint8_t SdFile::sync(void) {
  // only allow open files and directories
  if (!isOpen()) return false;

  if (flags_) {
    dir_t* d = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
    if (!d) return false;

    // do not set filesize for dir files
    if (!isDir()) d->fileSize = fileSize_;

    // update first cluster fields
    d->firstClusterLow = firstCluster_ & 0XFFFF;
    d->firstClusterHigh = firstCluster_ >> 16;

    // clear directory dirty
    flags_ = 0;
  }
  return SdVolume::cacheFlush();
}
//------------------------------------------------------------------------------
/**
 * Write data to an open file.
 *
 * \note Data is moved to the cache but may not be written to the
 * storage device until sync() is called.
 *
 * \param[in] buf Pointer to the location of the data to be written.
 *
 * \param[in] nbyte Number of bytes to write.
 *
 * \return For success write() returns the number of bytes written, always
 * \a nbyte.  If an error occurs, write() returns -1.  Possible errors
 * include write() is called before a file has been opened, write is called
 * for a read-only file, device is full, a corrupt file system or an I/O error.
 *
 */
size_t SdFile::write(const void* buf, uint16_t nbyte) {
  // convert void* to uint8_t*  -  must be before goto statements
  const uint8_t* src = reinterpret_cast<const uint8_t*>(buf);

  // number of bytes left to write  -  must be before goto statements
  uint16_t nToWrite = nbyte;

  while (nToWrite > 0) {
    uint8_t blockOfCluster = vol_->blockOfCluster(curPosition_);
    uint16_t blockOffset = curPosition_ & 0X1FF;
    if (blockOfCluster == 0 && blockOffset == 0) {
      // start of new cluster
      if (curCluster_ == 0) {
        if (firstCluster_ == 0) {
          // allocate first cluster of file
          if (!addCluster())   return 0;
        } else {
          curCluster_ = firstCluster_;
        }
      } else {
        uint32_t next;
        if (!vol_->fatGet(curCluster_, &next)) return false;
        if (vol_->isEOC(next)) {
          // add cluster if at end of chain
          if (!addCluster())   return 0;
        } else {
          curCluster_ = next;
        }
      }
    }
    // max space in block
    uint16_t n = 512 - blockOffset;

    // lesser of space and amount to write
    if (n > nToWrite) n = nToWrite;

    // block for data write
    uint32_t block = vol_->clusterStartBlock(curCluster_) + blockOfCluster;
    if (n == 512) {
      // full block - don't need to use cache
      // invalidate cache if block is in cache
      if (SdVolume::cacheBlockNumber_ == block) {
        SdVolume::cacheBlockNumber_ = 0XFFFFFFFF;
      }
      if (!vol_->writeBlock(block, src)) return 0;
      src += 512;
    } else {
      if (blockOffset == 0 && curPosition_ >= fileSize_) {
        // start of new block don't need to read into cache
        if (!SdVolume::cacheFlush()) return 0;
        SdVolume::cacheBlockNumber_ = block;
        SdVolume::cacheSetDirty();
      } else {
        // rewrite part of block
        if (!SdVolume::cacheRawBlock(block, SdVolume::CACHE_FOR_WRITE)) {
          return 0;
        }
      }
      uint8_t* dst = SdVolume::cacheBuffer_.data + blockOffset;
      uint8_t* end = dst + n;
      while (dst != end) *dst++ = *src++;
    }
    nToWrite -= n;
    curPosition_ += n;
  }
  if (curPosition_ > fileSize_) {
    // update fileSize and insure sync will update dir entry
    fileSize_ = curPosition_;
    flags_ = 1;
  }

  return nbyte;

}
//------------------------------------------------------------------------------
/**
 * Write a byte to a file. Required by the Arduino Print class.
 *
 * Use SdFile::writeError to check for errors.
 */
size_t SdFile::write(uint8_t b) {
  return write(&b, 1);
}
//------------------------------------------------------------------------------
/**
 * Write a string to a file. Used by the Arduino Print class.
 *
 * Use SdFile::writeError to check for errors.
 */
size_t SdFile::write(const char* str) {
  return write(str, strlen(str));
}
//------------------------------------------------------------------------------
/**
 * Write a PROGMEM string to a file.
 *
 * Use SdFile::writeError to check for errors.
 */
void SdFile::write_P(PGM_P str) {
  for (uint8_t c; (c = pgm_read_byte(str)); str++) write(c);
}
//------------------------------------------------------------------------------
/**
 * Write a PROGMEM string followed by CR/LF to a file.
 *
 * Use SdFile::writeError to check for errors.
 */
void SdFile::writeln_P(PGM_P str) {
  write_P(str);
  println();
}



//------------------------------------------------------------------------------
// raw block cache
// init cacheBlockNumber_to invalid SD block number
uint32_t SdVolume::cacheBlockNumber_ = 0XFFFFFFFF;
cache_t  SdVolume::cacheBuffer_;     // 512 byte cache for Sd2Card
Sd2Card* SdVolume::sdCard_;          // pointer to SD card object
uint8_t  SdVolume::cacheDirty_ = 0;  // cacheFlush() will write block if true
uint32_t SdVolume::cacheMirrorBlock_ = 0;  // mirror  block for second FAT
//------------------------------------------------------------------------------
// find a contiguous group of clusters
uint8_t SdVolume::allocContiguous(uint32_t* curCluster) {
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
    setStart = true;
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
    } else if (endCluster == bgnCluster) {
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
//------------------------------------------------------------------------------
uint8_t SdVolume::cacheFlush(void) {
  if (cacheDirty_) {
    if (!sdCard_->writeBlock(cacheBlockNumber_, cacheBuffer_.data)) {
      return false;
    }
    // mirror FAT tables
    if (cacheMirrorBlock_) {
      if (!sdCard_->writeBlock(cacheMirrorBlock_, cacheBuffer_.data)) {
        return false;
      }
      cacheMirrorBlock_ = 0;
    }
    cacheDirty_ = 0;
  }
  return true;
}
//------------------------------------------------------------------------------
uint8_t SdVolume::cacheRawBlock(uint32_t blockNumber, uint8_t action) {
  if (cacheBlockNumber_ != blockNumber) {
    if (!cacheFlush()) return false;
    if (!sdCard_->readBlock(blockNumber, cacheBuffer_.data)) return false;
    cacheBlockNumber_ = blockNumber;
  }
  cacheDirty_ |= action;
  return true;
}
//------------------------------------------------------------------------------
// cache a zero block for blockNumber
uint8_t SdVolume::cacheZeroBlock(uint32_t blockNumber) {
  if (!cacheFlush()) return false;

  // loop take less flash than memset(cacheBuffer_.data, 0, 512);
  for (uint16_t i = 0; i < 512; i++) {
    cacheBuffer_.data[i] = 0;
  }
  cacheBlockNumber_ = blockNumber;
  cacheSetDirty();
  return true;
}
//------------------------------------------------------------------------------
// return the size in bytes of a cluster chain
uint8_t SdVolume::chainSize(uint32_t cluster, uint32_t* size) const {
  uint32_t s = 0;
  do {
    if (!fatGet(cluster, &cluster)) return false;
    s += 512UL << clusterSizeShift_;
  } while (!isEOC(cluster));
  *size = s;
  return true;
}
//------------------------------------------------------------------------------
// Fetch a FAT entry
uint8_t SdVolume::fatGet(uint32_t cluster, uint32_t* value) const {
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
//------------------------------------------------------------------------------
// Store a FAT entry
uint8_t SdVolume::fatPut(uint32_t cluster, uint32_t value) {
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
uint8_t SdVolume::volumeinit(Sd2Card* dev, uint8_t part) {
  uint32_t volumeStartBlock = 0;
  sdCard_ = dev;
  // if part == 0 assume super floppy with FAT boot sector in block zero
  // if part > 0 assume mbr volume with partition table
  if (part) {
    if (!cacheRawBlock(volumeStartBlock, CACHE_FOR_READ)) return false;
    part_t* p = &cacheBuffer_.mbr.part[part-1];
    if ((p->boot & 0X7F) !=0  ||
      p->totalSectors < 100 ||
      p->firstSector == 0) {
      // not a valid partition
      return false;
    }
    volumeStartBlock = p->firstSector;
  }
  if (!cacheRawBlock(volumeStartBlock, CACHE_FOR_READ)) return false;
  bpb_t* bpb = &cacheBuffer_.fbs.bpb;
  if (bpb->bytesPerSector != 512 ||
    bpb->fatCount == 0 ||
    bpb->reservedSectorCount == 0 ||
    bpb->sectorsPerCluster == 0) {
       // not valid FAT volume
      return false;
  }
  fatCount_ = bpb->fatCount;
  blocksPerCluster_ = bpb->sectorsPerCluster;

  // determine shift that is same as multiply by blocksPerCluster_
  clusterSizeShift_ = 0;
  while (blocksPerCluster_ != (1 << clusterSizeShift_)) {
    // error if not power of 2
    if (clusterSizeShift_++ > 7) return false;
  }
  blocksPerFat_ = bpb->sectorsPerFat16 ? bpb->sectorsPerFat16 : bpb->sectorsPerFat32;

  fatStartBlock_ = volumeStartBlock + bpb->reservedSectorCount;

  // count for FAT16 zero for FAT32
  rootDirEntryCount_ = bpb->rootDirEntryCount;

  // directory start for FAT16 dataStart for FAT32
  rootDirStart_ = fatStartBlock_ + bpb->fatCount * blocksPerFat_;

  // data start for FAT16 and FAT32
  dataStartBlock_ = rootDirStart_ + ((32 * bpb->rootDirEntryCount + 511)/512);

  // total blocks for FAT16 or FAT32
  uint32_t totalBlocks = bpb->totalSectors16 ? bpb->totalSectors16 : bpb->totalSectors32;
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
    rootDirStart_ = bpb->fat32RootCluster;
    fatType_ = 32;
  }
  return true;
}

void File::setSDFile(SdFile* f) {
  _file = f; 
}

size_t File::write(uint8_t val) {
  return write(&val, 1);
}

size_t File::write(const uint8_t *buf, size_t size) {
  return _file->write(buf, size);
}

int File::peek() {
  int c = _file->read();
  if (c != -1) _file->seekCur(-1);
  return c;
}

int File::read() {
  return _file->read();
}

// buffered read for more efficient, high speed reading
int File::read(void *buf, uint16_t nbyte) {
  return _file->read(buf, nbyte);
}

int File::available() {
  uint32_t n = size() - position();
  return n > 0X7FFF ? 0X7FFF : n;
}

void File::flush() {
  _file->sync();
}

boolean File::seek(uint32_t pos) {
  return _file->seekSet(pos);
}

uint32_t File::position() {
  return _file->curPosition();
}

uint32_t File::size() {
  return _file->fileSize();
}

void File::close() {
  _file->close();
}


class SDClass {

public:
  // These are required for initialisation and use of sdfatlib
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  SdFile sdfile;
  File file;
  
  boolean begin() {
    if(!card.cardinit())return false;
    if(!volume.volumeinit(&card, 1))
    {
        if(!volume.volumeinit(&card, 0))
        {
          return false;
        }
    }
    return root.openRoot(&volume);
  }

  // Open the specified file/directory with the supplied mode (e.g. read or
  // write, etc). Returns a File object for interacting with the file.
  // Note that currently only one file can be open at a time.
  uint8_t openSimple(const char *filepath, uint8_t toend) {
    // Open the file itself
    

    if (!sdfile.open02(&root, filepath)) {
      // failed to open the file :(
      return 0;
    }

    if (toend) 
      sdfile.seekSet(sdfile.fileSize());
    
    file.setSDFile(&sdfile);
    return 1;
  }

private:
  
  friend class File;
};


SDClass SD;





void dly()
{
  for(uint8_t i=0;i<6;i++)//6 is stable
  {
    volatile uint8_t v=0;
    v++;
  }
}
#define DDR_SCL  DDRD
#define PORT_SCL PORTD

#define DDR_SDA  DDRD
#define PORT_SDA PORTD
#define PIN_SDA  PIND

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

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



uint8_t BIT_SCL = 0;
uint8_t BIT_SDA = 0;

//
void i2c_SoftI2CMaster()
{
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
}
//
uint8_t i2c_beginTransmission(uint8_t address)
{
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 0); // clr read bit
  return rc;
}
//
uint8_t i2c_requestFrom(uint8_t address)
{
  i2c_start();
  uint8_t rc = i2c_write((address<<1) | 1); // set read bit
  return rc;
}
//
//
//
uint8_t i2c_endTransmission(void)
{
  i2c_stop();
  //return ret; // FIXME
  return 0;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
void i2c_write(uint8_t* data, uint8_t quantity)
{
  for(uint8_t i = 0; i < quantity; ++i){
    i2c_write(data[i]);
  }
}

//--------------------------------------------------------------------
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
//
uint8_t i2c_readbit(void)
{
  i2c_sda_hi();
  i2c_scl_hi();
  dly();
  uint8_t c = PIN_SDA; // I2C_PIN;
  i2c_scl_lo();
  dly();
  return ( c & BIT_SDA) ? 1 : 0;
}
// Send a START Condition
//
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
void i2c_repstart(void)
{
  // set both to high at the same time (releases drive on both lines)
  i2c_sda_hi();
  i2c_scl_hi();
  i2c_scl_lo(); // force SCL low
  dly();
  i2c_sda_release(); // release SDA
  dly();
  i2c_scl_release(); // release SCL
  dly();
  i2c_sda_lo(); // force SDA low
  dly();
}
// Send a STOP Condition
//
void i2c_stop(void)
{
  i2c_scl_hi();
  dly();
  i2c_sda_hi();
  dly();
}
// write a byte to the I2C slave device
//
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















#define BMP085_ADDRESS 0x77  // I2C address of BMP085
const unsigned char OSS = 3;  // Oversampling Setting ultra high resolution
// Calibration values
int ac1;
int ac2;
int ac3;
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1;
int b2;
int mb;
int mc;
int md;
long b5; 

int temperature; //MUST be called first
long pressure;
//float altitude; //Uncompensated caculation - in Meters 

// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Calculate temperature in deg C
int bmp085GetTemperature(unsigned int ut){
  long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  long temp = ((b5 + 8)>>4);

  return (int)temp;
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up){
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;

  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  long temp = p;
  return temp;
}

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  i2c_beginTransmission(BMP085_ADDRESS);
  i2c_write(address);
  i2c_endTransmission();
  i2c_requestFrom(BMP085_ADDRESS);
  uint8_t msb = i2c_readLast();
  return msb;
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;

  i2c_beginTransmission(BMP085_ADDRESS);
  i2c_write(address);
  i2c_endTransmission();

  i2c_requestFrom(BMP085_ADDRESS);

  msb = i2c_read();
  lsb = i2c_readLast();
  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT(){
  unsigned int ut;

  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  i2c_beginTransmission(BMP085_ADDRESS);
  uint8_t d1 = 0xF4;
  uint8_t d2 = 0x2E;
  i2c_write(d1);
  i2c_write(d2);
  i2c_endTransmission();

  // Wait at least 4.5ms
  delay(5);

  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP(){

  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;

  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  i2c_beginTransmission(BMP085_ADDRESS);
  uint8_t d1 = 0xF4;
  uint8_t d2 = 0x34 + (OSS<<6);
  i2c_write(d1);
  i2c_write(d2);
  i2c_endTransmission();

  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));

  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  msb = bmp085Read(0xF6);
  lsb = bmp085Read(0xF7);
  xlsb = bmp085Read(0xF8);

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

  return up;
}

float calcAltitude(float pressure){

  float A = pressure/101325;
  float B = 1/5.25588;
  float C = pow(A,B);
  C = 1 - C;
  C = C /0.0000225577;

  return C;
}







uint8_t DS1307_SEC;// 0
uint8_t DS1307_MIN;// 1
uint8_t DS1307_HR;// 2
uint8_t DS1307_DOW;// 3
uint8_t DS1307_DATE;// 4
uint8_t DS1307_MTH;// 5
uint8_t DS1307_YR;// 6

#define DS1307_CTRL_ID 0x68 //B01101000  //DS1307

uint8_t bcdToDec(const uint8_t bcd) {
  return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}

uint8_t decToBcd(const uint8_t dec) {
  const uint8_t tens = dec / 10;
  const uint8_t ones = dec % 10;
  return (tens << 4) | ones;
}

// Aquire data from the RTC chip in BCD format
// refresh the buffer
void DS1307_read()
{
  BIT_SCL = _BV(5);//DS1307
  BIT_SDA = _BV(4);//DS1307
  // use the Wire lib to connect to tho rtc
  // reset the resgiter pointer to zero
  i2c_beginTransmission(DS1307_CTRL_ID);
  i2c_write(0x00);
  i2c_endTransmission();

  // request the 7 bytes of data    (secs, min, hr, dow, date. mth, yr)
  i2c_requestFrom(DS1307_CTRL_ID);
  // store data in raw bcd format
  DS1307_SEC=bcdToDec(i2c_read());// 0
  DS1307_MIN=bcdToDec(i2c_read());// 1
  DS1307_HR=bcdToDec(i2c_read());// 2
  DS1307_DOW=i2c_read();// 3
  DS1307_DATE=bcdToDec(i2c_read());// 4
  DS1307_MTH=bcdToDec(i2c_read());// 5
  DS1307_YR=bcdToDec(i2c_readLast());// 6
}

// update the data on the IC from the bcd formatted data in the buffer
void DS1307_save()
{
  BIT_SCL = _BV(5);//DS1307
  BIT_SDA = _BV(4);//DS1307
  i2c_beginTransmission(DS1307_CTRL_ID);
  i2c_write(0x00); // reset register pointer
  i2c_write(decToBcd(DS1307_SEC));
  i2c_write(decToBcd(DS1307_MIN));
  i2c_write(decToBcd(DS1307_HR));
  i2c_write(DS1307_DOW);
  i2c_write(decToBcd(DS1307_DATE));
  i2c_write(decToBcd(DS1307_MTH));
  i2c_write(decToBcd(DS1307_YR));
  i2c_endTransmission();
}





File myFile;


void getBMP180()
{
  BIT_SCL = _BV(7);//BMP180
  BIT_SDA = _BV(6);//BMP180
  temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  pressure = bmp085GetPressure(bmp085ReadUP());
  //altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 
}















// A1
#define I_SCK  DDRC |=  _BV(1)
#define H_SCK PORTC |=  _BV(1)
#define L_SCK PORTC &= ~_BV(1)

// A2
#define I_MOSI  DDRC |=  _BV(2)
#define H_MOSI PORTC |=  _BV(2)
#define L_MOSI PORTC &= ~_BV(2)

// A3
#define I_DC  DDRC |=  _BV(3)
#define H_DC PORTC |=  _BV(3)
#define L_DC PORTC &= ~_BV(3)

// 9
#define I_CS  DDRB |=  _BV(1)
#define H_CS PORTB |=  _BV(1)
#define L_CS PORTB &= ~_BV(1)

// 8
#define I_RST  DDRB |=  _BV(0)
#define H_RST PORTB |=  _BV(0)
#define L_RST PORTB &= ~_BV(0)

#define LCD_COMMAND 0
#define LCD_DATA 1

// PCD8544 Commandset
// ------------------
// General commands
#define PCD8544_EXTENDEDINSTRUCTION	0x01
#define PCD8544_DISPLAYNORMAL		0x04
// Normal instruction set
#define PCD8544_FUNCTIONSET			0x20
#define PCD8544_DISPLAYCONTROL		0x08
#define PCD8544_SETYADDR			0x40
#define PCD8544_SETXADDR			0x80
// Extended instruction set
#define PCD8544_SETTEMP				0x04
#define PCD8544_SETBIAS				0x10
#define PCD8544_SETVOP				0x80
// Display presets
#define LCD_BIAS					0x03	// Range: 0-7 (0x00-0x07)
#define LCD_TEMP					0x02	// Range: 0-3 (0x00-0x03)
#define LCD_CONTRAST				0x46	// Range: 0-127 (0x00-0x7F)

PROGMEM prog_uint8_t font[] = {
  //  32
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //! 33
  0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x33,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
  //" 34
  0x00,0x00,0x10,0x00,0x0C,0x00,0x06,0x00,0x10,0x00,0x0C,0x00,0x06,0x00,0x00,0x00,
  //# 35
  0x40,0x04,0xC0,0x3F,0x78,0x04,0x40,0x04,0xC0,0x3F,0x78,0x04,0x40,0x04,0x00,0x00,
  //$ 36
  0x00,0x00,0x70,0x18,0x88,0x20,0xFC,0xFF,0x08,0x21,0x30,0x1E,0x00,0x00,0x00,0x00,
  //% 37
  0xF0,0x00,0x08,0x21,0xF0,0x1C,0x00,0x03,0xE0,0x1E,0x18,0x21,0x00,0x1E,0x00,0x00,
  //& 38
  0x00,0x1E,0xF0,0x21,0x08,0x23,0x88,0x24,0x70,0x19,0x00,0x27,0x00,0x21,0x00,0x10,
  //' 39
  0x10,0x00,0x16,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //( 40
  0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x07,0x18,0x18,0x04,0x20,0x02,0x40,0x00,0x00,
  //) 41
  0x00,0x00,0x02,0x40,0x04,0x20,0x18,0x18,0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,
  //* 42
  0x40,0x02,0x40,0x02,0x80,0x01,0xF0,0x0F,0x80,0x01,0x40,0x02,0x40,0x02,0x00,0x00,
  //+ 43
  0x00,0x01,0x00,0x01,0x00,0x01,0xF0,0x1F,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x00,
  //, 44
  0x00,0x80,0x00,0xB0,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //- 45
  0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
  //. 46
  0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  /// 47
  0x00,0x00,0x00,0x60,0x00,0x18,0x00,0x06,0x80,0x01,0x60,0x00,0x18,0x00,0x04,0x00,
  //0 48
  0x00,0x00,0xE0,0x0F,0x10,0x10,0x08,0x20,0x08,0x20,0x10,0x10,0xE0,0x0F,0x00,0x00,
  //1 49
  0x00,0x00,0x10,0x20,0x10,0x20,0xF8,0x3F,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x00,
  //2 50
  0x00,0x00,0x70,0x30,0x08,0x28,0x08,0x24,0x08,0x22,0x88,0x21,0x70,0x30,0x00,0x00,
  //3 51
  0x00,0x00,0x30,0x18,0x08,0x20,0x88,0x20,0x88,0x20,0x48,0x11,0x30,0x0E,0x00,0x00,
  //4 52
  0x00,0x00,0x00,0x07,0xC0,0x04,0x20,0x24,0x10,0x24,0xF8,0x3F,0x00,0x24,0x00,0x00,
  //5 53
  0x00,0x00,0xF8,0x19,0x08,0x21,0x88,0x20,0x88,0x20,0x08,0x11,0x08,0x0E,0x00,0x00,
  //6 54
  0x00,0x00,0xE0,0x0F,0x10,0x11,0x88,0x20,0x88,0x20,0x18,0x11,0x00,0x0E,0x00,0x00,
  //7 55
  0x00,0x00,0x38,0x00,0x08,0x00,0x08,0x3F,0xC8,0x00,0x38,0x00,0x08,0x00,0x00,0x00,
  //8 56
  0x00,0x00,0x70,0x1C,0x88,0x22,0x08,0x21,0x08,0x21,0x88,0x22,0x70,0x1C,0x00,0x00,
  //9 57
  0x00,0x00,0xE0,0x00,0x10,0x31,0x08,0x22,0x08,0x22,0x10,0x11,0xE0,0x0F,0x00,0x00,
  //: 58
  0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x30,0xC0,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
  //; 59
  0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //< 60
  0x00,0x00,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x08,0x10,0x10,0x08,0x20,0x00,0x00,
  //= 61
  0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x00,0x00,
  //> 62
  0x00,0x00,0x08,0x20,0x10,0x10,0x20,0x08,0x40,0x04,0x80,0x02,0x00,0x01,0x00,0x00,
  //? 63
  0x00,0x00,0x70,0x00,0x48,0x00,0x08,0x30,0x08,0x36,0x08,0x01,0xF0,0x00,0x00,0x00,
  //@ 64
  0xC0,0x07,0x30,0x18,0xC8,0x27,0x28,0x24,0xE8,0x23,0x10,0x14,0xE0,0x0B,0x00,0x00,
  //A 65
  0x00,0x20,0x00,0x3C,0xC0,0x23,0x38,0x02,0xE0,0x02,0x00,0x27,0x00,0x38,0x00,0x20,
  //B 66
  0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x20,0x88,0x20,0x70,0x11,0x00,0x0E,0x00,0x00,
  //C 67
  0xC0,0x07,0x30,0x18,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x10,0x38,0x08,0x00,0x00,
  //D 68
  0x08,0x20,0xF8,0x3F,0x08,0x20,0x08,0x20,0x08,0x20,0x10,0x10,0xE0,0x0F,0x00,0x00,
  //E 69
  0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x20,0xE8,0x23,0x08,0x20,0x10,0x18,0x00,0x00,
  //F 70
  0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x00,0xE8,0x03,0x08,0x00,0x10,0x00,0x00,0x00,
  //G 71
  0xC0,0x07,0x30,0x18,0x08,0x20,0x08,0x20,0x08,0x22,0x38,0x1E,0x00,0x02,0x00,0x00,
  //H 72
  0x08,0x20,0xF8,0x3F,0x08,0x21,0x00,0x01,0x00,0x01,0x08,0x21,0xF8,0x3F,0x08,0x20,
  //I 73
  0x00,0x00,0x08,0x20,0x08,0x20,0xF8,0x3F,0x08,0x20,0x08,0x20,0x00,0x00,0x00,0x00,
  //J 74
  0x00,0xC0,0x00,0x80,0x08,0x80,0x08,0x80,0xF8,0x7F,0x08,0x00,0x08,0x00,0x00,0x00,
  //K 75
  0x08,0x20,0xF8,0x3F,0x88,0x20,0xC0,0x01,0x28,0x26,0x18,0x38,0x08,0x20,0x00,0x00,
  //L 76
  0x08,0x20,0xF8,0x3F,0x08,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x30,0x00,0x00,
  //M 77
  0x08,0x20,0xF8,0x3F,0xF8,0x00,0x00,0x3F,0xF8,0x00,0xF8,0x3F,0x08,0x20,0x00,0x00,
  //N 78
  0x08,0x20,0xF8,0x3F,0x30,0x20,0xC0,0x00,0x00,0x07,0x08,0x18,0xF8,0x3F,0x08,0x00,
  //O 79
  0xE0,0x0F,0x10,0x10,0x08,0x20,0x08,0x20,0x08,0x20,0x10,0x10,0xE0,0x0F,0x00,0x00,
  //P 80
  0x08,0x20,0xF8,0x3F,0x08,0x21,0x08,0x01,0x08,0x01,0x08,0x01,0xF0,0x00,0x00,0x00,
  //Q 81
  0xE0,0x0F,0x10,0x18,0x08,0x24,0x08,0x24,0x08,0x38,0x10,0x50,0xE0,0x4F,0x00,0x00,
  //R 82
  0x08,0x20,0xF8,0x3F,0x88,0x20,0x88,0x00,0x88,0x03,0x88,0x0C,0x70,0x30,0x00,0x20,
  //S 83
  0x08,0x20,0x18,0x30,0x68,0x2C,0x80,0x03,0x80,0x03,0x68,0x2C,0x18,0x30,0x08,0x20,
  //T 84
  0x18,0x00,0x08,0x00,0x08,0x20,0xF8,0x3F,0x08,0x20,0x08,0x00,0x18,0x00,0x00,0x00,
  //U 85
  0x08,0x00,0xF8,0x1F,0x08,0x20,0x00,0x20,0x00,0x20,0x08,0x20,0xF8,0x1F,0x08,0x00,
  //V 86
  0x08,0x00,0x78,0x00,0x88,0x07,0x00,0x38,0x00,0x0E,0xC8,0x01,0x38,0x00,0x08,0x00,
  //W 87
  0xF8,0x03,0x08,0x3C,0x00,0x07,0xF8,0x00,0x00,0x07,0x08,0x3C,0xF8,0x03,0x00,0x00,
  //X 88
  0x08,0x20,0x18,0x30,0x68,0x2C,0x80,0x03,0x80,0x03,0x68,0x2C,0x18,0x30,0x08,0x20,
  //Y 89
  0x08,0x00,0x38,0x00,0xC8,0x20,0x00,0x3F,0xC8,0x20,0x38,0x00,0x08,0x00,0x00,0x00,
  //Z 90
  0x10,0x20,0x08,0x38,0x08,0x26,0x08,0x21,0xC8,0x20,0x38,0x20,0x08,0x18,0x00,0x00,
  //[ 91
  0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x7F,0x02,0x40,0x02,0x40,0x02,0x40,0x00,0x00,
  //\ 92
  0x00,0x00,0x0C,0x00,0x30,0x00,0xC0,0x01,0x00,0x06,0x00,0x38,0x00,0xC0,0x00,0x00,
  //] 93
  0x00,0x00,0x02,0x40,0x02,0x40,0x02,0x40,0xFE,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,
  //^ 94
  0x00,0x00,0x00,0x00,0x04,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x00,0x00,
  //_ 95
  0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
  //` 96
  0x00,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //a 97
  0x00,0x00,0x00,0x19,0x80,0x24,0x80,0x22,0x80,0x22,0x80,0x22,0x00,0x3F,0x00,0x20,
  //b 98
  0x08,0x00,0xF8,0x3F,0x00,0x11,0x80,0x20,0x80,0x20,0x00,0x11,0x00,0x0E,0x00,0x00,
  //c
  0x00,0x00,0x00,0x0E,0x00,0x11,0x80,0x20,0x80,0x20,0x80,0x20,0x00,0x11,0x00,0x00,
  //d
  0x00,0x00,0x00,0x0E,0x00,0x11,0x80,0x20,0x80,0x20,0x88,0x10,0xF8,0x3F,0x00,0x20,
  //e
  0x00,0x00,0x00,0x1F,0x80,0x22,0x80,0x22,0x80,0x22,0x80,0x22,0x00,0x13,0x00,0x00,
  //f
  0x00,0x00,0x80,0x20,0x80,0x20,0xF0,0x3F,0x88,0x20,0x88,0x20,0x88,0x00,0x18,0x00,
  //g
  0x00,0x00,0x00,0x6B,0x80,0x94,0x80,0x94,0x80,0x94,0x80,0x93,0x80,0x60,0x00,0x00,
  //h
  0x08,0x20,0xF8,0x3F,0x00,0x21,0x80,0x00,0x80,0x00,0x80,0x20,0x00,0x3F,0x00,0x20,
  //i
  0x00,0x00,0x80,0x20,0x98,0x20,0x98,0x3F,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x00,
  //j
  0x00,0x00,0x00,0xC0,0x00,0x80,0x80,0x80,0x98,0x80,0x98,0x7F,0x00,0x00,0x00,0x00,
  //k
  0x08,0x20,0xF8,0x3F,0x00,0x24,0x00,0x02,0x80,0x2D,0x80,0x30,0x80,0x20,0x00,0x00,
  //l
  0x00,0x00,0x08,0x20,0x08,0x20,0xF8,0x3F,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x00,
  //m
  0x80,0x20,0x80,0x3F,0x80,0x20,0x80,0x00,0x80,0x3F,0x80,0x20,0x80,0x00,0x00,0x3F,
  //n
  0x80,0x20,0x80,0x3F,0x00,0x21,0x80,0x00,0x80,0x00,0x80,0x20,0x00,0x3F,0x00,0x20,
  //o
  0x00,0x00,0x00,0x1F,0x80,0x20,0x80,0x20,0x80,0x20,0x80,0x20,0x00,0x1F,0x00,0x00,
  //p
  0x80,0x80,0x80,0xFF,0x00,0xA1,0x80,0x20,0x80,0x20,0x00,0x11,0x00,0x0E,0x00,0x00,
  //q
  0x00,0x00,0x00,0x0E,0x00,0x11,0x80,0x20,0x80,0x20,0x80,0xA0,0x80,0xFF,0x00,0x80,
  //r
  0x80,0x20,0x80,0x20,0x80,0x3F,0x00,0x21,0x80,0x20,0x80,0x00,0x80,0x01,0x00,0x00,
  //s
  0x00,0x00,0x00,0x33,0x80,0x24,0x80,0x24,0x80,0x24,0x80,0x24,0x80,0x19,0x00,0x00,
  //t
  0x00,0x00,0x80,0x00,0x80,0x00,0xE0,0x1F,0x80,0x20,0x80,0x20,0x00,0x00,0x00,0x00,
  //u
  0x80,0x00,0x80,0x1F,0x00,0x20,0x00,0x20,0x00,0x20,0x80,0x10,0x80,0x3F,0x00,0x20,
  //v
  0x80,0x00,0x80,0x01,0x80,0x0E,0x00,0x30,0x00,0x08,0x80,0x06,0x80,0x01,0x80,0x00,
  //w
  0x80,0x0F,0x80,0x30,0x00,0x0C,0x80,0x03,0x00,0x0C,0x80,0x30,0x80,0x0F,0x80,0x00,
  //x
  0x00,0x00,0x80,0x20,0x80,0x31,0x00,0x2E,0x80,0x0E,0x80,0x31,0x80,0x20,0x00,0x00,
  //y
  0x80,0x80,0x80,0x81,0x80,0x8E,0x00,0x70,0x00,0x18,0x80,0x06,0x80,0x01,0x80,0x00,
  //z
  0x00,0x00,0x80,0x21,0x80,0x30,0x80,0x2C,0x80,0x22,0x80,0x21,0x80,0x30,0x00,0x00,
  //{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x7C,0x3F,0x02,0x40,0x02,0x40,
  //|
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
  //}
  0x00,0x00,0x02,0x40,0x02,0x40,0x7C,0x3F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //~
  0x06,0x00,0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x04,0x00,0x03,0x00,
  //
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


void _LCD_Write(unsigned char data, unsigned char mode)
{   
  L_CS;
  if (mode==LCD_COMMAND)
    L_DC;
  else
    H_DC;

  for (unsigned char c=0; c<8; c++)
  {
    if (data & 0x80)
      H_MOSI;
    else
      L_MOSI;
    data = data<<1;
    L_SCK;
    asm ("nop");
    H_SCK;
  }
  H_CS;
}

void _InitLCD()
{
  I_SCK;
  I_MOSI;
  I_DC;
  I_RST;
  I_CS;
  //resetLCD;
  H_DC;
  H_MOSI;
  H_SCK;
  H_CS;
  L_RST;
  delay(10);
  H_RST;

  _LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
  _LCD_Write(PCD8544_SETVOP | LCD_CONTRAST, LCD_COMMAND);
  _LCD_Write(PCD8544_SETTEMP | LCD_TEMP, LCD_COMMAND);
  _LCD_Write(PCD8544_SETBIAS | LCD_BIAS, LCD_COMMAND);
  _LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
  _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
  _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
  for (int c=0; c<504; c++)
    _LCD_Write(0x00, LCD_DATA);
  _LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);

  //_contrast=LCD_CONTRAST;
}

void setContrast(int contrast)
{
  if (contrast>0x7F)
    contrast=0x7F;
  if (contrast<0)
    contrast=0;
  _LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
  _LCD_Write(PCD8544_SETVOP | contrast, LCD_COMMAND);
  _LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
  //_contrast=contrast;
}

//Pos:0,1,2
void drawLine(char* line, uint8_t Pos)
{
  {
    _LCD_Write(PCD8544_SETYADDR | Pos*2, LCD_COMMAND);
    _LCD_Write(PCD8544_SETXADDR | 0, LCD_COMMAND);
    for(uint8_t i=0;i<10;i++)
    {
      int chr = line[i];
      if(chr<32)
      {
        chr = 0;
      }
      else
      {
        chr -= 32;
      }
      chr*=16;
      for(uint8_t cx=0; cx<16; cx+=2)
      {
        _LCD_Write(~pgm_read_byte_near(font+chr+cx), LCD_DATA);
      }
    }
  }
  {
    _LCD_Write(PCD8544_SETYADDR | (Pos*2+1), LCD_COMMAND);
    _LCD_Write(PCD8544_SETXADDR | 0, LCD_COMMAND);
    for(uint8_t i=0;i<10;i++)
    {
      int chr = line[i];
      if(chr<32)
      {
        chr = 0;
      }
      else
      {
        chr -= 32;
      }
      chr*=16;
      for(uint8_t cx=1; cx<17; cx+=2)
      {
        _LCD_Write(~pgm_read_byte_near(font+chr+cx), LCD_DATA);
      }
    }
  }

  _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
  _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
}








void setup(){
  Serial.begin(115200);
  DDRD |= _BV(3);
  DDRB |= _BV(5);
  BIT_SCL = _BV(7);//BMP180
  BIT_SDA = _BV(6);//BMP180
  i2c_SoftI2CMaster();

  BIT_SCL = _BV(5);//DS1307
  BIT_SDA = _BV(4);//DS1307
  i2c_SoftI2CMaster();

  DS1307_read();
  _InitLCD();

  while(true)
  {
    if(DS1307_SEC == 80)Serial.println("Time error:");
    int cmd = Serial.parseInt();
    if(cmd == 29)
    {
      //29,15,7,17,20,39,00,5,
      DS1307_YR  = Serial.parseInt();
      DS1307_MTH = Serial.parseInt();
      DS1307_DATE = Serial.parseInt();
      DS1307_HR  = Serial.parseInt();
      DS1307_MIN = Serial.parseInt();
      DS1307_SEC = Serial.parseInt();
      DS1307_DOW= Serial.parseInt();

      DS1307_save();

      DS1307_read();
      char buf[30];
      //                          2015-01-01 12:00:00
      snprintf(buf, sizeof(buf), "20%02d-%02d-%02d %02d:%02d:%02d",DS1307_YR, DS1307_MTH, DS1307_DATE, DS1307_HR, DS1307_MIN, DS1307_SEC);
      Serial.println(buf);
      break;
    }
    if(digitalRead(2)==LOW)
    {
      if(DS1307_SEC == 80)
      {
        //29,15,7,17,20,39,00,5,
        DS1307_YR  = 15;
        DS1307_MTH = 1;
        DS1307_DATE = 1;
        DS1307_HR  = 0;
        DS1307_MIN = 0;
        DS1307_SEC = 0;
        DS1307_DOW= 0;
        DS1307_save();
      }
      break;
    }
  }

  PORTD |= _BV(3);
  while (!SD.begin()) {
    Serial.println("SD not ready.");
    delay(1000);
  }
  PORTD &= ~_BV(3);
}

int a_t[50];
int LastTotalT = 999;
int TotalT = 999;
long a_p[50];
long LastTotalP=100000;
long TotalP=100000;

int idx = 0;
char line[11];
char buf[12];
char buf2[10];

void loop()
{
  uint32_t t0 = millis();
  PORTB |= _BV(5);

  if(idx==1)
  {
    for(uint8_t i = 0;i<10;i++)
    {
      line[i] = ' ';
    }
    line[0] = buf[4];
    line[1] = buf[5];
    line[2] = '-';
    line[3] = buf[6];
    line[4] = buf[7];
    line[5] = ' ';
    LastTotalT = LastTotalT/5;
    line[9] = '0'+(LastTotalT%10);    LastTotalT=LastTotalT/10;
    line[8] = '0'+(LastTotalT%10);    LastTotalT=LastTotalT/10;
    line[7] = '0'+(LastTotalT%10);    LastTotalT=LastTotalT/10;
    line[6] = '0'+(LastTotalT%10);    LastTotalT=LastTotalT/10;
    line[10] = '\0';
    drawLine(line,0);
  }
  if(idx==2)
  {
    for(uint8_t i = 0;i<10;i++)
    {
      line[i] = ' ';
    }
    LastTotalP = LastTotalP/5;
    line[9] = '0'+(LastTotalP%10);    LastTotalP=LastTotalP/10;
    line[8] = '0'+(LastTotalP%10);    LastTotalP=LastTotalP/10;
    line[7] = '0'+(LastTotalP%10);    LastTotalP=LastTotalP/10;
    line[6] = '0'+(LastTotalP%10);    LastTotalP=LastTotalP/10;
    line[5] = '0'+(LastTotalP%10);    LastTotalP=LastTotalP/10;
    line[4] = '0'+(LastTotalP%10);    LastTotalP=LastTotalP/10;
    line[3] = '0'+(LastTotalP%10);    LastTotalP=LastTotalP/10;
    line[2] = '0'+(LastTotalP%10);    LastTotalP=LastTotalP/10;
    line[1] = '0'+(LastTotalP%10);    LastTotalP=LastTotalP/10;
    line[0] = '0'+(LastTotalP%10);    LastTotalP=LastTotalP/10;
    line[10] = '\0';
    drawLine(line,1);
  }
  if(idx==3)
  {
    drawLine(buf2,2);
  }
  if(idx==50)
  {
    idx=0;
    DS1307_read();
    buf[0] = '2';
    buf[1] = '0';
    buf[2] = '0'+(DS1307_YR/10);
    buf[3] = '0'+(DS1307_YR%10);
    buf[4] = '0'+(DS1307_MTH/10);
    buf[5] = '0'+(DS1307_MTH%10);
    buf[6] = '0'+(DS1307_DATE/10);
    buf[7] = '0'+(DS1307_DATE%10);
    buf[8] = 'T';
    buf[9] = 'X';
    buf[10] = 'T';
    buf[11] = '\0';

    buf2[0] = '0'+(DS1307_HR/10);
    buf2[1] = '0'+(DS1307_HR%10);
    buf2[2] = ':';
    buf2[3] = '0'+(DS1307_MIN/10);
    buf2[4] = '0'+(DS1307_MIN%10);
    buf2[5] = ':';
    buf2[6] = '0'+(DS1307_SEC/10);
    buf2[7] = '0'+(DS1307_SEC%10);
    buf2[8] = '\0';

    //snprintf(buf, 12, "20%02d%02d%02dTXT",DS1307_YR, DS1307_MTH, DS1307_DATE);
    Serial.print(buf);
    //snprintf(buf2, 10, "%02d:%02d:%02d",DS1307_HR, DS1307_MIN, DS1307_SEC);
    Serial.println(buf2);

    PORTD |= _BV(3);
    if (SD.openSimple(buf, 1)) {
      SD.file.println(buf2);
      SD.file.print(a_t[0]);
      SD.file.print(",");
      SD.file.println(a_p[0]);
      for(int i=1;i<50;i++)
      {
        SD.file.print(a_t[i]-a_t[0]);
        SD.file.print(",");
        SD.file.println(a_p[i]-a_p[0]);
      }
      //myFile.print(",");
      //myFile.print(altitude);
      SD.file.println();
      SD.file.close();
      PORTD &= ~_BV(3);
    }
    else {
      Serial.println("error.");
    }
  }
  if(idx==0)
  {
    BIT_SCL = _BV(7);//BMP180
    BIT_SDA = _BV(6);//BMP180
    bmp085Calibration();
    LastTotalT = TotalT;
    TotalT = 0;
    LastTotalP = TotalP;
    TotalP = 0;
  }

  getBMP180();
  int T_a = temperature;
  long P_a = pressure;
  PORTB &= ~_BV(5);
  while(millis() - t0<100);
  PORTB |= _BV(5);

  getBMP180();
  a_t[idx] = temperature/2+T_a/2;
  TotalT += a_t[idx];
  a_p[idx] = pressure/2+P_a/2;
  TotalP += a_p[idx];

  t0 = millis();
  Serial.print(a_t[idx]);
  Serial.print(",");
  Serial.println(a_p[idx]);
  PORTB &= ~_BV(5);
  while(millis() - t0<100);
  idx++;

}













