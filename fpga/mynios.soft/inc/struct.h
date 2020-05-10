
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
// End Of Chain values for FAT entries
#define FAT16EOC      0XFFFF
#define FAT16EOC_MIN  0XFFF8
#define FAT32EOC      0X0FFFFFFF
#define FAT32EOC_MIN  0X0FFFFFF8
#define FAT32MASK     0X0FFFFFFF

#define DIR_NAME_DELETED        0XE5
#define DIR_NAME_FREE           0X00
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
