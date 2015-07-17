
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdint.h>


#include <avr/io.h>

uint8_t errCode = 0;

void _dly()
{
  for(int i=0;i<10;i++)
  {
    volatile int t;
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
// SD card errors
/** timeout error for command CMD0 */
uint8_t const SD_CARD_ERROR_CMD0 = 0X1;
/** CMD8 was not accepted - not a valid SD card*/
uint8_t const SD_CARD_ERROR_CMD8 = 0X2;
/** card returned an error response for CMD17 (read block) */
uint8_t const SD_CARD_ERROR_CMD17 = 0X3;
/** card returned an error response for CMD24 (write block) */
uint8_t const SD_CARD_ERROR_CMD24 = 0X4;
/**  WRITE_MULTIPLE_BLOCKS command failed */
uint8_t const SD_CARD_ERROR_CMD25 = 0X05;
/** card returned an error response for CMD58 (read OCR) */
uint8_t const SD_CARD_ERROR_CMD58 = 0X06;
/** SET_WR_BLK_ERASE_COUNT failed */
uint8_t const SD_CARD_ERROR_ACMD23 = 0X07;
/** card's ACMD41 initialization process timeout */
uint8_t const SD_CARD_ERROR_ACMD41 = 0X08;
/** card returned a bad CSR version field */
uint8_t const SD_CARD_ERROR_BAD_CSD = 0X09;
/** card returned an error token instead of read data */
uint8_t const SD_CARD_ERROR_READ = 0X0D;
/** read CID or CSD failed */
uint8_t const SD_CARD_ERROR_READ_REG = 0X0E;
/** timeout while waiting for start of read data */
uint8_t const SD_CARD_ERROR_READ_TIMEOUT = 0X0F;
/** card did not accept STOP_TRAN_TOKEN */
uint8_t const SD_CARD_ERROR_STOP_TRAN = 0X10;
/** card returned an error token as a response to a write operation */
uint8_t const SD_CARD_ERROR_WRITE = 0X11;
/** attempt to write protected block zero */
uint8_t const SD_CARD_ERROR_WRITE_BLOCK_ZERO = 0X12;
/** card did not go ready for a multiple block write */
uint8_t const SD_CARD_ERROR_WRITE_MULTIPLE = 0X13;
/** card returned an error to a CMD13 status check after a write */
uint8_t const SD_CARD_ERROR_WRITE_PROGRAMMING = 0X14;
/** timeout occurred during write programming */
uint8_t const SD_CARD_ERROR_WRITE_TIMEOUT = 0X15;
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
  Sd2Card(void) : errorCode_(0), inBlock_(0), partialBlockRead_(0), type_(0) {}
  /**
   * \return error code for last error. See Sd2Card.h for a list of error codes.
   */
  uint8_t errorCode(void) const {return errorCode_;}
  /** \return error data for last error. */
  uint8_t errorData(void) const {return status_;}

  /**
   * Initialize an SD flash memory card with the selected SPI clock rate
   * and the default SD chip select pin.
   */
  uint8_t init();
  void partialBlockRead(uint8_t value);
  /** Returns the current value, true or false, for partial block read. */
  uint8_t partialBlockRead(void) const {return partialBlockRead_;}
  uint8_t readBlock(uint32_t block, uint8_t* dst);
  uint8_t readData(uint32_t block,
          uint16_t offset, uint16_t count, uint8_t* dst);
  void readEnd(void);
  /** Return the card type: SD V1, SD V2 or SDHC */
  uint8_t type(void) const {return type_;}
  uint8_t writeBlock(uint32_t blockNumber, const uint8_t* src);
  uint8_t writeData(const uint8_t* src);
  uint8_t writeStart(uint32_t blockNumber, uint32_t eraseCount);
  uint8_t writeStop(void);
 private:
  uint32_t block_;
  uint8_t errorCode_;
  uint8_t inBlock_;
  uint16_t offset_;
  uint8_t partialBlockRead_;
  uint8_t status_;
  uint8_t type_;
  // private functions
  uint8_t cardAcmd(uint8_t cmd, uint32_t arg) {
    cardCommand(CMD55, 0);
    return cardCommand(cmd, arg);
  }
  uint8_t cardCommand(uint8_t cmd, uint32_t arg);
  void error(uint8_t code) {errorCode_ = code;}
  uint8_t readRegister(uint8_t cmd, void* buf);
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
 * Date Format. A FAT directory entry date stamp is a 16-bit field that is 
 * basically a date relative to the MS-DOS epoch of 01/01/1980. Here is the
 * format (bit 0 is the LSB of the 16-bit word, bit 15 is the MSB of the 
 * 16-bit word):
 *   
 * Bits 9-15: Count of years from 1980, valid value range 0-127 
 * inclusive (1980-2107).
 *   
 * Bits 5-8: Month of year, 1 = January, valid value range 1-12 inclusive.
 *
 * Bits 0-4: Day of month, valid value range 1-31 inclusive.
 *
 * Time Format. A FAT directory entry time stamp is a 16-bit field that has
 * a granularity of 2 seconds. Here is the format (bit 0 is the LSB of the 
 * 16-bit word, bit 15 is the MSB of the 16-bit word).
 *   
 * Bits 11-15: Hours, valid value range 0-23 inclusive.
 * 
 * Bits 5-10: Minutes, valid value range 0-59 inclusive.
 *      
 * Bits 0-4: 2-second count, valid value range 0-29 inclusive (0 - 58 seconds).
 *   
 * The valid time range is from Midnight 00:00:00 to 23:59:58.
 */
struct directoryEntry {
           /**
            * Short 8.3 name.
            * The first eight bytes contain the file name with blank fill.
            * The last three bytes contain the file extension with blank fill.
            */
  uint8_t  name[11];
          /** Entry attributes.
           *
           * The upper two bits of the attribute byte are reserved and should
           * always be set to 0 when a file is created and never modified or
           * looked at after that.  See defines that begin with DIR_ATT_.
           */
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
           /** Time file was created. */
  uint16_t creationTime;
           /** Date file was created. */
  uint16_t creationDate;
          /**
           * Last access date. Note that there is no last access time, only
           * a date.  This is the date of last read or write. In the case of
           * a write, this should be set to the same date as lastWriteDate.
           */
  uint16_t lastAccessDate;
          /**
           * High word of this entry's first cluster number (always 0 for a
           * FAT12 or FAT16 volume).
           */
  uint16_t firstClusterHigh;
           /** Time of last write. File creation is considered a write. */
  uint16_t lastWriteTime;
           /** Date of last write. File creation is considered a write. */
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
/** escape for name[0] = 0XE5 */
uint8_t const DIR_NAME_0XE5 = 0X05;
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
/** Test value for long name entry.  Test is
  (d->attributes & DIR_ATT_LONG_NAME_MASK) == DIR_ATT_LONG_NAME. */
uint8_t const DIR_ATT_LONG_NAME = 0X0F;
/** Test mask for long name entry */
uint8_t const DIR_ATT_LONG_NAME_MASK = 0X3F;
/** defined attribute bits */
uint8_t const DIR_ATT_DEFINED_BITS = 0X3F;
/** Directory entry is part of a long name */
static inline uint8_t DIR_IS_LONG_NAME(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_LONG_NAME_MASK) == DIR_ATT_LONG_NAME;
}
/** Mask for file/subdirectory tests */
uint8_t const DIR_ATT_FILE_TYPE_MASK = (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY);
/** Directory entry is for a file */
static inline uint8_t DIR_IS_FILE(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == 0;
}
/** Directory entry is for a subdirectory */
static inline uint8_t DIR_IS_SUBDIR(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == DIR_ATT_DIRECTORY;
}
/** Directory entry is for a file or subdirectory */
static inline uint8_t DIR_IS_FILE_OR_SUBDIR(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_VOLUME_ID) == 0;
}

//------------------------------------------------------------------------------
/**
 * Allow use of deprecated functions if non-zero
 */
#define ALLOW_DEPRECATED_FUNCTIONS 1
//------------------------------------------------------------------------------
// forward declaration since SdVolume is used in SdFile
class SdVolume;
//==============================================================================
// SdFile class

// flags for ls()

// use the gnu style oflag in open()
/** open() oflag for reading */
uint8_t const O_READ = 0X01;
/** open() oflag - same as O_READ */
uint8_t const O_RDONLY = O_READ;
/** open() oflag for write */
uint8_t const O_WRITE = 0X02;
/** open() oflag - same as O_WRITE */
uint8_t const O_WRONLY = O_WRITE;
/** open() oflag for reading and writing */
uint8_t const O_RDWR = (O_READ | O_WRITE);
/** open() oflag mask for access modes */
uint8_t const O_ACCMODE = (O_READ | O_WRITE);
/** The file offset shall be set to the end of the file prior to each write. */
uint8_t const O_APPEND = 0X04;
/** synchronous writes - call sync() after each write */
uint8_t const O_SYNC = 0X08;
/** create the file if nonexistent */
uint8_t const O_CREAT = 0X10;
/** If O_CREAT and O_EXCL are set, open() shall fail if the file exists */
uint8_t const O_EXCL = 0X20;
/** truncate the file to zero length */
uint8_t const O_TRUNC = 0X40;

// flags for timestamp
/** set the file's last access date */
uint8_t const T_ACCESS = 1;
/** set the file's creation date and time */
uint8_t const T_CREATE = 2;
/** Set the file's write date and time */
uint8_t const T_WRITE = 4;
// values for type_
/** This SdFile has not been opened. */
uint8_t const FAT_FILE_TYPE_CLOSED = 0;
/** SdFile for a file */
uint8_t const FAT_FILE_TYPE_NORMAL = 1;
/** SdFile for a FAT16 root directory */
uint8_t const FAT_FILE_TYPE_ROOT16 = 2;
/** SdFile for a FAT32 root directory */
uint8_t const FAT_FILE_TYPE_ROOT32 = 3;
/** SdFile for a subdirectory */
uint8_t const FAT_FILE_TYPE_SUBDIR = 4;
/** Test value for directory type */
uint8_t const FAT_FILE_TYPE_MIN_DIR = FAT_FILE_TYPE_ROOT16;

/** date field for FAT directory entry */
static inline uint16_t FAT_DATE(uint16_t year, uint8_t month, uint8_t day) {
  return (year - 1980) << 9 | month << 5 | day;
}
/** year part of FAT directory date field */
static inline uint16_t FAT_YEAR(uint16_t fatDate) {
  return 1980 + (fatDate >> 9);
}
/** month part of FAT directory date field */
static inline uint8_t FAT_MONTH(uint16_t fatDate) {
  return (fatDate >> 5) & 0XF;
}
/** day part of FAT directory date field */
static inline uint8_t FAT_DAY(uint16_t fatDate) {
  return fatDate & 0X1F;
}
/** time field for FAT directory entry */
static inline uint16_t FAT_TIME(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour << 11 | minute << 5 | second >> 1;
}
/** hour part of FAT directory time field */
static inline uint8_t FAT_HOUR(uint16_t fatTime) {
  return fatTime >> 11;
}
/** minute part of FAT directory time field */
static inline uint8_t FAT_MINUTE(uint16_t fatTime) {
  return(fatTime >> 5) & 0X3F;
}
/** second part of FAT directory time field */
static inline uint8_t FAT_SECOND(uint16_t fatTime) {
  return 2*(fatTime & 0X1F);
}
/** Default date for file timestamps is 1 Jan 2000 */
uint16_t const FAT_DEFAULT_DATE = ((2000 - 1980) << 9) | (1 << 5) | 1;
/** Default time for file timestamp is 1 am */
uint16_t const FAT_DEFAULT_TIME = (1 << 11);
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
  /**
   * Cancel unbuffered reads for this file.
   * See setUnbufferedRead()
   */
  void clearUnbufferedRead(void) {
    flags_ &= ~F_FILE_UNBUFFERED_READ;
  }
  uint8_t close(void);
  /** \return The current cluster number for a file or directory. */
  uint32_t curCluster(void) const {return curCluster_;}
  /** \return The current position for a file or directory. */
  uint32_t curPosition(void) const {return curPosition_;}
  /** \return Address of the block that contains this file's directory. */
  uint32_t dirBlock(void) const {return dirBlock_;}
  uint8_t dirEntry(dir_t* dir);
  /** \return Index of this file's directory in the block dirBlock. */
  uint8_t dirIndex(void) const {return dirIndex_;}
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
  uint8_t open(SdFile* dirFile, uint16_t index, uint8_t oflag);
  uint8_t open(SdFile* dirFile, const char* fileName, uint8_t oflag);

  uint8_t openRoot(SdVolume* vol);
  static void printFatDate(uint16_t fatDate);
  static void printFatTime(uint16_t fatTime);
  static void printTwoDigits(uint8_t v);
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
  int8_t readDir(dir_t* dir);
  static uint8_t remove(SdFile* dirFile, const char* fileName);
  uint8_t remove(void);
  /** Set the file's current position to zero. */
  void rewind(void) {
    curPosition_ = curCluster_ = 0;
  }
  uint8_t rmDir(void);
  /** Set the files position to current position + \a pos. See seekSet(). */
  uint8_t seekCur(uint32_t pos) {
    return seekSet(curPosition_ + pos);
  }
  /**
   *  Set the files current position to end of file.  Useful to position
   *  a file for append. See seekSet().
   */
  uint8_t seekEnd(void) {return seekSet(fileSize_);}
  uint8_t seekSet(uint32_t pos);
  /**
   * Use unbuffered reads to access this file.  Used with Wave
   * Shield ISR.  Used with Sd2Card::partialBlockRead() in WaveRP.
   *
   * Not recommended for normal applications.
   */
  void setUnbufferedRead(void) {
    if (isFile()) flags_ |= F_FILE_UNBUFFERED_READ;
  }
  uint8_t sync(void);
  /** Type of this SdFile.  You should use isFile() or isDir() instead of type()
   * if possible.
   *
   * \return The file or directory type.
   */
  uint8_t type(void) const {return type_;}
  uint8_t truncate(uint32_t size);
  /** \return Unbuffered read flag. */
  uint8_t unbufferedRead(void) const {
    return flags_ & F_FILE_UNBUFFERED_READ;
  }
  /** \return SdVolume that contains this file. */
  SdVolume* volume(void) const {return vol_;}
  size_t write(uint8_t b);
  size_t write(const void* buf, uint16_t nbyte);
  size_t write(const char* str);
  void write_P(PGM_P str);
  void writeln_P(PGM_P str);
//------------------------------------------------------------------------------
#if ALLOW_DEPRECATED_FUNCTIONS

  /** \deprecated Use: uint8_t SdFile::dirEntry(dir_t* dir); */
  uint8_t dirEntry(dir_t& dir) {return dirEntry(&dir);}  // NOLINT

  /** \deprecated Use:
   * uint8_t SdFile::open(SdFile* dirFile, const char* fileName, uint8_t oflag);
   */
  uint8_t open(SdFile& dirFile, // NOLINT
    const char* fileName, uint8_t oflag) {
    return open(&dirFile, fileName, oflag);
  }
  /** \deprecated  Do not use in new apps */
  uint8_t open(SdFile& dirFile, const char* fileName) {  // NOLINT
    return open(dirFile, fileName, O_RDWR);
  }
  /** \deprecated Use:
   * uint8_t SdFile::open(SdFile* dirFile, uint16_t index, uint8_t oflag);
   */
  uint8_t open(SdFile& dirFile, uint16_t index, uint8_t oflag) {  // NOLINT
    return open(&dirFile, index, oflag);
  }
  /** \deprecated Use: uint8_t SdFile::openRoot(SdVolume* vol); */
  uint8_t openRoot(SdVolume& vol) {return openRoot(&vol);}  // NOLINT

  /** \deprecated Use: int8_t SdFile::readDir(dir_t* dir); */
  int8_t readDir(dir_t& dir) {return readDir(&dir);}  // NOLINT
  /** \deprecated Use:
   * static uint8_t SdFile::remove(SdFile* dirFile, const char* fileName);
   */
  static uint8_t remove(SdFile& dirFile, const char* fileName) {  // NOLINT
    return remove(&dirFile, fileName);
  }
//------------------------------------------------------------------------------
// rest are private
 private:
  static void (*oldDateTime_)(uint16_t& date, uint16_t& time);  // NOLINT
#endif  // ALLOW_DEPRECATED_FUNCTIONS
 private:
  // bits defined in flags_
  // should be 0XF
  static uint8_t const F_OFLAG = (O_ACCMODE | O_APPEND | O_SYNC);
  // available bits
  static uint8_t const F_UNUSED = 0X30;
  // use unbuffered SD read
  static uint8_t const F_FILE_UNBUFFERED_READ = 0X40;
  // sync of directory entry required
  static uint8_t const F_FILE_DIR_DIRTY = 0X80;

// make sure F_OFLAG is ok
#if ((F_UNUSED | F_FILE_UNBUFFERED_READ | F_FILE_DIR_DIRTY) & F_OFLAG)
#error flags_ bits conflict
#endif  // flags_ bits

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

  // private functions
  uint8_t addCluster(void);
  uint8_t addDirCluster(void);
  dir_t* cacheDirEntry(uint8_t action);
  static void (*dateTime_)(uint16_t* date, uint16_t* time);
  static uint8_t make83Name(const char* str, uint8_t* name);
  uint8_t openCachedEntry(uint8_t cacheIndex, uint8_t oflags);
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
  uint8_t init(Sd2Card* dev) { return init(dev, 1) ? true : init(dev, 0);}
  uint8_t init(Sd2Card* dev, uint8_t part);

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
#if ALLOW_DEPRECATED_FUNCTIONS
  // Deprecated functions  - suppress cpplint warnings with NOLINT comment
  /** \deprecated Use: uint8_t SdVolume::init(Sd2Card* dev); */
  uint8_t init(Sd2Card& dev) {return init(&dev);}  // NOLINT

  /** \deprecated Use: uint8_t SdVolume::init(Sd2Card* dev, uint8_t vol); */
  uint8_t init(Sd2Card& dev, uint8_t part) {  // NOLINT
    return init(&dev, part);
  }
#endif  // ALLOW_DEPRECATED_FUNCTIONS
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
  uint8_t allocContiguous(uint32_t count, uint32_t* curCluster);
  uint8_t blockOfCluster(uint32_t position) const {
          return (position >> 9) & (blocksPerCluster_ - 1);}
  uint32_t clusterStartBlock(uint32_t cluster) const {
           return dataStartBlock_ + ((cluster - 2) << clusterSizeShift_);}
  uint32_t blockNumber(uint32_t cluster, uint32_t position) const {
           return clusterStartBlock(cluster) + blockOfCluster(position);}
  static uint8_t cacheFlush(void);
  static uint8_t cacheRawBlock(uint32_t blockNumber, uint8_t action);
  static void cacheSetDirty(void) {cacheDirty_ |= CACHE_FOR_WRITE;}
  static uint8_t cacheZeroBlock(uint32_t blockNumber);
  uint8_t chainSize(uint32_t beginCluster, uint32_t* size) const;
  uint8_t fatGet(uint32_t cluster, uint32_t* value) const;
  uint8_t fatPut(uint32_t cluster, uint32_t value);
  uint8_t fatPutEOC(uint32_t cluster) {
    return fatPut(cluster, 0x0FFFFFFF);
  }
  uint8_t freeChain(uint32_t cluster);
  uint8_t isEOC(uint32_t cluster) const {
    return  cluster >= (fatType_ == 16 ? FAT16EOC_MIN : FAT32EOC_MIN);
  }
  uint8_t readBlock(uint32_t block, uint8_t* dst) {
    return sdCard_->readBlock(block, dst);}
  uint8_t readData(uint32_t block, uint16_t offset,
    uint16_t count, uint8_t* dst) {
      return sdCard_->readData(block, offset, count, dst);
  }
  uint8_t writeBlock(uint32_t block, const uint8_t* dst) {
    return sdCard_->writeBlock(block, dst);
  }
};


class File : public Stream {
 private:
  char _name[13]; // our name
  SdFile *_file;  // underlying file pointer

public:
  File(SdFile f, const char *name);     // wraps an underlying SdFile
  File(void);      // 'empty' constructor
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
  operator bool();
  char * name();
  
  using Print::write;
};

class SDClass {

private:
  // These are required for initialisation and use of sdfatlib
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  
public:
  // This needs to be called to set up the connection to the SD card
  // before other methods are used.
  boolean begin();
  
  // Open the specified file/directory with the supplied mode (e.g. read or
  // write, etc). Returns a File object for interacting with the file.
  // Note that currently only one file can be open at a time.
  File openSimple(const char *filename, uint8_t mode, uint8_t toend);

private:
  
  friend class File;
};

extern SDClass SD;





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
/**
 * Initialize an SD flash memory card.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.  The reason for failure
 * can be determined by calling errorCode() and errorData().
 */
uint8_t Sd2Card::init() {
  errorCode_ = inBlock_ = partialBlockRead_ = type_ = 0;
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
 * Enable or disable partial block reads.
 *
 * Enabling partial block reads improves performance by allowing a block
 * to be read over the SPI bus as several sub-blocks.  Errors may occur
 * if the time between reads is too long since the SD card may timeout.
 * The SPI SS line will be held low until the entire block is read or
 * readEnd() is called.
 *
 * Use this for applications like the Adafruit Wave Shield.
 *
 * \param[in] value The value TRUE (non-zero) or FALSE (zero).)
 */
void Sd2Card::partialBlockRead(uint8_t value) {
  readEnd();
  partialBlockRead_ = value;
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
uint8_t Sd2Card::readData(uint32_t block,
        uint16_t offset, uint16_t count, uint8_t* dst) {
  if (count == 0) return true;
  if ((count + offset) > 512) {
    goto fail;
  }
  if (!inBlock_ || block != block_ || offset < offset_) {
    block_ = block;
    // use address if not SDHC card
    if (type()!= SD_CARD_TYPE_SDHC) block <<= 9;
    if (cardCommand(CMD17, block)) {
      error(SD_CARD_ERROR_CMD17);
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
  if (!partialBlockRead_ || offset_ >= 512) {
    // read rest of data, checksum and set chip select high
    readEnd();
  }
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
/** read CID or CSR register */
uint8_t Sd2Card::readRegister(uint8_t cmd, void* buf) {
  uint8_t* dst = reinterpret_cast<uint8_t*>(buf);
  if (cardCommand(cmd, 0)) {
    error(SD_CARD_ERROR_READ_REG);
    goto fail;
  }
  if (!waitStartBlock()) goto fail;
  // transfer data
  for (uint16_t i = 0; i < 16; i++) dst[i] = spiRec();
  spiRec();  // get first crc byte
  spiRec();  // get second crc byte
  SPI_CHIP_SELECT_HIGH();
  return true;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return false;
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
      error(SD_CARD_ERROR_READ_TIMEOUT);
      goto fail;
    }
  }
  if (status_ != DATA_START_BLOCK) {
    error(SD_CARD_ERROR_READ);
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
    error(SD_CARD_ERROR_CMD24);
    goto fail;
  }
  if (!writeData(DATA_START_BLOCK, src)) goto fail;

  // wait for flash programming to complete
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) {
    error(SD_CARD_ERROR_WRITE_TIMEOUT);
    goto fail;
  }
  // response is r2 so get and check two bytes for nonzero
  if (cardCommand(CMD13, 0) || spiRec()) {
    error(SD_CARD_ERROR_WRITE_PROGRAMMING);
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
    error(SD_CARD_ERROR_WRITE_MULTIPLE);
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
    error(SD_CARD_ERROR_WRITE);
    SPI_CHIP_SELECT_HIGH();
    return false;
  }
  return true;
}
//------------------------------------------------------------------------------
/** Start a write multiple blocks sequence.
 *
 * \param[in] blockNumber Address of first block in sequence.
 * \param[in] eraseCount The number of blocks to be pre-erased.
 *
 * \note This function is used with writeData() and writeStop()
 * for optimized multiple block writes.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t Sd2Card::writeStart(uint32_t blockNumber, uint32_t eraseCount) {
  // send pre-erase count
  if (cardAcmd(ACMD23, eraseCount)) {
    error(SD_CARD_ERROR_ACMD23);
    goto fail;
  }
  // use address if not SDHC card
  if (type() != SD_CARD_TYPE_SDHC) blockNumber <<= 9;
  if (cardCommand(CMD25, blockNumber)) {
    error(SD_CARD_ERROR_CMD25);
    goto fail;
  }
  return true;

 fail:
  SPI_CHIP_SELECT_HIGH();
  return false;
}
//------------------------------------------------------------------------------
/** End a write multiple blocks sequence.
 *
* \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t Sd2Card::writeStop(void) {
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) goto fail;
  spiSend(STOP_TRAN_TOKEN);
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) goto fail;
  SPI_CHIP_SELECT_HIGH();
  return true;

 fail:
  error(SD_CARD_ERROR_STOP_TRAN);
  SPI_CHIP_SELECT_HIGH();
  return false;
}




//------------------------------------------------------------------------------
// callback function for date/time
void (*SdFile::dateTime_)(uint16_t* date, uint16_t* time) = NULL;

#if ALLOW_DEPRECATED_FUNCTIONS
// suppress cpplint warnings with NOLINT comment
void (*SdFile::oldDateTime_)(uint16_t& date, uint16_t& time) = NULL;  // NOLINT
#endif  // ALLOW_DEPRECATED_FUNCTIONS
//------------------------------------------------------------------------------
// add a cluster to a file
uint8_t SdFile::addCluster() {
  if (!vol_->allocContiguous(1, &curCluster_)) return false;

  // if first cluster of file link to directory entry
  if (firstCluster_ == 0) {
    firstCluster_ = curCluster_;
    flags_ |= F_FILE_DIR_DIRTY;
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
/**
 * Return a files directory entry
 *
 * \param[out] dir Location for return of the files directory entry.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t SdFile::dirEntry(dir_t* dir) {
  // make sure fields on SD are correct
  if (!sync()) return false;

  // read entry
  dir_t* p = cacheDirEntry(SdVolume::CACHE_FOR_READ);
  if (!p) return false;

  // copy to caller's struct
  memcpy(dir, p, sizeof(dir_t));
  return true;
}
//------------------------------------------------------------------------------
// format directory name field from a 8.3 name string
uint8_t SdFile::make83Name(const char* str, uint8_t* name) {
  uint8_t c;
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
      //PGM_P p = PSTR("|<>^+=?/[];,*\"\\");
      //uint8_t b;
      //while ((b = pgm_read_byte(p++))) if (b == c) return false;
      // check size and only allow ASCII printable characters
      //if (i > n || c < 0X21 || c > 0X7E)return false;
      // only upper case allowed in 8.3 names - convert lower to upper
      name[i++] = c < 'a' || c > 'z' ?  c : c + ('A' - 'a');
    }
  }
  // must have a file name, extension is optional
  return name[0] != ' ';
}
//------------------------------------------------------------------------------
uint8_t SdFile::open(SdFile* dirFile, const char* fileName, uint8_t oflag) {
  uint8_t dname[11];
  dir_t* p;

  // error if already open
  if (isOpen()){errCode = 101;return false;}

  if (!make83Name(fileName, dname)){errCode = 102;return false;}
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
      // don't open existing file if O_CREAT and O_EXCL
      if ((oflag & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL)){errCode = 104; return false;}

      // open found file
      return openCachedEntry(0XF & index, oflag);
    }
  }
  // only create file if O_CREAT and O_WRITE
  if ((oflag & (O_CREAT | O_WRITE)) != (O_CREAT | O_WRITE)){errCode = 105; return false;}

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

  // set timestamps
  if (dateTime_) {
    // call user function
    dateTime_(&p->creationDate, &p->creationTime);
  } else {
    // use default date/time
    p->creationDate = FAT_DEFAULT_DATE;
    p->creationTime = FAT_DEFAULT_TIME;
  }
  p->lastAccessDate = p->creationDate;
  p->lastWriteDate = p->creationDate;
  p->lastWriteTime = p->creationTime;

  // force write of entry to SD
  if (!SdVolume::cacheFlush()){errCode = 109;  return false;}

  // open entry in cache
  return openCachedEntry(dirIndex_, oflag);
}
//------------------------------------------------------------------------------
// open a cached directory entry. Assumes vol_ is initializes
uint8_t SdFile::openCachedEntry(uint8_t dirIndex, uint8_t oflag) {
  // location of entry in cache
  dir_t* p = SdVolume::cacheBuffer_.dir + dirIndex;

  // write or truncate is an error for a directory or read-only file
  if (p->attributes & (DIR_ATT_READ_ONLY | DIR_ATT_DIRECTORY)) {
    if (oflag & (O_WRITE | O_TRUNC)){errCode = 200;  return false;}
  }
  // remember location of directory entry on SD
  dirIndex_ = dirIndex;
  dirBlock_ = SdVolume::cacheBlockNumber_;

  // copy first cluster number for directory fields
  firstCluster_ = (uint32_t)p->firstClusterHigh << 16;
  firstCluster_ |= p->firstClusterLow;

  // make sure it is a normal file or subdirectory
  if (DIR_IS_FILE(p)) {
    fileSize_ = p->fileSize;
    type_ = FAT_FILE_TYPE_NORMAL;
  } else if (DIR_IS_SUBDIR(p)) {
    if (!vol_->chainSize(firstCluster_, &fileSize_)){errCode = 201;  return false;}
    type_ = FAT_FILE_TYPE_SUBDIR;
  } else {
    errCode = 202; return false;
  }
  // save open flags for read/write
  flags_ = oflag & (O_ACCMODE | O_SYNC | O_APPEND);

  // set to start of file
  curCluster_ = 0;
  curPosition_ = 0;

  // truncate file to zero length if requested
  if (oflag & O_TRUNC) return truncate(0);
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
/** %Print a directory date field to Serial.
 *
 *  Format is yyyy-mm-dd.
 *
 * \param[in] fatDate The date field from a directory entry.
 */
void SdFile::printFatDate(uint16_t fatDate) {
  Serial.print(FAT_YEAR(fatDate));
  Serial.print('-');
  printTwoDigits(FAT_MONTH(fatDate));
  Serial.print('-');
  printTwoDigits(FAT_DAY(fatDate));
}
//------------------------------------------------------------------------------
/** %Print a directory time field to Serial.
 *
 * Format is hh:mm:ss.
 *
 * \param[in] fatTime The time field from a directory entry.
 */
void SdFile::printFatTime(uint16_t fatTime) {
  printTwoDigits(FAT_HOUR(fatTime));
  Serial.print(':');
  printTwoDigits(FAT_MINUTE(fatTime));
  Serial.print(':');
  printTwoDigits(FAT_SECOND(fatTime));
}
//------------------------------------------------------------------------------
/** %Print a value as two digits to Serial.
 *
 * \param[in] v Value to be printed, 0 <= \a v <= 99
 */
void SdFile::printTwoDigits(uint8_t v) {
  char str[3];
  str[0] = '0' + v/10;
  str[1] = '0' + v % 10;
  str[2] = 0;
  Serial.print(str);
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

  // error if not open or write only
  if (!isOpen() || !(flags_ & O_READ)) return -1;

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
    if ((unbufferedRead() || n == 512) &&
      block != SdVolume::cacheBlockNumber_) {
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
/**
 * Read the next directory entry from a directory file.
 *
 * \param[out] dir The dir_t struct that will receive the data.
 *
 * \return For success readDir() returns the number of bytes read.
 * A value of zero will be returned if end of file is reached.
 * If an error occurs, readDir() returns -1.  Possible errors include
 * readDir() called before a directory has been opened, this is not
 * a directory file or an I/O error occurred.
 */
int8_t SdFile::readDir(dir_t* dir) {
  int8_t n;
  // if not a directory file or miss-positioned return an error
  if (!isDir() || (0X1F & curPosition_)) return -1;

  while ((n = read(dir, sizeof(dir_t))) == sizeof(dir_t)) {
    // last entry if DIR_NAME_FREE
    if (dir->name[0] == DIR_NAME_FREE) break;
    // skip empty entries and entry for .  and ..
    if (dir->name[0] == DIR_NAME_DELETED || dir->name[0] == '.') continue;
    // return if normal file or subdirectory
    if (DIR_IS_FILE_OR_SUBDIR(dir)) return n;
  }
  // error, end of file, or past last entry
  return n < 0 ? -1 : 0;
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
 * Remove a file.
 *
 * The directory entry and all data for the file are deleted.
 *
 * \note This function should not be used to delete the 8.3 version of a
 * file that has a long name. For example if a file has the long name
 * "New Text Document.txt" you should not delete the 8.3 name "NEWTEX~1.TXT".
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include the file read-only, is a directory,
 * or an I/O error occurred.
 */
uint8_t SdFile::remove(void) {
  // free any clusters - will fail if read-only or directory
  if (!truncate(0)) return false;

  // cache directory entry
  dir_t* d = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
  if (!d) return false;

  // mark entry deleted
  d->name[0] = DIR_NAME_DELETED;

  // set this SdFile closed
  type_ = FAT_FILE_TYPE_CLOSED;

  // write entry to SD
  return SdVolume::cacheFlush();
}
//------------------------------------------------------------------------------
/**
 * Remove a file.
 *
 * The directory entry and all data for the file are deleted.
 *
 * \param[in] dirFile The directory that contains the file.
 * \param[in] fileName The name of the file to be removed.
 *
 * \note This function should not be used to delete the 8.3 version of a
 * file that has a long name. For example if a file has the long name
 * "New Text Document.txt" you should not delete the 8.3 name "NEWTEX~1.TXT".
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include the file is a directory, is read only,
 * \a dirFile is not a directory, \a fileName is not found
 * or an I/O error occurred.
 */
uint8_t SdFile::remove(SdFile* dirFile, const char* fileName) {
  SdFile file;
  if (!file.open(dirFile, fileName, O_WRITE)) return false;
  return file.remove();
}
//------------------------------------------------------------------------------
/** Remove a directory file.
 *
 * The directory file will be removed only if it is empty and is not the
 * root directory.  rmDir() follows DOS and Windows and ignores the
 * read-only attribute for the directory.
 *
 * \note This function should not be used to delete the 8.3 version of a
 * directory that has a long name. For example if a directory has the
 * long name "New folder" you should not delete the 8.3 name "NEWFOL~1".
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 * Reasons for failure include the file is not a directory, is the root
 * directory, is not empty, or an I/O error occurred.
 */
uint8_t SdFile::rmDir(void) {
  // must be open subdirectory
  if (!isSubDir()) return false;

  rewind();

  // make sure directory is empty
  while (curPosition_ < fileSize_) {
    dir_t* p = readDirCache();
    if (p == NULL) return false;
    // done if past last used entry
    if (p->name[0] == DIR_NAME_FREE) break;
    // skip empty slot or '.' or '..'
    if (p->name[0] == DIR_NAME_DELETED || p->name[0] == '.') continue;
    // error not empty
    if (DIR_IS_FILE_OR_SUBDIR(p)) return false;
  }
  // convert empty directory to normal file for remove
  type_ = FAT_FILE_TYPE_NORMAL;
  flags_ |= O_WRITE;
  return remove();
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

  if (flags_ & F_FILE_DIR_DIRTY) {
    dir_t* d = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
    if (!d) return false;

    // do not set filesize for dir files
    if (!isDir()) d->fileSize = fileSize_;

    // update first cluster fields
    d->firstClusterLow = firstCluster_ & 0XFFFF;
    d->firstClusterHigh = firstCluster_ >> 16;

    // set modify time if user supplied a callback date/time function
    if (dateTime_) {
      dateTime_(&d->lastWriteDate, &d->lastWriteTime);
      d->lastAccessDate = d->lastWriteDate;
    }
    // clear directory dirty
    flags_ &= ~F_FILE_DIR_DIRTY;
  }
  return SdVolume::cacheFlush();
}
//------------------------------------------------------------------------------
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
uint8_t SdFile::truncate(uint32_t length) {
// error if not a normal file or read-only
  if (!isFile() || !(flags_ & O_WRITE)){errCode = 50;  return false;}

  // error if length is greater than current size
  if (length > fileSize_){errCode = 51;  return false;}

  // fileSize and length are zero - nothing to do
  if (fileSize_ == 0){errCode = 52;  return true;}

  // remember position for seek after truncation
  uint32_t newPos = curPosition_ > length ? length : curPosition_;

  // position to last cluster in truncated file
  if (!seekSet(length)){errCode = 53;  return false;}

  if (length == 0) {
    // free all clusters
    if (!vol_->freeChain(firstCluster_)) {errCode = 54; return false;}
    firstCluster_ = 0;
  } else {
    uint32_t toFree;
    if (!vol_->fatGet(curCluster_, &toFree)) {errCode = 55; return false;}

    if (!vol_->isEOC(toFree)) {
      // free extra clusters
      if (!vol_->freeChain(toFree)) {errCode = 56; return false;}

      // current cluster is end of chain
      if (!vol_->fatPutEOC(curCluster_)){errCode = 57;  return false;}
    }
  }
  fileSize_ = length;

  // need to update directory entry
  flags_ |= F_FILE_DIR_DIRTY;

  if (!sync()) {errCode = 58; return false;}

  // set file to correct position
  return seekSet(newPos);
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

  // error if not a normal file or is read-only
  if (!isFile() || !(flags_ & O_WRITE)) goto writeErrorReturn;

  // seek to end of file if append flag
  if ((flags_ & O_APPEND) && curPosition_ != fileSize_) {
    if (!seekEnd()) goto writeErrorReturn;
  }

  while (nToWrite > 0) {
    uint8_t blockOfCluster = vol_->blockOfCluster(curPosition_);
    uint16_t blockOffset = curPosition_ & 0X1FF;
    if (blockOfCluster == 0 && blockOffset == 0) {
      // start of new cluster
      if (curCluster_ == 0) {
        if (firstCluster_ == 0) {
          // allocate first cluster of file
          if (!addCluster()) goto writeErrorReturn;
        } else {
          curCluster_ = firstCluster_;
        }
      } else {
        uint32_t next;
        if (!vol_->fatGet(curCluster_, &next)) return false;
        if (vol_->isEOC(next)) {
          // add cluster if at end of chain
          if (!addCluster()) goto writeErrorReturn;
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
      if (!vol_->writeBlock(block, src)) goto writeErrorReturn;
      src += 512;
    } else {
      if (blockOffset == 0 && curPosition_ >= fileSize_) {
        // start of new block don't need to read into cache
        if (!SdVolume::cacheFlush()) goto writeErrorReturn;
        SdVolume::cacheBlockNumber_ = block;
        SdVolume::cacheSetDirty();
      } else {
        // rewrite part of block
        if (!SdVolume::cacheRawBlock(block, SdVolume::CACHE_FOR_WRITE)) {
          goto writeErrorReturn;
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
    flags_ |= F_FILE_DIR_DIRTY;
  } else if (dateTime_ && nbyte) {
    // insure sync will update modified date and time
    flags_ |= F_FILE_DIR_DIRTY;
  }

  if (flags_ & O_SYNC) {
    if (!sync()) goto writeErrorReturn;
  }
  return nbyte;

 writeErrorReturn:
  // return for write error
  //writeError = true;
  setWriteError();
  return 0;
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
uint8_t SdVolume::allocContiguous(uint32_t count, uint32_t* curCluster) {
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
//------------------------------------------------------------------------------
// free a cluster chain
uint8_t SdVolume::freeChain(uint32_t cluster) {
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
uint8_t SdVolume::init(Sd2Card* dev, uint8_t part) {
  uint32_t volumeStartBlock = 0;
  sdCard_ = dev;
  // if part == 0 assume super floppy with FAT boot sector in block zero
  // if part > 0 assume mbr volume with partition table
  if (part) {
    if (part > 4)return false;
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
  blocksPerFat_ = bpb->sectorsPerFat16 ?
                    bpb->sectorsPerFat16 : bpb->sectorsPerFat32;

  fatStartBlock_ = volumeStartBlock + bpb->reservedSectorCount;

  // count for FAT16 zero for FAT32
  rootDirEntryCount_ = bpb->rootDirEntryCount;

  // directory start for FAT16 dataStart for FAT32
  rootDirStart_ = fatStartBlock_ + bpb->fatCount * blocksPerFat_;

  // data start for FAT16 and FAT32
  dataStartBlock_ = rootDirStart_ + ((32 * bpb->rootDirEntryCount + 511)/512);

  // total blocks for FAT16 or FAT32
  uint32_t totalBlocks = bpb->totalSectors16 ?
                           bpb->totalSectors16 : bpb->totalSectors32;
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



File::File(SdFile f, const char *n) {
  // oh man you are kidding me, new() doesnt exist? Ok we do it by hand!
  _file = (SdFile *)malloc(sizeof(SdFile)); 
  if (_file) {
    memcpy(_file, &f, sizeof(SdFile));
    
    strncpy(_name, n, 12);
    _name[12] = 0;
    
  }
}

File::File(void) {
  _file = 0;
  _name[0] = 0;
  //Serial.print("Created empty file object");
}

// returns a pointer to the file name
char *File::name(void) {
  return _name;
}


size_t File::write(uint8_t val) {
  return write(&val, 1);
}

size_t File::write(const uint8_t *buf, size_t size) {
  size_t t;
  if (!_file) {
    setWriteError();
    return 0;
  }
  _file->clearWriteError();
  t = _file->write(buf, size);
  if (_file->getWriteError()) {
    setWriteError();
    return 0;
  }
  return t;
}

int File::peek() {
  if (! _file) 
    return 0;

  int c = _file->read();
  if (c != -1) _file->seekCur(-1);
  return c;
}

int File::read() {
  if (_file) 
    return _file->read();
  return -1;
}

// buffered read for more efficient, high speed reading
int File::read(void *buf, uint16_t nbyte) {
  if (_file) 
    return _file->read(buf, nbyte);
  return 0;
}

int File::available() {
  if (! _file) return 0;

  uint32_t n = size() - position();

  return n > 0X7FFF ? 0X7FFF : n;
}

void File::flush() {
  if (_file)
    _file->sync();
}

boolean File::seek(uint32_t pos) {
  if (! _file) return false;

  return _file->seekSet(pos);
}

uint32_t File::position() {
  if (! _file) return -1;
  return _file->curPosition();
}

uint32_t File::size() {
  if (! _file) return 0;
  return _file->fileSize();
}

void File::close() {
  if (_file) {
    _file->close();
    free(_file); 
    _file = 0;
  }
}

File::operator bool() {
  if (_file) 
    return  _file->isOpen();
  return false;
}




#define MAX_COMPONENT_LEN 12 // What is max length?
#define PATH_COMPONENT_BUFFER_LEN MAX_COMPONENT_LEN+1

boolean SDClass::begin() {
  return card.init() && volume.init(card) && root.openRoot(volume);
}

File SDClass::openSimple(const char *filepath, uint8_t mode,uint8_t toend) {
  // Open the file itself
  SdFile file;

	if ( ! file.open(SD.root, filepath, mode)) {
	  // failed to open the file :(
	  return File();
	}

  if (toend) 
    file.seekSet(file.fileSize());
  return File(file, filepath);
}

SDClass SD;
